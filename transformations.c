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

int rota_90(Image *img){
    Image copy_img = copy(img);
    int rows = img->dibHeader.height;
    int columns = img->dibHeader.width;
    ClearAndRedimensioner(img,columns,rows);
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, x, y, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }
    //affiche_image(*img);
    symetrie_y(*img);
    freeImage(&copy_img);
    
}

int redimensionner(Image *img, unsigned float facteur){
    Image copy_img = copy(img);
    int rows = int(img.dibHeader.height * facteur);
    int columns = int(img.dibHeader.width * facteur);
    ClearAndRedimensioner(img,rows,columns);
    for (int x = 0; x<columns; x++){
        for (int y = 0; y<rows/2; y++){
            
        }
    }
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
    rota_90(&img);
    affiche_image(img);
    freeImage(&img);
    printf("img");
}
