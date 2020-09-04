/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Brian Park
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col) {
    
    Color *color = (Color *) malloc(sizeof(struct Color));

    if (color == NULL) {
        printf("Memory allocation failed.");
        exit(-1);
    }

    if (image->image[row][col].B & 1) {
        color->R = 255;
        color->G = 255;
        color->B = 255;
    } else {
        color->R = 0;
        color->G = 0;
        color->B = 0;
    }

    return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image) {
    
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            Color *color;
            color = evaluateOnePixel(image, i, j);
            image->image[i][j].R = color->R;
            image->image[i][j].G = color->G;
            image->image[i][j].B = color->B;

            free(color);
        }
    }
    return image;
}

/*
Loads a .ppm from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv) {
    
    Image *image;
    
    if (argc != 2) {
        printf("usage: %s filename\n", argv[0]);
        printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        exit(-1);
    }
    
    char *filename = argv[1];
    image = readData(filename);
    image = steganography(image);
    writeData(image);
    freeImage(image);

    return 0;
}
