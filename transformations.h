#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gestionFichierImg.h"

//Fonction qui passe l'image en noir et blanc dans la sélection
void grayscale(Image *img, int sel[4]);

//Fonction qui augmente ou baisse la luminosité dans la sélection
void brightness (Image *img, float percentage, int sel[4]);

//Fonction qui augmente le contraste de l'image dans la sélection
void contrast(Image *img, int sel[4]);

//Fonction qui binarise l'image dans la sélection
void binary(Image *img, int sel[4]);

//Fonction qui inverse les couleurs de l'image dans la sélection
void reverse_colors(Image *img, int sel[4]);

//Fonction qui fait une symetrie de l'image selon l'axe y dans la sélection
void symmetry_y(Image *img, int sel[4]);

//Fonction qui fait une symetrie de l'image selon l'axe x dans la sélection
void symmetry_x(Image *img, int sel[4]);

//Fonction qui effectue une rotation de l'image de 90 degrés dans le sens horaire
void rotate_90(Image *img);

//Fonction qui floute l'image selon un facteur dans la sélection
void blur(Image *img, int range, int sel[4]);

//Fonction qui rogne l'image selon la sélection
void crop(Image *img, int sel[4]);

//Fonctions qui redimensionne l'image à une nouvelle taille (x,y)
void resize(Image *img, int new_x, int new_y);

#endif