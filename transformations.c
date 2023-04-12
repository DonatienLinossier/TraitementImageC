#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



void inverse_image(int columns, int rows, int pixels[rows][columns][3]){
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows; y++){
            for (int rgb = 0; rgb<3; rgb++){
                pixels[y][x][rgb] = 255-pixels[y][x][rgb];
            }
        }
    }
}
void swap(int columns, int rows, int tab[rows][columns][3], int x1, int y1, int x2, int y2){
    for (int rgb = 0; rgb<3; rgb++){
        int temp = tab[y1][x1][rgb];
        tab[y1][x1][rgb] = tab[y2][x2][rgb];
        tab[y2][x2][rgb] = temp;
    }
}

void symetrie_y(int columns, int rows, int pixels[rows][columns][3]){
    for (int x = 0; x<columns/2; x++){
        for (int y = 0; y<rows; y++){
            swap(columns, rows, pixels, x, y, columns-x-1, y);
        }
    }
}
void symetrie_x(int columns, int rows, int pixels[rows][columns][3]){
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows/2; y++){
            swap(columns, rows, pixels, x, y, x, rows-y-1);
        }
    }
}

void affiche_image(int columns, int rows, int pixels[rows][columns][3]){
    printf("[\n");
    for (int y = 0; y<rows; y++){
        printf("    [ ");
        for (int x = 0; x<columns; x++){
            printf("[%3d, %3d, %3d],",pixels[y][x][0],pixels[y][x][1],pixels[y][x][2]);
        }
        printf(" ]\n");
    }
    printf("]\n");
}

int rota_90(int columns, int rows, int pixels[rows][columns][3]){
    int (*pixels2)[rows][3] = malloc(columns * sizeof(*pixels2));
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows; y++){
            for (int rgb = 0; rgb<3; rgb++){
                pixels2[y][x][rgb] = pixels[x][y][rgb];
            }
        }
    }
    symetrie_y(rows, columns, pixels2);
    affiche_image(rows, columns, pixels2);
    
}




void main(void){
    const int columns=2;
    const int rows=3;
    int (*pixels)[columns][3] = malloc(rows * sizeof(*pixels));
    pixels[0][0][0] = 0; pixels[0][0][1] = 1; pixels[0][0][2] = 2;
    pixels[0][1][0] = 10; pixels[0][1][1] = 11; pixels[0][1][2] = 12;
    pixels[1][0][0] = 100; pixels[1][0][1] = 101; pixels[1][0][2] = 102;
    pixels[1][1][0] = 110; pixels[1][1][1] = 111; pixels[1][1][2] = 112;
    pixels[2][0][0] = 200; pixels[2][0][1] = 201; pixels[2][0][2] = 202;
    pixels[2][1][0] = 210; pixels[2][1][1] = 211; pixels[2][1][2] = 212;
affiche_image(columns, rows, pixels);
rota_90(columns, rows, pixels);
affiche_image(columns, rows, pixels);
free(pixels);
}
