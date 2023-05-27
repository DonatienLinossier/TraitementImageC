//Ce fichier contient les différentes fonctions qui modifient l'image
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "transformations.h"
#include "imageManagement.h"


/*
Ces quelques lignes presentes au début de chaque fonction servent à les sécuriser : vérifier que l'image existe et que les valeurs sont valides : 
    if(img == NULL){
        printf("function : image null\n");
        exit(1);
        }
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>=max_x || min_y>=max_y){
        printf("function : valeurs invalides\n");
        return;
    }
*/


//Fonction qui passe l'image en noir et blanc selon la sélection
void grayscale(Image *img, int sel[4]){
    if(img == NULL){
        printf("grayscale : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>=max_x || min_y>=max_y){
        printf("grayscale : valeurs invalides\n");
        return;
    }

    int sum,average;
    for (int x = min_x; x<max_x; x++){
        for (int y = min_y; y<max_y; y++){
            sum=0;
            for (int rgb = 0; rgb<3; rgb++){
                sum += getP(img, y, x, rgb);
             }
             //On fait la moyenne des 3 valeurs rgb puis définit ces 3 valeurs à cette moyenne
            average=(int)(sum/3.0); 
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, y, x, rgb, average);
            }
        }
    } 
}

//Fonction qui augmente ou baisse la luminosité dans la sélection
void brightness(Image *img, float percentage, int sel[4]){
    if(img == NULL){
        printf("brightness : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>max_x || min_y>=max_y ){
        printf("brightness : valeurs invalides\n");
        return;
    }
    int new_rgb;
    percentage /= 100;
    for (int x = min_x; x<max_x; x++){
        for (int y = min_y; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                //On multiplie chaque composantes rgb par le facteur de luminosité choisi
                new_rgb = (int)getP(img,y,x,rgb)*percentage;
                // On s'assure que la valeur ne dépasse pas 255
                if (new_rgb > 255){
                    new_rgb = 255; 
                }
                setP(img, y, x, rgb, new_rgb);
             }
        }   
    }
}

//Fonction qui rend les pixels lumineux encore plus lumineux et ceux sombres encore plus sombre
int contrast_recursive(int value, int power){
    if(value <= 255 / 2){
        return (int)( (255/2) * pow(2 * value / 255, power));
    }
    else{
        return 255 - contrast_recursive(255 - value, power);
    }

}

//Fonction qui augmente le contraste de l'image dans la sélection
void contrast(Image *img, int sel[4]){
    if(img == NULL){
        printf("contrast : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>max_x || min_y>=max_y){
        printf("contrast : valeurs invalides\n");
        return;
    }
    int power = 3,value;
    for (int x = min_x; x<max_x; x++){
        for (int y = min_y; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                value = contrast_recursive(getP(img,y,x,rgb),power);
                setP(img,y,x,rgb,value);
            }
        }
    }        
}

//Fonction qui rends chaque pixel dans la sélection noir ou blanc dans la sélection
void binary(Image *img, int sel[4]){
    if(img == NULL){
        printf("binary : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>max_x || min_y>max_y){
        printf("binary : valeurs invalides\n");
        return;
    }

    int value;
    grayscale(img,sel); //On passe d'abord l'image en noir et blanc
    for (int x = min_x; x<max_x; x++){
        for (int y = min_y; y<max_y; y++){
            if (getP(img,y,x,0)<128){ // On arrondit à 0 ou 255
                value = 0;
            }
            else {
                value = 255;
            }
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, y, x, rgb, value);
            }
        }
    }
}

//Fonction qui inverse les couleurs de l'image dans la sélection
void reverse_colors(Image *img, int sel[4]){
    if(img == NULL){
        printf("reverse_image : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>=max_x || min_y>=max_y){
        printf("reverse_image : valeurs invalides\n");
        return;
    }

    for (int x = min_x; x<max_x; x++){
        for (int y = min_y; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                //On définit chaque valeur rgb de chaque pixel à son inverse, soit 255 - sa valeur
                setP(img, y, x, rgb, 255-getP(img, y, x, rgb));
            }
        }
    }
}

//Fonction qui échange 2 pixels de l'image
void swap(Image *img, int x1, int y1, int x2, int y2){
    //x1 et y1 sont les coordonnées du premier pixel, x2 et y2 sont les coordonnées du deuxième pixel
    if(img == NULL){
        printf("swap : image null\n");
        exit(1);
    }
    if(x1<0 || y1<0 || x2>img->dibHeader.width || y2>img->dibHeader.height){
        printf("swap : valeurs invalides\n");
        return;
    }

    for (int rgb = 0; rgb<3; rgb++){
        unsigned char temp = getP(img, y1, x1, rgb);
        //On crée une variable temporaire pour échanger les deux pixels : A->C, B->A, C->B
        setP(img, y1, x1, rgb, getP(img, y2, x2, rgb));
        setP(img, y2, x2, rgb, temp);
    }
}

//Fonction qui fait une symetrie de l'image selon l'axe y dans la sélection
void symmetry_y(Image *img, int sel[4]){
    if(img == NULL){
        printf("symmetry_y : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>max_x || min_y>max_y){
        printf("symmetry_y : valeurs invalides\n");
        return;
    }


    for (int x = min_x; x<max_x/2; x++){
        for (int y = min_y; y<max_y; y++){
            //On échange chaque pixel avec le pixel avec le même y et le x = 255-x
            swap(img, x, y, max_x-x-1, y);
        }
    }
}

//Fonctiongi qui fait une symetrie de l'image selon l'axe x dans la sélection
void symmetry_x(Image *img, int sel[4]){
    if(img == NULL){
        printf("symmetry_x : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>=max_x || min_y>=max_y){
        printf("symmetry_x : valeurs invalides\n");
        return;
    }
    for (int x = min_x; x<max_x; x++){
        for (int y = min_y; y<max_y/2; y++){
            //On échange chaque pixel avec le pixel avec le même x et le x inverse
            swap(img, x, y, x, max_y-y-1);
        }
    }
}

//Fonction qui affiche les valeurs rgb de l'image dans le terminal
void display_image(Image *img){
    if(img == NULL){
        printf("display_image : image null\n");
        exit(1);
    }
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

//Fonction qui effectue une rotation de l'image de 90 degrés dans le sens horaire
void rotate_90(Image *img){
    if(img == NULL){
        printf("rotate_90 : image null\n");
        exit(1);
    }
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    clearAndResize(img,max_x,max_y);
    //On inverse l'image selon l'axe diagonal en échangeant les pixels (x,y) avec les pixels (y,x)
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, x, y, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }
    //On effecture une symetrie selon l'axe y, ce qui revient enfin à une rotation
    int sel[] = {0,0,max_y,max_x};
    symmetry_y(img,sel);
    freeImage(&copy_img);
    
}

//Fonction qui floute l'image selon un facteur dans la sélection
void blur(Image *img, int range, int sel[4]){
    if(img == NULL){
        printf("blur : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(range%2 != 1 || range<0 || min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>max_x || min_y>max_y){
        printf("blur : valeurs invalides\n");
        return;
    }
    Image copy_img = copy(img);

    for (int x = 0; x<img->dibHeader.width; x++){
        for (int y = 0; y<img->dibHeader.height; y++){
            if(min_x<=x && x<=max_x && min_x<=y && y<=max_x){
                //On crée les coordonnées des coins de la matrice flou qu'on applique à l'image
                int min_iy=range/-2, max_iy=range+min_iy, min_ix=min_iy, max_ix=max_iy;
                // On rectifie ces coordonnées pour éviter les valeurs hors de l'image
                if (x + min_ix < 0){
                    min_ix -= (x + min_ix);
                }
                else if(x + max_ix > img->dibHeader.width){
                    max_ix = max_x - x;
                }
                if (y + min_iy < 0){
                    min_iy -= (y + min_iy);
                }
                else if(y + max_iy > img->dibHeader.height){
                    max_iy = max_y - y;
                }
                for (int rgb = 0; rgb<3; rgb++){
                    float sum = 0;
                    for(int ix = min_ix; ix < max_ix; ix++){
                        for(int iy = min_iy; iy < max_iy; iy++){
                            //On ajoute les valeurs de tous les pixels autour
                            sum += getP(&copy_img,y+iy,x+ix,rgb);
                        }
                    }
                    //On divise cette somme par le nombre de cases de la matrice
                    sum /= range*range;
                    setP(img, y, x, rgb, (int)sum);
                }
            }
            else{
                for (int rgb = 0; rgb<3; rgb++){
                    setP(img, y, x, rgb, getP(img, y, x, rgb));
                }
            }
        }
    }
    freeImage(&copy_img);
}

//Fonctions qui rogne l'image selon la sélection
void crop(Image *img, int sel[4]) {
    if(img == NULL){
        printf("crop : image null\n");
        exit(1);
    }
    int min_y = sel[1];
    int min_x = sel[0];
    int max_y = sel[3];
    int max_x = sel[2];
    if(min_x<0 || min_y<0 || max_x>img->dibHeader.width || max_y>img->dibHeader.height|| min_x>=max_x || min_y>=max_y){
        printf("crop : valeurs invalides\n");
        return;
    }
    //On crée une copie de l'image puis une image vide de la nouvelle taille
    Image copy_img = copy(img);
    clearAndResize(img, max_y-min_y, max_x-min_x);
    for(int x = min_x; x<max_x; x++) {
        for(int y = min_y; y<max_y; y++) {
            for(int rgb =0; rgb<3; rgb++) {
                //
                setP(img, y-min_x, x-min_x, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }

    freeImage(&copy_img);
}

//Fonction qui redimensionne l'image selon un facteur
void resize(Image *img, int new_x, int new_y){
    if(img == NULL){
        printf("resize : image null\n");
        exit(1);
    }
    if(new_x<=0 || new_y<=0){
        printf("resize : valeurs invalides\n");
        return;
    }
    //Définition des variables
    int x_floor, x_ceil, y_floor, y_ceil;
    float old_x, old_y, dx, dy;
    float weights[4], val[4];

    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    //Les facteurs x et y par lesquels on multiple l'ancienne taille pour avoir la nouvelle
    float factor_x = (float)new_x / (float)max_x;
    float factor_y = (float)new_y / (float)max_y;
    
    Image copy_img = copy(img);
    clearAndResize(img,new_y,new_x);
    for (int x = 0; x<new_x; x++){
        for (int y = 0; y<new_y; y++){
            //Le point old correspond au point actuel sur l'ancienne image
            old_x = x / factor_x;
            old_y = y / factor_y;
            //On arrondit ces coordonnées aux entiers inférieurs et supérieurs
            x_floor = (int)floor(old_x);
            x_ceil = (int)ceil(old_x);
            y_floor = (int)floor(old_y);
            y_ceil = (int)ceil(old_y);
            dx = old_x - x_floor;
            dy = old_y - y_floor;
            //Le tableau weights contient les poids selon lesquels les 4 pixels sont pris en compte
            weights[0] = (1 - dx) * (1 - dy);
            weights[1] = dx * (1 - dy);
            weights[2] = (1 - dx) * dy;
            weights[3] = dx * dy;
            //On s'assure de ne pas dépasser la taille de l'image
            if(x_ceil==max_x){
                x_ceil-=1;
            }
            if(y_ceil==max_y){
                y_ceil-=1;
            }
            for (int rgb = 0; rgb<3; rgb++){
                val[0] = getP(&copy_img, y_floor, x_floor,rgb) * weights[0];
                val[1] = getP(&copy_img, y_floor, x_ceil, rgb) * weights[1];
                val[2] = getP(&copy_img, y_ceil, x_floor, rgb) * weights[2];
                val[3] = getP(&copy_img, y_ceil, x_ceil, rgb) * weights[3];
                //On ajoute les 4 valeurs selon leurs poids respectifs
                setP(img,y,x,rgb,(int)(val[0] + val[1] + val[2] + val[3]));
            }
        }
    }
    freeImage(&copy_img);
}

