#ifndef GESTIONFICHIERIMG_H
#define GESTIONFICHIERIMG_H

#include <stdio.h>

//---------------------------
//Mode d'emploi récupération de l'image
//---------------------------
/*
    FILE* fichier = NULL;

    fichier = fopen("%image%.bmp", "rb");

    Image image = getImageFromFile(fichier);

    fclose(fichier);
*/

//---------------------------
//Mode d'emploi redimensionner
//---------------------------
/*
void maFonctionQuiABesoinDeRedimensioner(Image *image, int height, int width, Autres arguments de la fonctions...) {

    (...)

    //imgTemporaire est une image vide qui possede les informations du Header et du DIbHeader de image, mais qui est de la dimension souhaitée.
    //image possede donc encore toutes ses valeurs. 
    Image imgTemporaire = newBlankImageFrom(image, height, width);

    
    //Je fais mes opérations en prenant les infos sur image (avec getP()), mais je change les infos sur imgTemporaire(avec setP())
    //Les opérations faites sur image sont inutiles car seront écrasé par la suite du programme par celles d'imgTemporaire
    MesOpérations();


    //Une fois mes opérations finies et que imgTemporaire correspond bien l'image que je veux retourner, il ne me reste plus qu'à remplacer mon image par imgTemporaire
    // /!\ Attention : les valeurs contenues dans image seront écrasées, et seront donc perdues /!\ 
    image = imgTemporaire;

    (...)
}


*/

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
      |------>DIBHeader--------|- SizeHeadergit 
      |                        |- width
      |                        |- height
      |                        |- ... 
      |                        |- Le reste du DIBHeader comporte des informations non utiles aux projets et qui dépendent du type de Header de l'image, voir directement dans la fonction de decodage du DIBHeader concerné si besoin
      | 
      |------>Tableau 1D avec l'ensemble des valeurs rgb de l'image (Utiliser getP() pour accéder aux pixels, et setP() pour les modifier.)



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
    int* reste;
    /*int nbColorplane; //must be 1
    int nbBitByPixel;
    int compressionMethod;
    int imageSize;
    int horizontalResolution;
    int verticalResolution;
    int nbColorInPalette;
    int nbOfImportantColorUsed;*/
} DibHeader;

//Structure du fichier image
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

//Retourne une nouvelle image vide[noir] de dimension (height) * (width) et remplie le header et le DIBHeader avec les infos de l'image entrée en parametre 
Image newBlankImageFrom(Image *image, int height, int width);

#endif