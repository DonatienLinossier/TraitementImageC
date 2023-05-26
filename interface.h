#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "imageManagement.h"


FILE* fileChoice();
//Permet de changer quelle partie de l'image sera impactée par les changements
void selectionInterface(Image* image, int *sel);
void resizeInterface(Image* image);
void cropInterface(Image* image, int sel[4]);
void displayASCIIInterface(Image* image);
void grayscaleInterface(Image* image, int sel[4]);
void rotationInterface(Image* image);
void brightnessInterface(Image* image);
void contrastInterface(Image* image);
void blurInterface(Image* image, int sel[4]);
void binarizeInterface(Image* image, int sel[4]);
void invertcolorsInterface(Image* image, int sel[4]);
void symetryInterface(Image *image, int sel[4]);
void steganographyInterface(Image* image);
void saveImageInterface(Image* image);
void changeImageInterface(FILE* activeFile, Image* img);
int choiceImageManipulation();



#endif