/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Brian Park
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) {
	FILE *fp = fopen(filename, "r");
    
    Image *image;
    char ft[2];
    uint32_t cols, rows, scale;
    
    fscanf(fp, "%s", ft);
    fscanf(fp, "%d %d", cols, rows);
    fscanf(fp, "%d", scale);

    image->rows = rows;
    image->cols = cols;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Color *color;
            fscanf(fp, "%d %d %d", color->R, color->G, color->B);
            image[i][j] = color;
            
        }
    }


    fclose(fp);
    return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
}
