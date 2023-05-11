#include <stdio.h>
#include <stdlib.h>
#include "gestionFichierImg.h"



void inverse_image(Image img){
    int max_y = img.dibHeader.height;
    int max_x = img.dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
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
    int max_y = img.dibHeader.height;
    int max_x = img.dibHeader.width;
    for (int x = 0; x<max_x/2; x++){
        for (int y = 0; y<max_y; y++){
            swap(img, x, y, max_x-x-1, y);
        }
    }
}
void symetrie_x(Image img){
    int max_y = img.dibHeader.height;
    int max_x = img.dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y/2; y++){
            swap(img, x, y, x, max_y-y-1);
        }
    }
}

void affiche_image(Image img){
    int max_y = img.dibHeader.height;
    int max_x = img.dibHeader.width;
    printf("[\n");
    for (int y = 0; y<max_y; y++){
        printf("    [ ");
        for (int x = 0; x<max_x; x++){
            printf("[%3d, %3d, %3d],",getP(&img, y, x, 0),getP(&img, y, x, 1),getP(&img, y, x, 2));
        }
        printf(" ]\n");
    }
    printf("]\n");
}

int rota_90(Image *img){
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    ClearAndRedimensioner(img,max_x,max_y);
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, x, y, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }
    //affiche_image(*img);
    symetrie_y(*img);
    freeImage(&copy_img);
    
}

int redimensionner(Image *img, float facteur){
    Image copy_img = copy(img);
    int max_x = (int)img->dibHeader.width * facteur;
    int max_y = (int)img->dibHeader.height * facteur;
    float facteur_x = max_x/img->dibHeader.width;
    float facteur_y = max_y/img->dibHeader.height;
    printf("%d %d",max_x,max_y);
    ClearAndRedimensioner(img,max_y,max_x);
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, x, y, rgb, getP(&copy_img, (int)(x/facteur_x), (int)(y/facteur_y), rgb));
            }
            printf("(%d,%d) ",(int)(x/facteur_x),(int)(y/facteur_y));
        }
    }
}


void main(void){
    FILE* fichier = NULL;
    fichier = fopen("./test.bmp", "rb+");
    if(fichier == NULL) {
        printf("0");
        exit(0);
    }                                                                                                            
    Image img = getImageFromFile(fichier);                                                                      
    fclose(fichier);

    //affiche_image(img);
    redimensionner(&img,2);
    //affiche_image(img);

    fichier = NULL;
    fichier = fopen("./EcritureImg.bmp", "wb+");
    if(fichier == NULL) {
        printf("1");
        exit(0);
    }
    writeFileFromImage(fichier, &img);
    fclose(fichier);
    freeImage(&img);
    printf("bien");
}
