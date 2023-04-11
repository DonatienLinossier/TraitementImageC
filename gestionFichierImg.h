#ifndef GESTIONFICHIERIMG_H
#define GESTIONFICHIERIMG_H

#include <stdio.h>

//---------------------------
//Structure de l'image 
//---------------------------
/*
    Image---->Header-----------|- type
      |                        |- size
      |                        |- createdBy
      |                        |- CreatedByBis
      |                        |- Offset
      |                   
      |------>DIBHeader--------|- SizeHeader
      |                        |- width
      |                        |- height
      |                        |- ... 
      |                        |- Le reste du DIBHeader comporte des informations non utiles aux projets et qui dépendent du type de Header de l'image, voir directement dans la fonction de decodage du DIBHeader concerné si besoin
      | 
      |------>Tableau 1D avec l'ensemble des valeurs rgb de l'image (Utiliser getP() pour accéder aux pixels, et setP() pour les modifier)



*/

//Structure du Header du fichier .bmp
typedef struct {
    char type[2];
    int size;
    int createdBy;
    int createdByBis;
    int offset;  
} Header;


//Structure du DIBHeader format ... (A completer)
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

//Structure de l'image
typedef struct {
    Header header;
    DibHeader dibHeader;
    int* image;
} Image;

//Afficher les infos du headers
void affichageHeader(Header header);

//Afficher les infos du DIBHeader
void affichageDIBHeader(DibHeader dibHeader);

//Fonction permettant de récuperer une structure image à partir d'un fichier donné
Image getImageFromFile(FILE *fichier);



//Récuperer la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image)
int getP(Image* image, int height,  int width, int rgb);

//Mettre la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image) à (valeur)
void setP(Image* image, int height, int width, int rgb, int value);

#endif