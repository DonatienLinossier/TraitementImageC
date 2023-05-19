#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gestionFichierImg.h"

//Fonction qui passe l'image en noir et blanc
void grayscale(Image *img);

//Fonction qui inverse les couleurs de l'image
void reverse_image(Image *img);

//Fonction qui fait une symetrie de l'image selon l'axe y
void symmetry_y(Image *img);

//Fonction qui fait une symetrie de l'image selon l'axe x
void symmetry_x(Image *img);

//Fonction qui effectue une rotation de l'image de 90 degrés dans le sens horaire
int rotate_90(Image *img);

//Fonction qui floute l'image selon un facteur
int blur(Image *img, int factor);

//Fonctions qui redimensionne l'image selon un facteur
int resize(Image *img, float factor);

#endif