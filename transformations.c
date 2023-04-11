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

void symetrie_y(int columns, int rows, int pixels[rows][columns][3]){
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows; y++){
            for (int rgb = 0; rgb<3; rgb++){
                pixels[y][x][rgb] = 255-pixels[y][x][rgb];
            }
        }
    }
}

void affiche_image(int columns, int rows, int pixels[rows][columns][3]){
    printf("[\n");
    for (int x = 0; x<columns; x++){
        printf("    [ ");
        for (int y = 0; y<rows; y++){
            printf("[%d, %d, %d],",pixels[y][x][0],pixels[y][x][1],pixels[y][x][2]);
        }
        printf(" ]\n");
    }
    printf("]\n");
}

/*int rota_90(int columns, int rows, int pixels[rows][columns][3]){
    int (*pixels2)[rows][3] = malloc(columns * sizeof(*pixels2));
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows; y++){
            for (int rgb = 0; rgb<3; rgb++){
                pixels2[y][x][rgb] = pixels[x][y][rgb];
            }
        }
    }
    affiche_image(rows, columns, pixels2);
    pixels=pixels2;
    free(pixels2);
}*/




void main(void){
    const int columns=2;
    const int rows=2;
    int (*pixels)[columns][3] = malloc(rows * sizeof(*pixels));
    pixels[0][0][0] = 255;
    pixels[0][0][1] = 0;
    pixels[0][0][2] = 0;
    pixels[0][1][0] = 0;
    pixels[0][1][1] = 0;
    pixels[0][1][2] = 0;
    pixels[1][0][0] = 255;
    pixels[1][0][1] = 255;
    pixels[1][0][2] = 0;
    pixels[1][1][0] = 128;
    pixels[1][1][1] = 128;
    pixels[1][1][2] = 128;
affiche_image(columns, rows, pixels);
symetrie_y(columns, rows, pixels);
affiche_image(columns, rows, pixels);
free(pixels);
}
