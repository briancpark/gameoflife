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

    Image *img;
    img = (Image *) malloc(sizeof(struct Image));
    
    char ft[2];
    uint32_t cols, rows;
   
    fscanf(fp, "%s", ft);
    fscanf(fp, "%hhu %hhu", &cols, &rows);
    fscanf(fp, "%*d");
    
    img->rows = (uint32_t*) malloc(sizeof(uint32_t));
    img->cols = (uint32_t*) malloc(sizeof(uint32_t));

    img->rows = rows;
    img->cols = cols;

    //Dynamically allocate an array of struct

    int i;
    int size_r = (int) rows;
    int size_c = (int) cols;
    
    Color **color_arr = (Color **) malloc(sizeof(Color*) * size_r);

    if (color_arr == NULL) {
        printf("out of memory");
        return 0;
    }

    *color_arr = (Color*) malloc(sizeof(Color) * size_r * size_c);

    for (i = 0; i < size_r; i++) {
        color_arr[i] = (*color_arr + size_r*i);
    }
    
    for (int i = 0; i < size_r; i++) {
        for (int j = 0; j < size_c; j++) {
            fscanf(fp, "%hhu %hhu %hhu", &color_arr[i][j].R, &color_arr[i][j].G, &color_arr[i][j].B);
        }
    }

    fclose(fp);
    
    img->image = color_arr; 

    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {

	printf("P3\n");
    printf("%hhu %hhu\n", &image->rows, &image->cols);
    printf("255\n");

    int r = (int) image->rows;
    int c = (int) image->cols;

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%hhu %hhu %hhu", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
            printf("   ");
        }
        printf("\n");
    }
    
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE

    free(image->image);
    free(image);
}
