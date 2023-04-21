#include <stdio.h>
#include <stdlib.h>
#include "gestionFichierImg.h"



void inverse_image(Image img){
    int height = img.dibHeader.height;
    int width = img.dibHeader.width;
    for (int x = 0; x<width; x++){
        for (int y = 0; y<height; y++){
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
    int height = img.dibHeader.height;
    int width = img.dibHeader.width;
    for (int x = 0; x<width/2; x++){
        for (int y = 0; y<height; y++){
            swap(img, x, y, width-x-1, y);
        }
    }
}
void symetrie_x(Image img){
    int height = img.dibHeader.height;
    int width = img.dibHeader.width;
    for (int x = 0; x<width; x++){
        for (int y = 0; y<height/2; y++){
            swap(img, x, y, x, height-y-1);
        }
    }
}

void affiche_image(Image img){
    int height = img.dibHeader.height;
    int width = img.dibHeader.width;
    printf("[\n");
    for (int y = 0; y<height; y++){
        printf("    [ ");
        for (int x = 0; x<width; x++){
            printf("[%3d, %3d, %3d],",getP(&img, y, x, 0),getP(&img, y, x, 1),getP(&img, y, x, 2));
        }
        printf(" ]\n");
    }
    printf("]\n");
}

int rota_90(Image *img){
    Image copy_img = copy(img);
    int height = img->dibHeader.height;
    int width = img->dibHeader.width;
    ClearAndRedimensioner(img,width,height);
    for (int x = 0; x<width; x++){
        for (int y = 0; y<height; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, x, y, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }
    //affiche_image(*img);
    symetrie_y(*img);
    freeImage(&copy_img);
    
}

int redimensionner(Image *img, unsigned int new_h, unsigned int new_w){
    int height = img->dibHeader.height;
    int width = img->dibHeader.width;
    Image copy_img = copy(img);
    ClearAndRedimensioner(img,new_h,new_w);
    for (int x = 0; x<width; x++){
        for (int y = 0; y<height/2; y++){
            float new_x = x*new_w/width;
            float new_y = y*new_h/height;
            int x1 = (int)new_x;
            int y1 = (int)new_y;
            int x2 = x1+1;
            int y2 = y1+1;
            printf("%d, %d\n",new_x,new_y);
            for(int rgb; rgb<3; rgb++){
                int linear1 = (x2-new_x)*getP(&copy_img,y1,x1,rgb) + (new_x - x1) * getP(&copy_img,y1,x2,rgb);
                int linear2 = (x2-new_x)*getP(&copy_img,y2,x1,rgb) + (new_x - x1) * getP(&copy_img,y2,x2,rgb);
                setP(img,x,y,rgb, (y2-new_y) * linear1 + (new_y-y1) * linear2);
            }

        }
    }
    freeImage(&copy_img);
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
    redimensionner(&img, 30, 20);
    //affiche_image(img);

    fichier = NULL;
    fichier = fopen("EcritureImg.bmp", "wb+");
    if(fichier == NULL) {
        exit(0);
    }
    writeFileFromImage(fichier, &img);
    fclose(fichier);
    
    freeImage(&img);
}
