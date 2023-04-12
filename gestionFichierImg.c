#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"

//Permet de décoder les infos dans les headers stockées en little Endian(La casi totalité des infos).
int decodageLittleEndian(int bytes[], int SIZE) {
    int resulttotal = 0;
    for (int i =0; i<SIZE; i++) {
        int nb = bytes[i];
        int result = 0;
        int decomposition[8];
        for(int h=0; h<8; h++) {  
                decomposition[h] = nb%2;  
                result += decomposition[h] * pow(2, h) * pow(2, 8*i);
                nb = nb/2;
        }
        resulttotal+= result;
        
    } 
    return resulttotal;
}


//Afficher les infos du headers
void affichageHeader(Header header) {
    printf("Information sur le header : \n");
    printf("    type : %s\n", header.type);
    printf("    Image size : %d\n", header.size);
    printf("    createdBy : %d\n", header.createdBy);
    printf("    createdByBis : %d\n", header.createdByBis);
    printf("    Offset : %d\n", header.offset);
}


void affichageDIBHeader(DibHeader dibHeader) {
    printf("Information sur le header : \n");
    printf("    sizeHeader : %d\n", dibHeader.sizeHeader);
    printf("    Image width : %d\n", dibHeader.width);
    printf("    Image height : %d\n", dibHeader.height);
}

int getP(Image* image, int height,  int width, int rgb) {
    return image->image[(height*(image->dibHeader.width*3) + width *3 + rgb)];
}

void setP(Image* image, int height, int width, int rgb, int value) {
    image->image[(height*(image->dibHeader.width*3) + width *3 + rgb)]=value;
}

void decoderANDgetHeader(FILE* fichier, Image* image) {
    int caractereActuel;
    int SizeElementsHeaders[5] = {2, 4, 2, 2, 4}; 
    //printf("HEADER :\n");
    for(int i =0; i<5; i++) {
        int tab[SizeElementsHeaders[i]];
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(fichier); //On lit le caractère
            //printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        switch(i) {
            case 0: 
                image->header.type[0] = tab[0];
                image->header.type[1] = tab[1];
                break;
            case 1:
                image->header.size = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 2:
                image->header.createdBy = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 3: 
                image->header.createdByBis = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 4:
                image->header.offset = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
        }
        //printf("\n");
    }
}


//ATTENTION, A MODIFIER EN FONCTION DU FORMAT
/*Reflexion perso:
On recupere que la taille du DIBHeader, la width et la height
Le fin du DIB Header depend de son type. Long a faire pour rien.
--> On partirait sur le truc dit juste au dessus. il faut juste que je garde la fin du dibHeader dans un tab. Pas bs de le decoder normalement. dp du DIB Header
*/
void decoderANDgetDIBHeader(FILE* fichier, Image* image) {
    int caractereActuel;
    int SizeElementsDIBHeaders[3] = {4, 4, 4};
    //int SizeElementsDIBHeaders[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    int* elementsDIBHeaders[3];
    //printf("\nDIB HEADER :\n");
    for(int i = 0; i<3; i++) {
        //printf("%d : ", i);
        int tab[SizeElementsDIBHeaders[i]];
        for(int j = 0; j < SizeElementsDIBHeaders[i]; j++) {
            //printf("\nj : %d \n", j);
            caractereActuel = fgetc(fichier); // On lit le caractère
            printf(" %d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        elementsDIBHeaders[i] = tab;
        switch(i) {
            case 0:
                image->dibHeader.sizeHeader = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
            case 1:
                image->dibHeader.width = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 2:
                image->dibHeader.height = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
        }
    }
    //récuperer le reste, sans le décoder
    //A VERIFIER!! YA PEUT ETRE UN DECALAGE DE UN
    int reste[image->dibHeader.sizeHeader-4-4-4];
    for(int i = 0; i<image->dibHeader.sizeHeader-4-4-4; i++) {
        caractereActuel = fgetc(fichier);
        reste[i] = caractereActuel;
        printf("%d ", caractereActuel);
    }
    image->dibHeader.reste = reste;
}



void decoderImg(FILE* fichier, Image* image) {
    int caractereActuel;
    int i = 0;
    int imagee[image->dibHeader.height * image->dibHeader.width * 3];
    while(caractereActuel != EOF && i!=3*image->dibHeader.width*image->dibHeader.height) {
        caractereActuel = fgetc(fichier);
        imagee[i] = caractereActuel;
        i++;
    }
    image->image = imagee; 
}


Image getImageFromFile(FILE *fichier) {
    Image image;

    decoderANDgetHeader(fichier, &image);
    //affichageHeader(image.header);

    decoderANDgetDIBHeader(fichier, &image);
    //affichageDIBHeader(image.dibHeader);

    decoderImg(fichier, &image);

    return image;
}








int main()
{


    FILE* fichier = NULL;

    fichier = fopen("bmp_24.bmp", "rb");
    //fichier = fopen("img.bmp", "rb");

    Image image = getImageFromFile(fichier);

    fclose(fichier);
    

    
    /*int i = 0;
    printf("%d", image.dibHeader.height * image.dibHeader.width * 3);
    int imagee[image.dibHeader.height * image.dibHeader.width * 3];
    while(caractereActuel != EOF && i!=3*image.dibHeader.width*image.dibHeader.height) {
        caractereActuel = fgetc(fichier);
        imagee[i] = caractereActuel;
        i++;
    }
    //int j=i;
    image.image = imagee; 
    */

    /*for(int i =0; i<image.dibHeader.height * image.dibHeader.width * 3 && i<j; i++) {
        //printf("%d ", image.image[i]);
    }*/

    for(int i = 0; i<image.dibHeader.height; i++) {
        for(int j = 0; j<image.dibHeader.width; j++) {
            for(int k = 0; k<3; k++) {
                if(getP(&image, i, j, k)>255 || getP(&image, i, j, k)<0) {
                    printf("\n");
                }
                printf("%d ", getP(&image, i, j, k));
            }
        }
    }


    //printf("\nDone\n");
    return 0;
    
}












//-----------------------------------------
// NE PLUS PRENDRE EN COMPTE A PARTIR DE LA
//-----------------------------------------

/*
void affichageDIBHeader(DibHeader dibHeader) {
    printf("Information sur le header : \n");
    printf("    sizeHeader : %d\n", dibHeader.sizeHeader);
    printf("    Image width : %d\n", dibHeader.width);
    printf("    Image height : %d\n", dibHeader.height);
    printf("    nbColorplane : %d\n", dibHeader.nbColorplane);
    printf("    nbBitByPixel : %d\n", dibHeader.nbBitByPixel);
    printf("    compressionMethod : %d\n", dibHeader.compressionMethod);
    printf("    imageSize : %d\n", dibHeader.imageSize);
    printf("    horizontalResolution : %d\n", dibHeader.horizontalResolution);
    printf("    verticalResolution : %d\n", dibHeader.verticalResolution);
    printf("    nbColorInPalette : %d\n", dibHeader.nbColorInPalette);
    printf("    nbOfImportantColorUsed : %d\n", dibHeader.nbOfImportantColorUsed);
}



*/


/*PAS UTILISé CAR TROP GALERE AVEC LES DIFFENTES FORMATS EXISTANTS. ON UTILISERA SUREMENT QUE LE TAILLE DU HEADER, AINSI QUE LA LARGEUR ET LONGUEUR DE L'IMAGE
typedef struct {
    int sizeHeader;
    int width;
    int height;
    int nbColorplane; //must be 1
    int nbBitByPixel;
    int compressionMethod;
    int imageSize;
    int horizontalResolution;
    int verticalResolution;
    int nbColorInPalette;
    int nbOfImportantColorUsed;
} DibHeader;

void affichageDIBHeader(DibHeader dibHeader) {
    printf("Information sur le header : \n");
    printf("    sizeHeader : %d\n", dibHeader.sizeHeader);
    printf("    Image width : %d\n", dibHeader.width);
    printf("    Image height : %d\n", dibHeader.height);
    printf("    nbColorplane : %d\n", dibHeader.nbColorplane);
    printf("    nbBitByPixel : %d\n", dibHeader.nbBitByPixel);
    printf("    compressionMethod : %d\n", dibHeader.compressionMethod);
    printf("    imageSize : %d\n", dibHeader.imageSize);
    printf("    horizontalResolution : %d\n", dibHeader.horizontalResolution);
    printf("    verticalResolution : %d\n", dibHeader.verticalResolution);
    printf("    nbColorInPalette : %d\n", dibHeader.nbColorInPalette);
    printf("    nbOfImportantColorUsed : %d\n", dibHeader.nbOfImportantColorUsed);
}*/

    /*ON NE PASSE PLUS PAR UN TAB 3D, donc obsolete
    int imagee[image.height][image.width][3];
    //vérif le décalage, peut etre un pb. Peut etre qu'on ne commence pas au db; pb avec le -1??
    while(caractereActuel != EOF && i!=3*image.width*image.height) {
        caractereActuel = fgetc(fichier);
        int indiceRGB = i%3;
        int indiceWIDTH = (i/3)%image.height;
        int indiceHEIGHT = (i/3)/image.height;
        //printf("%d: [%d][%d][%d] ", i, indiceHEIGHT, indiceWIDTH, indiceRGB);
        imagee[indiceHEIGHT][indiceWIDTH][indiceRGB] = caractereActuel;
        i++;
    }*/



        /*int SizeElementsHeaders[5] = {2, 4, 2, 2, 4};
    int* elementsHeaders[5];   

    printf("HEADER :\n");
    for(int i =0; i<5; i++) {
        int tab[SizeElementsHeaders[i]];
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(fichier); //On lit le caractère
            printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        switch(i) {
            case 0: 
                image.header.type[0] = tab[0];
                image.header.type[1] = tab[1];
                break;
            case 1:
                image.header.size = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 2:
                image.header.createdBy = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 3: 
                image.header.createdByBis = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 4:
                image.header.offset = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
        }
        elementsHeaders[i] = tab;

        
        
        printf("\n");
    }
    */



    //contient la taille de tous les elements du DIBHeader format BITMAPINFOHEADER. Mais on ne vas décoder que les 3 premiers elements qui semblent commun a tous. 
    //Je ne stocke pas la taille du header, mais cela surement indispensable pour réecrire le fichier 
    /*int SizeElementsDIBHeaders[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    int* elementsDIBHeaders[11];   
    //printf("\nDIB HEADER :\n");
    for(int i = 0; i<11; i++) {
        //printf("%d : ", i);
        int tab[SizeElementsDIBHeaders[i]];
        for(int j = 0; j < SizeElementsDIBHeaders[i]; j++) {
            //printf("\nj : %d \n", j);
            caractereActuel = fgetc(fichier); // On lit le caractère
            //printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        elementsDIBHeaders[i] = tab;
        switch(i) {
            case 0:
                image.dibHeader.sizeHeader = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
            case 1:
                image.width = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 2:
                image.dibHeader.height = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 3:
                image.dibHeader.nbColorplane = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 4:
                image.dibHeader.nbBitByPixel = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 5:
                image.dibHeader.compressionMethod = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 6:
                image.dibHeader.imageSize = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 7:
                image.dibHeader.horizontalResolution = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 8:
                image.dibHeader.verticalResolution = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 9:
                image.dibHeader.nbColorInPalette = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 10:
                image.dibHeader.nbOfImportantColorUsed = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
        }
        printf("\n");
    }*/