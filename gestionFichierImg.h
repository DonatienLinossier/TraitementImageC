#ifndef GESTIONFICHIERIMG_H
#define GESTIONFICHIERIMG_H

#include <stdio.h>

//---------------------------
//Utilisation de Image 
//---------------------------
/*
- - - CREATION DE IMAGE - - - 
    Pour créer une image à partir d'un fichier utiliser la fonction getImageFromFile(fichier) :
        |-----------------------------------------------------------|
        |                                                           |
        |    FILE* fichier = NULL;                                  |
        |                                                           |
        |    fichier = fopen("Images/image.bmp", "rb");             |
        |    if(fichier == NULL) {                                  |
        |        exit(0);                                           |
        |    }                                                      |
        |                                                           |
        |    Image image = getImageFromFile(fichier);               |
        |                                                           |
        |   fclose(fichier);                                        |
        |                                                           |
        |-----------------------------------------------------------|


    Pour créer une nouvelle image à partir d'une image existante utiliser copy(&image) :
        |-----------------------------------------------------------|
        |                                                           |
        |   Image copie = copy(&image);                             |
        |                                                           |
        |-----------------------------------------------------------|



- - - DIMENSION DE IMAGE - - -
Pour accéder aux dimensions de l'image :
    - Hauteur : image.dibHeader.height
    - Largeur : image.dibHeader.width

    /!\ Pour la hauteur comme la largeur, ne pas modifier les variables directement. Utiliser la fonction Clearredimensioner() pour changer la taille de l'image. /!\




- - - PIXEL DE IMAGE - - -
Pour acceder et modifer les pixels de l'image :
    - Lecture : getP(Image* image, int height, int wight, int rgb) retourne la composante r,g ou b du pixel à la ligne height, colonne width de image.
    - Modification : setP(Image* image, int height, int wight, int rgb, int value) met la composante r,g ou b du pixel à la ligne height, colonne width de image à la valeur value.

    /!\ Que ce soit pour modifier ou lire un pixel, toujours passer par getP() et setP(), mais ne jamais utiliser image.image[x] /!\



- - - AFFICHAGE DE IMAGE - - - 
Pour afficher l'image et avoir un premier rendu il est possible d'utiliser afficherASCII(Image* image).

    |!| Pour les images dépassant de la console, il est préférable d'afficher une copie redimensionée de l'image : |!|
        |-----------------------------------------------------------|
        |                                                           |
        |    Image affichage = copy(&image));                       |
        |    redimensioner(&affichage, 50, 50);                     | /!\ NE PAS UTILISER POUR L'INSTANT, fonction redimensioner marche moyennement bien
        |    afficherASCII(&affichage);                             |
        |    freeImage(&affichage);                                 |
        |                                                           |
        |-----------------------------------------------------------|



- - - ROGNER UNE IMAGE - - - 
Pour rogner une image il suffit d'utiliser la foncion rogner(&image, int PosYCoinSupérieurGauche,  int PosXCoinSupérieurGauche, int hauteur, int longueur)

    /!\ Attention à ne pas deborder de l'image /!\


WIP - - - REDIMENSIONER UNE IMAGE - - -  WIP
Pour redimensioner une image il suffit d'utiliser la fonction redimensioner(&image, int hauteur, int longueur)

    /WIP\ Opérationel sur les dimensions de l'image, mais de maniere approximative sur le rendu redimensioner de l'image.(potentiellement un flou et création de blanc lors d'un agrandissement)


- - -  SUPRESSION DE IMAGE  - - -
Une partie de l'image est alloué de maniere dynamique, il est important d'utiliser freeImage(&image) dès qu'on ne se sert plus de l'image.

    /!\ Ne pas le faire provoquerait une fuite de memoire /!\


- - - CREATION D'UN FICHIER POUR IMAGE - - - 
    Pour creer un fichier à partir d'une image utiliser la fonction writeFileFromImage(FILE* fichier, Image* image) :
        |-----------------------------------------------------------|
        |                                                           |
        |    FILE* fichier = NULL;                                  |
        |                                                           |
        |    fichier = fopen("Output/EcritureImg.bmp", "wb+");      |
        |    if(fichier == NULL) {                                  |
        |        exit(0);                                           |
        |    }                                                      |
        |                                                           |
        |    writeFileFromImage(fichier, &image);                   |
        |                                                           |
        |    fclose(fichier);                                       |
        |                                                           |
        |    //freeImage(&image); si on n'utilise plus l'image      |
        |                                                           |
        |-----------------------------------------------------------|



*/




//Structure du Header du fichier .bmp
typedef struct {
    char type[2];
    int size;
    int createdBy;
    int createdByBis;
    int offset;  
} Header;


//Structure du DIBHeader (Seul la taille du header, la largeur et la hauteur de l'image sont décodés. Le reste ne l'est pas afin de permettre la compatibilité avec les différents types de DIBHeaders)
typedef struct {
    int sizeHeader;
    int width;
    int height;
    unsigned char* reste;
} DibHeader;

//Structure du fichier image
typedef struct {
    Header header;
    DibHeader dibHeader;
    unsigned char* image;
    short padding;//(Interne à l'image, ne sert pas en dehors)
} Image;



//Fonction permettant de récuperer une structure image à partir d'un fichier donné
Image getImageFromFile(FILE *fichier);

//Fonction permettant d'écrire une image dans un fichier
void writeFileFromImage(FILE* fichier, Image* image);

//Crée une copie de l'image(qui est indépendante de l'image)
Image copy(Image *image);

//Récuperer la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image)
unsigned char getP(Image* image, int height,  int width, int rgb);

//Mettre la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image) à (valeur)
void setP(Image* image, int height, int width, int rgb, int value);

//Permet de rogner une image
void rogner(Image *image, int y, int x, int height, int width);

//Redimensionne l'image et affecte 
void ClearAndRedimensioner(Image *image, int height, int width);

//Affiche l'image en caractere ASCII avec " .:?#"
void afficherASCII(Image* image);

//Libere l'espace mémoire alloué dynamiquement de l'image
void freeImage(Image *image);


#endif