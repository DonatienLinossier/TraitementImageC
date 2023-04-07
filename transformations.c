#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "image.c"

#define columns 200
#define rows 200



void display_grid(int grid[rows][columns][3]){
    printf("{");
    for (int y=0; y<rows; y++){
        printf("[");
        for (int x=0; x<columns; x++){
            printf("(%d,%d,%d), ",grid[y][x][0],grid[y][x][1],grid[y][x][2]);
        }
        printf("]\n\n");
    }
    printf("}");
}



void main(void){
    printf("1");
    long int len_tab = sizeof(image_map)/sizeof(image_map[0]);
    printf("%d", len_tab);
    int grid[rows][columns][3];
    for (int y=0; y < rows; y++){
        for (int x=0; x< columns; x++){
            //printf("%d %d / ", y, x);
            for (int i=0; i<3; i++){
                grid[y][x][i] = image_map[y*rows + x + i];
            }
        }
    }
    printf("2");
    display_grid (grid[rows][columns][3]);
    printf("3");
    return 0;
}
