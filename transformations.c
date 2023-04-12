#include <stdio.h>
#include <stdlib.h>
#include <gestionFichierImg.h>



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

void affiche_image(Image img){
    rows = img.dibHeader.
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
    struct Image img;
    img.dibHeader.width = 2;
    img.dibHeader.height = 3;
    setP(img,0,0,0, 0);setP(img,0,0,1, 1);setP(img,0,0,2, 2);
    setP(img,0,1,0, 10);setP(img,0,1,1, 11);setP(img,0,1,2, 12);
    setP(img,1,0,0, 100);setP(img,1,0,1, 101);setP(img,1,0,2, 102);
    setP(img,1,1,0, 110);setP(img,1,1,1, 111);setP(img,1,1,2, 112);
    setP(img,2,0,0, 200);setP(img,2,0,1, 201);setP(img,2,0,2, 202);
    setP(img,2,1,0, 210);setP(img,2,1,1, 211);setP(img,2,1,2, 212);
affiche_image(img);
//symetrie_x(columns, rows, pixels);
//affiche_image(columns, rows, pixels);
//free(pixels);
}
