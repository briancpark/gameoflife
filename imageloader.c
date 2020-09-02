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

    struct Image *img;
    img = malloc(sizeof(struct Image));
    
    char ft[2];   //Leave it as an arbitrary number for now (file type)
    uint32_t *cols, *rows;
    uint8_t *scale = (uint8_t*) malloc(sizeof(uint8_t));
   
    fscanf(fp, "%s\n", ft);
    fscanf(fp, "%"SCNu32 "%"SCNu32"\n", &cols, &rows);
    fscanf(fp, "%" SCNu8 "\n", &scale);
    img->rows = rows;
    img->cols = cols;

    struct Color color_arr**;
    *color_arr = malloc(sizeof(struct Color*) * &rows);
    for (int i = 0; i < (int) rows; i++) {
        color_arr[i] = malloc(sizeof(struct Color*) * cols);
    }
   

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            fscanf(fp, "%" SCNu8 "%" SCNu8 "%" SCNu8, &color_arr[i][j].image.R, &color_arr[i][j].image.G, &color_arr[i][j].image.B);
            

        }
        fscanf(fp, "\n");
    }

    /*    
    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            uint8_t *r = (uint8_t*) malloc(sizeof(uint8_t)); 
            uint8_t *g = (uint8_t*) malloc(sizeof(uint8_t));
            uint8_t *b = (uint8_t*) malloc(sizeof(uint8_t));
         
            fscanf(fp, "%"SCNu32 "%"SCNu32 "%"SCNu32, &r, &g, &b);
            *(color->image + i + j) = &color;

            free(r);
            free(g);
            free(b);
        }
    }
    */
    fclose(fp);
    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {
/*
	printf("P3\n");
    printf("%u %u\n", &image->rows, &image->cols);
    printf("255\n");
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            printf("0");
        }
        printf("\n");
    }
    */
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
    free(image);
}
