#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "gestionFichierImg.h"


FILE* fileChoice();
//Permet de changer quelle partie de l'image sera impactée par les changements
void selectionInterface(Image* image, int *sel);
void resizeInterface(Image* image);
void rognerInterface(Image* image, int sel[4]);
void affichageASCIIInterface(Image* image);
void grayscaleInterface(Image* image, int sel[4]);
void rotationInterface(Image* image);
void luminositeInterface();
void contrasteInterface();
void flouInterface(Image* image, int sel[4]);
void binariserInterface(Image* image, int sel[4]);
void inverserCouleursInterface(Image* image, int sel[4]);
void symetrieInterface(Image *image, int sel[4]);
void steganographieInterface(Image* image);
void saveImageInterface(Image* image);
void changeImageInterface(FILE* activeFile, Image* img);
int choiceImageManipulation();



#endif