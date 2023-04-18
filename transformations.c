#include <stdio.h>
#include <stdlib.h>
#include "gestionFichierImg.h"



void inverse_image(Image img){
    int rows = img.dibHeader.height;
    int columns = img.dibHeader.width;
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(&img, y, x, rgb, 255-getP(&img, y, x, rgb));
            }
        }
    }
}
void swap(Image img, int x1, int y1, int x2, int y2){
    for (int rgb = 0; rgb<3; rgb++){
        unsigned char temp = getP(&img, y1, x1, rgb);
        setP(&img, y1, x1, rgb, getP(&img, y2, x2, rgb));
        setP(&img, y2, x2, rgb, temp);
    }
}

void symetrie_y(Image img){
    int rows = img.dibHeader.height;
    int columns = img.dibHeader.width;
    for (int x = 0; x<columns/2; x++){
        for (int y = 0; y<rows; y++){
            swap(img, x, y, columns-x-1, y);
        }
    }
}
void symetrie_x(Image img){
    int rows = img.dibHeader.height;
    int columns = img.dibHeader.width;
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows/2; y++){
            swap(img, x, y, x, rows-y-1);
        }
    }
}

void affiche_image(Image img){
    int rows = img.dibHeader.height;
    int columns = img.dibHeader.width;
    printf("[\n");
    for (int y = 0; y<rows; y++){
        printf("    [ ");
        for (int x = 0; x<columns; x++){
            printf("[%3d, %3d, %3d],",getP(&img, y, x, 0),getP(&img, y, x, 1),getP(&img, y, x, 2));
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
    //symetrie_y(rows, columns, pixels2);
    //affiche_image(rows, columns, pixels2);
    
}




void main(void){
    FILE* fichier = NULL;
    fichier = fopen("./test.bmp", "rb+");
    if(fichier == NULL) {
        exit(0);
    }                                                                                                            
    Image img = getImageFromFile(fichier);                                                                      
    fclose(fichier);                                       

affiche_image(img);
inverse_image(img);
affiche_image(img);
freeImage(&img);
}
