#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gestionFichierImg.h"

//Fonction qui passe l'image en noir et blanc
void grayscale(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    int sum,average;

     for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
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

//Fonction qui inverse les couleurs de l'image
void reverse_image(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;

    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
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
void symmetry_y(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;

    for (int x = 0; x<max_x/2; x++){
        for (int y = 0; y<max_y; y++){
            //On échange chaque pixel avec le pixel avec le même y et le x = 255-x
            swap(img, x, y, max_x-x-1, y);
        }
    }
}

//Fonction qui fait une symetrie de l'image selon l'axe x
void symmetry_x(Image *img){
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y/2; y++){
            //On échange chaque pixel avec le pixel avec le même x et le x inverse
            swap(img, x, y, x, max_y-y-1);
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
int rotate_90(Image *img){
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    clearAndResize(img,max_x,max_y);
    //On inverse l'image selon l'axe diagonal en inversant les pixels (x,y)
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            for (int rgb = 0; rgb<3; rgb++){
                setP(img, x, y, rgb, getP(&copy_img, y, x, rgb));
            }
        }
    }
    //On effecture une symetrie selon l'axe y, ce qui revient enfin à une rotation
    symmetry_y(img);
    freeImage(&copy_img);
    
}

//Fonction qui floute l'image selon un facteur
int blur(Image *img, int factor){
    //Ne marche pas encore
    if (factor%2 != 1 || factor<0){
        printf("1\n");
        exit(1);
    }
    printf("start\n");
    Image copy_img = copy(img);
    int max_y = img->dibHeader.height;
    int max_x = img->dibHeader.width;
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            int min_iy=factor/-2, max_iy=factor+min_iy, min_ix=min_iy, max_ix=max_iy;
            
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
                float value = 0;
                for(int ix = min_ix; ix < max_ix; ix++){
                    for(int iy = min_iy; iy < max_iy; iy++){
                        value += getP(&copy_img,y+iy,x+ix,rgb);
                        
                    }
                }
                value /= factor*factor;
                setP(img, y, x, rgb, (int)value);
                //printf("%d ", (int)value);
            }
            //printf("%d %d\n", x, y);
        }
    }
    freeImage(&copy_img);
}

//Fonctions qui redimensionne l'image selon un facteur
int resize(Image *img, float factor){
    Image copy_img = copy(img);
    int max_x = (int)img->dibHeader.width * factor;
    int max_y = (int)img->dibHeader.height * factor;
    float factor_x = max_x/img->dibHeader.width;
    float factor_y = max_y/img->dibHeader.height;
    printf("%d %d",max_x,max_y);
    clearAndResize(img,max_y,max_x);
    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            float dx = x/factor_x;
            float dy = y/factor_y;
            int new_x = (int)dx;
            int new_y = (int)dy;
            for (int rgb = 0; rgb<3; rgb++){
                int delta_x = getP(&copy_img,new_y,new_x+1,rgb)-getP(&copy_img,new_y,new_x,rgb);
                int delta_y = getP(&copy_img,new_y+1,new_x,rgb)-getP(&copy_img,new_y,new_x,rgb);
                int delta_xy = getP(&copy_img,new_y,new_x,rgb)+getP(&copy_img,new_y+1,new_x,rgb+1)-getP(&copy_img,new_y,new_x+1,rgb)-getP(&copy_img,new_y+1,new_x,rgb);
                int value = delta_x * dx + delta_y * dy + delta_xy * dx * dy + getP(&copy_img,new_y,new_x,rgb);
                setP(img,y,x,rgb,value);

                //setP(img, y, x, rgb, getP(&copy_img, (int)(y/factor_y), (int)(x/factor_x), rgb));
            }
            printf("(%d,%d) ",(int)(x/factor_x),(int)(y/factor_y));
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
    grayscale(&img);
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