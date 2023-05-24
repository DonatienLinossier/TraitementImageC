#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "transformations.h"
#include "gestionFichierImg.h"

//Fonction qui passe l'image en noir et blanc
void grayscale(Image *img, int x1, int y1, int x2, int y2){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    if(x1<0 || y1<0 || x2>=max_x || y2>=max_y || x1>=x2 || y1>=y2){
        exit(1);
       
    }

    int sum,average;
    for (int x = x1; x<x2; x++){
        for (int y = y1; y<y2; y++){
            sum=0;
            for (int rgb = 0; rgb<3; rgb++){
                sum += getP(img, y, x, rgb);
             }
            average=(int)(sum/3.0);
            //On fait la moyenne des 3 valeurs rgb puis définit ces 3 valeurs à cette moyenne
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, y, x, rgb, average);
            }
        }
    } 
}

void binary(Image *img, int x1, int y1, int x2, int y2){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    if(x1<0 || y1<0 || x2>=max_x || y2>=max_y || x1>=x2 || y1>=y2){
        exit(1);
       
    }

    int value;
    grayscale(img,x1,y1,x2,y2);
    for (int x = x1; x<x2; x++){
        for (int y = y1; y<y2; y++){
            if (getP(img,y,x,0)<128){
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

//Fonction qui inverse les couleurs de l'image
void reverse_image(Image *img, int x1, int y1, int x2, int y2){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    if(x1<0 || y1<0 || x2>=max_x || y2>=max_y || x1>=x2 || y1>=y2){
        exit(1);
       
    }

    for (int x = x1; x<x2; x++){
        for (int y = x1; y<x2; y++){
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
    if(x1<0 || y1<0 || x2>=img->dibHeader.width || y2>=img->dibHeader.height){
        exit(1);
    }

    for (int rgb = 0; rgb<3; rgb++){
        unsigned char temp = getP(img, y1, x1, rgb);
        //On crée une variable temporaire pour échanger les deux pixels : A->C, B->A, C->B
        setP(img, y1, x1, rgb, getP(img, y2, x2, rgb));
        setP(img, y2, x2, rgb, temp);
    }
}

//Fonction qui fait une symetrie de l'image selon l'axe y
void symmetry_y(Image *img, int x1, int y1, int x2, int y2){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    if(x1<0 || y1<0 || x2>=max_x || y2>=max_y || x1>=x2 || y1>=y2){
        exit(1);
       
    }


    for (int x = x1; x<x2/2; x++){
        for (int y = y1; y<y2; y++){
            //On échange chaque pixel avec le pixel avec le même y et le x = 255-x
            swap(img, x, y, x2-x-1, y);
        }
    }
}

//Fonctiongi qui fait une symetrie de l'image selon l'axe x
void symmetry_x(Image *img, int x1, int y1, int x2, int y2){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = x1; x<x2; x++){
        for (int y = y1; y<y2/2; y++){
            //On échange chaque pixel avec le pixel avec le même x et le x inverse
            swap(img, x, y, x, y2-y-1);
        }
    }
}

//Fonction qui affiche les valeurs rgb de l'image dans le terminal
void display_image(Image *img){
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
    //symmetry_y(img,);
    freeImage(&copy_img);
    
}

//Fonction qui floute l'image selon un facteur
void blur(Image *img, int range, int x1, int y1, int x2, int y2){
    if (range%2 != 1 || range<0){
        printf("1\n");
        exit(1);
    }
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;

    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            if(x1<=x && x<=x2 && y1<=y && y<=y2){
                //On crée les coordonnées des coins de la matrice flou qu'on applique à l'image
                int min_iy=range/-2, max_iy=range+min_iy, min_ix=min_iy, max_ix=max_iy;
                // On rectifie ces coordonnées pour éviter les valeurs hors de l'image
                if (x + min_ix < 0){
                    min_ix -= (x + min_ix);
                }
                else if(x + max_ix > max_x){
                    max_ix = max_x - x;
                }
                if (y + min_iy < 0){
                    min_iy -= (y + min_iy);
                }
                else if(y + max_iy > max_y){
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

//Fonctions qui redimensionne l'image selon un facteur
void resize(Image *img, int new_x, int new_y){
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    float facteur_x = (float)new_x / (float)max_x;
    float facteur_y = (float)new_y / (float)max_y;
    int x_floor, x_ceil, y_floor, y_ceil;
    float old_x, old_y, dx, dy;
    float weights[4], val[4];
    clearAndResize(img,new_y,new_x);
    for (int x = 0; x<new_x; x++){
        for (int y = 0; y<new_y; y++){
            old_x = x / facteur_x;
            old_y = y / facteur_y;
            x_floor = (int)floor(old_x);
            x_ceil = (int)ceil(old_x);
            y_floor = (int)floor(old_y);
            y_ceil = (int)ceil(old_y);
            dx = old_x - x_floor;
            dy = old_y - y_floor;
            weights[0] = (1 - dx) * (1 - dy);
            weights[1] = dx * (1 - dy);
            weights[2] = (1 - dx) * dy;
            weights[3] = dx * dy;
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
                setP(img,y,x,rgb,(int)(val[0] + val[1] + val[2] + val[3]));
            }
        }
    }
    freeImage(&copy_img);
}

/*
void main(void){
    printf("start\n");
    FILE* file = NULL;
    file = fopen("./Images/couleur.bmp", "rb+");
    if(file == NULL) {
        printf("0\n");
        exit(0);
    }                                                                                                            
    Image img = getImageFromFile(file);                                                                     
    fclose(file);

    blur(&img,11,50,50,150,150);
    printf("mid\n");

    file = NULL;
    file = fopen("./EcritureImg.bmp", "wb+");
    printf("A\n");
    if(file == NULL) {
        printf("1\n");
        exit(0);
    }
    writeFileFromImage(file, &img);
    printf("B\n");
    fclose(file);
    printf("C\n");
    freeImage(&img);
    printf("end\n");
}
ù/