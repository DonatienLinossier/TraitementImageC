#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "gestionFichierImg.h"


FILE* fileChoice();
//Permet de changer quelle partie de l'image sera impactée par les changements
void selectionInterface(Image* image, int *sel);
void resizeInterface(Image* image);
void rognerInterface(Image* image);
void affichageASCIIInterface(Image* image);
void grayscaleInterface(Image* image);
void rotationInterface(Image* image);
void luminositeInterface();
void contrasteInterface();
void flouInterface(Image* image);
void binariserInterface(Image* image);
void inverserCouleursInterface(Image* image);
void symetrieInterface(Image *image);
void steganographieInterface(Image* image);
void saveImageInterface(Image* image);
void changeImageInterface(FILE* activeFile, Image* img);
int choiceImageManipulation();



#endif