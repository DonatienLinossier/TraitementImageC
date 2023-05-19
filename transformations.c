#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void noir_et_blanc(int width, int height, int pixels[height][width][3]){
    int somme=0, rgb, m;
     for (int x = 0; x<width; x++){
        for (int y = 0; y<height; y++){
            for (int rgb = 0; rgb<3; rgb++){
                somme += pixels[0][0][rgb];
             }
                m=somme/3;
                for (int rgb = 0; rgb<3; rgb++){
                    pixels[y][x][rgb]=m;
                }
        }
    } 
}


void inverse_image(int columns, int rows, int pixels[rows][columns][3]){
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
    for (int y = 0; y<max_y; y++){
        printf("    [ ");
        for (int y = 0; y<rows; y++){
            printf("[%d, %d, %d],",pixels[y][x][0],pixels[y][x][1],pixels[y][x][2]);
        }
        printf(" ]\n");
    }
    printf("]\n");
}




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
inverse_image(columns, rows, pixels);
affiche_image(columns, rows, pixels);
free(pixels);
}
