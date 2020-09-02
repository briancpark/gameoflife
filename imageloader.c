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
    uint32_t cols;
    uint32_t rows;
   
    fscanf(fp, "%s", ft);
    fscanf(fp, "%u %u", &cols, &rows);
    fscanf(fp, "%*d");

    img->rows = rows;
    img->cols = cols;

    int size_r = (int) rows;
    int size_c = (int) cols;

    Color** color_arr;  
    color_arr = (Color **) malloc(sizeof(Color*) * size_r);  
    for (int i = 0; i < size_r; i++) {
       color_arr[i] = (Color*) malloc(sizeof(double) * size_c);  
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
    
    uint32_t rows = (uint32_t) image->rows;
    uint32_t cols = (uint32_t) image->cols;

    printf("P3\n");
    printf("%hhu %hhu\n", image->rows, image->cols);
    printf("255\n");

    for (uint32_t i = 0; i < rows; i++) {
        for (uint32_t j = 0; j < cols; j++) {
            printf("%3hhu %3hhu %3hhu", (uint8_t) image->image[i][j].R, (uint8_t) image->image[i][j].G, (uint8_t) image->image[i][j].B);
            
            if (j < image->cols - 1) {
                printf("   ");
            }
        }
        printf("\n");
    }    
}

//Frees an image
void freeImage(Image *image) {
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}

    free(image->image); 
    free(image);   
}
