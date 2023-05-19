#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gestionFichierImg.h"



void inverse_image(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, y, x, rgb, 255-getP(img, y, x, rgb));
            }
        }
    }
}

void swap(Image *img, int x1, int y1, int x2, int y2){
    for (int rgb = 0; rgb<3; rgb++){
        unsigned char temp = getP(img, y1, x1, rgb);
        setP(img, y1, x1, rgb, getP(img, y2, x2, rgb));
        setP(img, y2, x2, rgb, temp);
    }
}

void symetrie_y(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = 0; x<max_x/2; x++){
        for (int y = 0; y<max_y; y++){
            swap(img, x, y, max_x-x-1, y);
        }
    }
}

void symetrie_x(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y/2; y++){
            swap(img, x, y, x, max_y-y-1);
        }
    }
}

void affiche_image(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    printf("[\n");
    for (int y = 0; y<max_y; y++){
        printf("    [ ");
        for (int x = 0; x<max_x; x++){
            printf("[%3d, %3d, %3d],",getP(img, y, x, 0),getP(img, y, x, 1),getP(img, y, x, 2));
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
    symetrie_y(img);
    freeImage(&copy_img);
    
}

int flou(Image *img, int force){
    if (force%2 != 1 || force<0){
        printf("1\n");
        exit(1);
    }
    printf("start\n");
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            int min_iy=force/-2, max_iy=force+min_iy, min_ix=min_iy, max_ix=max_iy;
            
            if (x + min_ix < 0){
                min_ix-=(x + min_ix);
            }
            else if(x + max_ix > max_x){
                max_ix-=(x + max_x);
            }
            if (y + min_iy < 0){
                min_iy-=(x + min_iy);
            }
            else if(y + max_iy > max_y){
                max_iy-=(x + max_y);
            }
            for (int rgb = 0; rgb<3; rgb++){
                float valeur = 0;
                for(int ix = min_ix; ix < max_ix; ix++){
                    for(int iy = min_iy; iy < max_iy; iy++){
                        valeur += getP(&copy_img,y+iy,x+ix,rgb);
                        
                    }
                }
                valeur /= force*force;
                setP(img, y, x, rgb, (int)valeur);
                //printf("%d ", (int)valeur);
            }
            //printf("%d %d\n", x, y);
        }
    }
    freeImage(&copy_img);
}

/*
int rogner(Image *img, int new_x, int new_y){
    if (new_x > img->dibHeader.width || new_y > img->dibHeader.height){
        exit(1);
    }
    Image copy_img = copy(img);
    ClearAndRedimensioner(img,new_y,new_x);
    for (int x = 0; x<new_x; x++){
        for (int y = 0; y<new_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, y, x, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }

}
*/

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
            float dx = x/facteur_x;
            float dy = y/facteur_y;
            int new_x = (int)dx;
            int new_y = (int)dy;
            for (int rgb = 0; rgb<3; rgb++){
                int delta_x = getP(&copy_img,new_y,new_x+1,rgb)-getP(&copy_img,new_y,new_x,rgb);
                int delta_y = getP(&copy_img,new_y+1,new_x,rgb)-getP(&copy_img,new_y,new_x,rgb);
                int delta_xy = getP(&copy_img,new_y,new_x,rgb)+getP(&copy_img,new_y+1,new_x,rgb+1)-getP(&copy_img,new_y,new_x+1,rgb)-getP(&copy_img,new_y+1,new_x,rgb);
                int valeur = delta_x * dx + delta_y * dy + delta_xy * dx * dy + getP(&copy_img,new_y,new_x,rgb);
                setP(img,y,x,rgb,valeur);

                //setP(img, y, x, rgb, getP(&copy_img, (int)(y/facteur_y), (int)(x/facteur_x), rgb));
            }
            printf("(%d,%d) ",(int)(x/facteur_x),(int)(y/facteur_y));
        }
    }
    freeImage(&copy_img);
}


void main(void){
    FILE* fichier = NULL;
    fichier = fopen("./Images/couleurTriangle.bmp", "rb+");
    if(fichier == NULL) {
        printf("0\n");
        exit(0);
    }                                                                                                            
    Image img = getImageFromFile(fichier);                                                                      
    fclose(fichier);

    //affiche_image(&img);
    flou(&img,3);
    //affiche_image(&img);
    printf("mid\n");
    fichier = NULL;
    fichier = fopen("./EcritureImg.bmp", "wb+");
    if(fichier == NULL) {
        printf("1\n");
        exit(0);
    }
    writeFileFromImage(fichier, &img);
    fclose(fichier);
    freeImage(&img);
    printf("end\n");
}
