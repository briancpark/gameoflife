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

// My custom modulo function
int mod(int n, int m) {
    if (n < 0) {
        return n + m;
    } else {
        return n % m;
    }
}

// Determines what color the cell at the given row/col should be. This function allocates space for
// a new Color. Note that you will need to read the eight neighbors of the cell in question. The
// grid "wraps", so we treat the top row as adjacent to the bottom row and the left column as
// adjacent to the right column.
Color* evaluateOneCell(Image* image, int row, int col, uint32_t rule) {

    Color** img = image->image;
    Color* new_color = (Color*)malloc(sizeof(struct Color));

    if (new_color == NULL) {
        printf("Memory allocation failed.\n");
        exit(-1);
    }

    new_color->R = 0;
    new_color->G = 0;
    new_color->B = 0;

    uint32_t alive = rule >> 9;
    uint32_t dead = rule & 511;

    for (int i = 7; i >= 0; i--) {
        int neighbors = 0;

        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col - 1, image->cols)))->R >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row, image->rows) * image->cols + mod(col - 1, image->cols)))->R >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col - 1, image->cols)))->R >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col, image->cols)))->R >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col, image->cols)))->R >> i) &
            1;
        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col + 1, image->cols)))->R >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row, image->rows) * image->cols + mod(col + 1, image->cols)))->R >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col + 1, image->cols)))->R >>
             i) &
            1;

        if ((((*(img + row * image->cols + col))->R >> i) & 1) == 1 &&
            ((alive >> neighbors) & 1) == 1) {
            new_color->R = new_color->R | (1 << i);
        }

        if ((((*(img + row * image->cols + col))->R >> i) & 1) == 0 &&
            ((dead >> neighbors) & 1) == 1) {
            new_color->R = new_color->R | (1 << i);
        }
    }

    for (int i = 7; i >= 0; i--) {
        int neighbors = 0;

        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col - 1, image->cols)))->G >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row, image->rows) * image->cols + mod(col - 1, image->cols)))->G >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col - 1, image->cols)))->G >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col, image->cols)))->G >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col, image->cols)))->G >> i) &
            1;
        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col + 1, image->cols)))->G >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row, image->rows) * image->cols + mod(col + 1, image->cols)))->G >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col + 1, image->cols)))->G >>
             i) &
            1;

        if ((((*(img + row * image->cols + col))->G >> i) & 1) == 1 &&
            ((alive >> neighbors) & 1) == 1) {
            new_color->G = new_color->G | (1 << i);
        }

        if ((((*(img + row * image->cols + col))->G >> i) & 1) == 0 &&
            ((dead >> neighbors) & 1) == 1) {
            new_color->G = new_color->G | (1 << i);
        }
    }

    for (int i = 7; i >= 0; i--) {
        int neighbors = 0;

        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col - 1, image->cols)))->B >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row, image->rows) * image->cols + mod(col - 1, image->cols)))->B >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col - 1, image->cols)))->B >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col, image->cols)))->B >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col, image->cols)))->B >> i) &
            1;
        neighbors +=
            ((*(img + mod(row - 1, image->rows) * image->cols + mod(col + 1, image->cols)))->B >>
             i) &
            1;
        neighbors +=
            ((*(img + mod(row, image->rows) * image->cols + mod(col + 1, image->cols)))->B >> i) &
            1;
        neighbors +=
            ((*(img + mod(row + 1, image->rows) * image->cols + mod(col + 1, image->cols)))->B >>
             i) &
            1;

        if ((((*(img + row * image->cols + col))->B >> i) & 1) == 1 &&
            ((alive >> neighbors) & 1) == 1) {
            new_color->B = new_color->B | (1 << i);
        }

        if ((((*(img + row * image->cols + col))->B >> i) & 1) == 0 &&
            ((dead >> neighbors) & 1) == 1) {
            new_color->B = new_color->B | (1 << i);
        }
    }

    return new_color;
}

// The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
// You should be able to copy most of this from steganography.c
Image* life(Image* image, uint32_t rule) {

    Image* new_image;
    new_image = (Image*)malloc(sizeof(struct Image));

    if (new_image == NULL) {
        printf("Memory allocation failed.\n");
        freeImage(image);
        exit(-1);
    }

    new_image->rows = image->rows;
    new_image->cols = image->cols;

    Color** color_arr = (Color**)malloc(sizeof(Color*) * new_image->rows * new_image->cols);

    if (color_arr == NULL) {
        printf("Memory allocation failed.\n");
        free(new_image);
        freeImage(image);
        exit(-1);
    }

    for (uint32_t i = 0; i < new_image->rows; i++) {
        for (uint32_t j = 0; j < new_image->cols; j++) {
            *(color_arr + i * image->cols + j) = evaluateOneCell(image, i, j, rule);
        }
    }

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
