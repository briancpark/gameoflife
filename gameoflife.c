/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Brian Park
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include "imageloader.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(__AVX512F__)
#include <immintrin.h>
#elif defined(__ARM_NEON)
#include <arm_neon.h>
#endif

// My custom modulo function
static inline int mod(int n, int m) {
    int result = n % m;
    return result < 0 ? result + m : result;
}

// Determines what color the cell at the given row/col should be. This function allocates space for
// a new Color. Note that you will need to read the eight neighbors of the cell in question. The
// grid "wraps", so we treat the top row as adjacent to the bottom row and the left column as
// adjacent to the right column.
Color* evaluateOneCell(Image* image, int row, int col, uint32_t rule) {
    Color** img = image->image;

    uint32_t alive = rule >> 9;
    uint32_t dead = rule & 511;

    int rows = image->rows;
    int cols = image->cols;

    // Allocate and initialize new_color
    Color* new_color = (Color*)calloc(1, sizeof(Color));
    if (new_color == NULL) {
        printf("Memory allocation failed.\n");
        exit(-1);
    }

    // Precompute neighbor positions
    int neighbor_rows[8] = {mod(row - 1, rows),
                            mod(row, rows),
                            mod(row + 1, rows),
                            mod(row - 1, rows),
                            /* Current Pixel */ mod(row + 1, rows),
                            mod(row - 1, rows),
                            mod(row, rows),
                            mod(row + 1, rows)};
    int neighbor_cols[8] = {mod(col - 1, cols),
                            mod(col - 1, cols),
                            mod(col - 1, cols),
                            mod(col, cols),
                            /* Current Pixel */ mod(col, cols),
                            mod(col + 1, cols),
                            mod(col + 1, cols),
                            mod(col + 1, cols)};

    for (int i = 0; i < 8; i++) {
#pragma unroll
        for (int channel = 0; channel < 3; channel++) {
            int neighbors = 0;
#pragma unroll 8
            for (int n = 0; n < 8; n++) {
                neighbors +=
                    ((img[neighbor_rows[n] * cols + neighbor_cols[n]]->values[channel] >> i) & 1);
            }

            int current_bit = (img[row * cols + col]->values[channel] >> i) & 1;
            if (((current_bit == 1 && (alive >> neighbors) & 1) ||
                 (current_bit == 0 && (dead >> neighbors) & 1))) {
                new_color->values[channel] |= (1 << i);
            }
        }
    }

    return new_color;
}

// Fast path: each pixel's 24 bits (R, G, B x 8 bit-planes) are 24 independent Life boards, so we
// pack a pixel into one uint32_t and compute all 24 boards at once with bit-sliced adders. Bit k of
// every intermediate word belongs to board k. Vectors process 16 (AVX-512) or 4 (NEON) pixels.

// 2-way mux on every bit: s ? a : b
#define MUX(s, a, b) (((s) & (a)) | (~(s) & (b)))

// Applies a 9-bit birth/survival mask to the bit-sliced neighbor count (c3 c2 c1 c0). Count 8 is
// the only value with c3 set, so the low 8 bits form a 3-input LUT on (c2, c1, c0).
static inline uint32_t applyMask(uint32_t mask, uint32_t c3, uint32_t c2, uint32_t c1,
                                 uint32_t c0) {
    uint32_t m[9];
    for (int n = 0; n < 9; n++) {
        m[n] = ((mask >> n) & 1) ? ~0u : 0u;
    }
    uint32_t lo = MUX(c2, MUX(c1, MUX(c0, m[7], m[6]), MUX(c0, m[5], m[4])),
                      MUX(c1, MUX(c0, m[3], m[2]), MUX(c0, m[1], m[0])));
    return MUX(c3, m[8], lo);
}

// Scalar version of the bit-sliced step, used for the row tail and for non-AVX-512 builds.
static inline uint32_t stepWord(const uint32_t* up, const uint32_t* mid, const uint32_t* down,
                                uint32_t alive, uint32_t dead) {
    uint32_t a = up[0], b = up[1], c = up[2], d = mid[0], e = mid[2], f = down[0], g = down[1],
             h = down[2], cur = mid[1];

    // Carry-save adder tree summing 8 one-bit inputs per lane into a 4-bit count.
    uint32_t s1 = a ^ b ^ c, k1 = (a & b) | (c & (a ^ b));
    uint32_t s2 = f ^ g ^ h, k2 = (f & g) | (h & (f ^ g));
    uint32_t s3 = d ^ e, k3 = d & e;
    uint32_t c0 = s1 ^ s2 ^ s3, t1 = (s1 & s2) | (s3 & (s1 ^ s2));
    uint32_t u1 = k1 ^ k2 ^ k3, u2 = (k1 & k2) | (k3 & (k1 ^ k2));
    uint32_t c1 = t1 ^ u1, w = t1 & u1;
    uint32_t c2 = u2 ^ w, c3 = u2 & w;

    return MUX(cur, applyMask(alive, c3, c2, c1, c0), applyMask(dead, c3, c2, c1, c0));
}

// Vector backends. Each provides a register type holding LANES packed pixels, plus loads, stores
// and the bitwise primitives used by the kernel: VMUX(s, a, b) = s ? a : b per bit,
// VXOR3 = 3-way xor (full-adder sum), VMAJ = majority (full-adder carry).
#if defined(__AVX512F__)
#define HAVE_VEC 1
#define LANES    16
typedef __m512i vec;
#define VLOAD(p)     _mm512_loadu_si512(p)
#define VSTORE(p, v) _mm512_storeu_si512((p), (v))
#define VSET1(x)     _mm512_set1_epi32((int)(x))
#define VXOR(a, b)   _mm512_xor_si512((a), (b))
#define VAND(a, b)   _mm512_and_si512((a), (b))
// vpternlogd truth tables: 0xCA = A ? B : C, 0x96 = A ^ B ^ C, 0xE8 = majority.
#define VMUX(s, a, b)  _mm512_ternarylogic_epi32((s), (a), (b), 0xCA)
#define VXOR3(a, b, c) _mm512_ternarylogic_epi32((a), (b), (c), 0x96)
#define VMAJ(a, b, c)  _mm512_ternarylogic_epi32((a), (b), (c), 0xE8)
#elif defined(__ARM_NEON)
#define HAVE_VEC      1
#define LANES         4
typedef uint32x4_t vec;
#define VLOAD(p)      vld1q_u32(p)
#define VSTORE(p, v)  vst1q_u32((p), (v))
#define VSET1(x)      vdupq_n_u32(x)
#define VXOR(a, b)    veorq_u32((a), (b))
#define VAND(a, b)    vandq_u32((a), (b))
#define VMUX(s, a, b) vbslq_u32((s), (a), (b))
#ifdef __ARM_FEATURE_SHA3
#define VXOR3(a, b, c) veor3q_u32((a), (b), (c))
#else
#define VXOR3(a, b, c) veorq_u32(veorq_u32((a), (b)), (c))
#endif
// Majority: where a and b differ, c decides; otherwise a (== b) does.
#define VMAJ(a, b, c) vbslq_u32(veorq_u32((a), (b)), (c), (a))
#endif

#ifdef HAVE_VEC
static inline vec applyMaskVec(const vec* m, vec c3, vec c2, vec c1, vec c0) {
    vec lo = VMUX(c2, VMUX(c1, VMUX(c0, m[7], m[6]), VMUX(c0, m[5], m[4])),
                  VMUX(c1, VMUX(c0, m[3], m[2]), VMUX(c0, m[1], m[0])));
    return VMUX(c3, m[8], lo);
}

static inline vec stepVec(const uint32_t* up, const uint32_t* mid, const uint32_t* down,
                          const vec* ma, const vec* md) {
    vec a = VLOAD(up), b = VLOAD(up + 1), c = VLOAD(up + 2);
    vec d = VLOAD(mid), cur = VLOAD(mid + 1), e = VLOAD(mid + 2);
    vec f = VLOAD(down), g = VLOAD(down + 1), h = VLOAD(down + 2);

    vec s1 = VXOR3(a, b, c), k1 = VMAJ(a, b, c);
    vec s2 = VXOR3(f, g, h), k2 = VMAJ(f, g, h);
    vec s3 = VXOR(d, e), k3 = VAND(d, e);
    vec c0 = VXOR3(s1, s2, s3), t1 = VMAJ(s1, s2, s3);
    vec u1 = VXOR3(k1, k2, k3), u2 = VMAJ(k1, k2, k3);
    vec c1 = VXOR(t1, u1), w = VAND(t1, u1);
    vec c2 = VXOR(u2, w), c3 = VAND(u2, w);

    return VMUX(cur, applyMaskVec(ma, c3, c2, c1, c0), applyMaskVec(md, c3, c2, c1, c0));
}
#endif

// The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
// You should be able to copy most of this from steganography.c
Image* life(Image* image, uint32_t rule) {
    uint32_t rows = image->rows;
    uint32_t cols = image->cols;
    uint32_t alive = rule >> 9;
    uint32_t dead = rule & 511;

    Image* new_image = (Image*)malloc(sizeof(struct Image));
    // Packed grid with one wrapped column on each side, and a packed output buffer.
    size_t stride = (size_t)cols + 2;
    uint32_t* grid = (uint32_t*)malloc(sizeof(uint32_t) * stride * rows);
    uint32_t* out = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)cols * rows);
    Color** color_arr = (Color**)malloc(sizeof(Color*) * (size_t)rows * cols);
    Color* colors = (Color*)malloc(sizeof(Color) * (size_t)rows * cols);

    if (new_image == NULL || grid == NULL || out == NULL || color_arr == NULL || colors == NULL) {
        printf("Memory allocation failed.\n");
        free(new_image);
        free(grid);
        free(out);
        free(color_arr);
        free(colors);
        freeImage(image);
        exit(-1);
    }

    new_image->rows = rows;
    new_image->cols = cols;

#pragma omp parallel
    {
#pragma omp for schedule(static)
        for (uint32_t i = 0; i < rows; i++) {
            Color** src = image->image + (size_t)i * cols;
            uint32_t* dst = grid + i * stride;
            for (uint32_t j = 0; j < cols; j++) {
                dst[j + 1] = src[j]->R | (uint32_t)src[j]->G << 8 | (uint32_t)src[j]->B << 16;
            }
            dst[0] = dst[cols];
            dst[cols + 1] = dst[1];
        }

#ifdef HAVE_VEC
        vec ma[9], md[9];
        for (int n = 0; n < 9; n++) {
            ma[n] = VSET1(((alive >> n) & 1) ? ~0u : 0u);
            md[n] = VSET1(((dead >> n) & 1) ? ~0u : 0u);
        }
#endif

#pragma omp for schedule(static)
        for (uint32_t i = 0; i < rows; i++) {
            const uint32_t* up = grid + (size_t)(i == 0 ? rows - 1 : i - 1) * stride;
            const uint32_t* mid = grid + (size_t)i * stride;
            const uint32_t* down = grid + (size_t)(i == rows - 1 ? 0 : i + 1) * stride;
            uint32_t* dst = out + (size_t)i * cols;
            uint32_t j = 0;
#ifdef HAVE_VEC
            for (; j + LANES <= cols; j += LANES) {
                VSTORE(dst + j, stepVec(up + j, mid + j, down + j, ma, md));
            }
#endif
            for (; j < cols; j++) {
                dst[j] = stepWord(up + j, mid + j, down + j, alive, dead);
            }
        }

#pragma omp for schedule(static)
        for (size_t p = 0; p < (size_t)rows * cols; p++) {
            Color* color = colors + p;
            color->R = out[p];
            color->G = out[p] >> 8;
            color->B = out[p] >> 16;
            color_arr[p] = color;
        }
    }

    free(grid);
    free(out);
    new_image->image = color_arr;
    return new_image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the
new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code
-1. Otherwise, you should return from main with code 0. Make sure to free all memory before
returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char** argv) {
    Image* image;

    if (argc != 3) {
        printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with "
               "maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
        exit(-1);
    }

    char* filename = argv[1];
    uint32_t rule;
    char* str;
    rule = strtol(argv[2], &str, 16);

    image = readData(filename);
    Image* new_image = life(image, rule);
    freeImage(image);
    writeData(new_image);
    freeImage(new_image);

    return 0;
}
