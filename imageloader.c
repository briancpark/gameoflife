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
    
    //Do a file pointer null check here
    if (fp == NULL) {
        printf("File doesn't exist. Exit with code 0");
        fclose(fp);
        return 0;
    }

    printf("file successfully opened");
    
    struct Image *image;
    
    char ft[20];   //Leave it as an arbitrary number for now (file type)
    uint32_t cols, rows;
    uint8_t scale;
    
    fscanf(fp, "%s\n", ft);
    fscanf(fp, "%"PRIu32 "%"PRIu32"\n", cols, rows);
    fscanf(fp, "%hhu\n", scale);

    image->rows = rows;
    image->cols = cols;

    struct Color *color = (struct Color *) malloc(rows * cols * sizeof(Color));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            struct Color color = (struct Color*) malloc(sizeof(struct Color));
            color->R = (uint8_t*) malloc(sizeof(uint8_t)); 
            color->G = (uint8_t*) malloc(sizeof(uint8_t));
            color->B = (uint8_t*) malloc(sizeof(uint8_t));
         
            fscanf(fp, "%u %u %u", color->R, color->G, color->B);
            image[i][j] = color;
            
        }
    }
    fclose(fp);
    return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {

	printf("P3\n");
    printf("%u %u\n", &image->rows, &image->cols);
    printf("255\n");
    for (int i = 0; i < *image->rows; i++) {
        for (int j = 0; j < *image->cols; j++) {
            printf("0");
        }
        printf("\n");
    }
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
    free(image);
}
