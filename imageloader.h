#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

typedef union Color {
    uint8_t values[3]; // 0: R, 1: G, 2: B
    struct {
        uint8_t R;
        uint8_t G;
        uint8_t B;
    };
} Color;
typedef struct Image {
    Color** image;
    uint32_t rows;
    uint32_t cols;
} Image;

// Loads an image from a ppm file, and converts to an Image object
extern Image* readData(char* filename);

// Writes an image to stdout
extern void writeData(Image* image);

// Frees an image and all its associated data
extern void freeImage(Image* image);
