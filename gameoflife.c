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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
    
    Color *color = (Color*) malloc(sizeof(struct Color));
    
    uint32_t alive = rule >> 9;
    uint32_t dead = rule & 511;

    for () {
    
    }


    for (int i = 0; i < 9; i++) {
         if (alive >> i & 1) {
            
         }
    
    }


    image->image[mod(i - 1, i)][mod(j - 1 , j)].R;
    image->image[mod(i, i)][mod(j - 1, j)].R;
    image->image[mod(i + 1, i)][mod(j - 1, j)].R;
    image->image[mod(i - 1, i)][mod(j, j)].R;
    image->image[mod(i + 1, i)][mod(j, j)].R;
    image->image[mod(i - 1, i)][mod(j + 1, j)].R;
    image->image[mod(i, i)][mod(j + 1, j)].R;
    image->image[mod(i + 1, i)][mod(j + 1, j)].R;
    
    




    return color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule) {
	for (int i = 0; image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            Color *color;
            color = evaluateOneCell(image, i, j, rule);
            image->image[i][j].R = color->R;
            image->image[i][j].G = color->G;
            image->image[i][j].B = color->B;

            free(color);
        }
    }
    return image;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv) {
    Image *image;

    if (argc != 3) {
        //printf();
        exit(-1);
    }

    char *filename = argv[1];
    uint32_t rule;
    char *str;
    rule = strtol(argv[2], &str, 16);
    
    image = readData(filename);
    image = life(image, rule);
    writeData(image);
    freeImage(image);

    return 0;
}

//My custom modulo function
int mod (int n, int m) {
    if (n < 0) {
        return n + m % m;
    } else {
        return n % m;
    }
}

