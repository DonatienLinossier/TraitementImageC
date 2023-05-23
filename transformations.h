#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gestionFichierImg.h"

//Fonction qui passe l'image en noir et blanc
void grayscale(Image *img, int x1, int y1, int x2, int y2);

//Fonction qui binarise l'image
void binary(Image *img, int x1, int y1, int x2, int y2);

//Fonction qui inverse les couleurs de l'image
void reverse_image(Image *img, int x1, int y1, int x2, int y2);

//Fonction qui fait une symetrie de l'image selon l'axe y
void symmetry_y(Image *img, int x1, int y1, int x2, int y2);

//Fonction qui fait une symetrie de l'image selon l'axe x
void symmetry_x(Image *img, int x1, int y1, int x2, int y2);

//Fonction qui effectue une rotation de l'image de 90 degrés dans le sens horaire
void rotate_90(Image *img);

//Fonction qui floute l'image selon un facteur
void blur(Image *img, int factor);

//Fonctions qui redimensionne l'image à une nouvelle taille (x,y)
void resize(Image *img, int new_x, int new_y);

#endif