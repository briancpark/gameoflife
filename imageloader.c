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
        printf("File doesn't exist. Exit with code -1");
        exit(-1);
    }

    Image *img;
    img = (Image*) malloc(sizeof(struct Image));  
    
    if (img == NULL) {
        fclose(fp);
        exit(-1);
    }

    char ft[2];

    fscanf(fp, "%s", ft);
    fscanf(fp, "%u %u", &img->cols, &img->rows);
    fscanf(fp, "%*d");

    Color **color_arr = (Color**) malloc(sizeof(Color*) * img->rows * img->cols);
    if (color_arr == NULL) {
        free(img);
        printf("Memory allocation failed.");
        exit(-1);
    }

    for (uint32_t i = 0; i < img->rows; i++) {
        for (uint32_t j = 0; j < img->cols; j++) {
            Color *color = malloc(sizeof(Color));
            if (color == NULL) {
                free(img);
                free(color_arr);
                printf("Memory allocation failed.");
                exit(-1);
            }
            fscanf(fp, "%hhu %hhu %hhu", &color->R, &color->G, &color->B);
            *(color_arr + i * img->cols + j) = color;
        }
    }

    fclose(fp);
    img->image = color_arr;
    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {
    printf("P3\n");
    printf("%hhu %hhu\n", image->cols, image->rows);
    printf("255\n");

    Color **img = image->image;

    for (uint32_t i = 0; i < image->rows; i++) {
        for (uint32_t j = 0; j < image->cols; j++) {
            Color *color = *(img + i * image->cols + j);
            printf("%3hhu %3hhu %3hhu", color->R, color->G, color->B);
            free(color);
            if (j < image->cols - 1) {
                printf("   ");
            }
        }
        printf("\n");
    }    
}

//Frees an image
void freeImage(Image *image) {
    free(image->image);
    free(image);   
}
