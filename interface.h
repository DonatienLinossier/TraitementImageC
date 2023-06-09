#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "imageManagement.h"


FILE* fileChoice();
//Permet de changer quelle partie de l'image sera impactée par les changements
void selectionInterface(Image* image, int *sel);
void resizeInterface(Image* image, int sel[4]);
void cropInterface(Image* image, int sel[4]);
void displayInterfaceASCII(Image* image);
void grayscaleInterface(Image* image, int sel[4]);
void rotateInterface(Image* image, int sel[4]);
void brightnessInterface(Image* image, int sel[4]);
void contrastInterface(Image* image, int sel[4]);
void blurInterface(Image* image, int sel[4]);
void binaryInterface(Image* image, int sel[4]);
void reverseColorsInterface(Image* image, int sel[4]);
void symmetryInterface(Image *image, int sel[4]);
void steganographyInterface(Image* image);
void saveImageInterface(Image* image);
void changeImageInterface(FILE* activeFile, Image* img, int sel[4]);
int choiceImageManipulation();
void addImageToImageTmp(Image* img, char* filename);
void undoInterface(Image *img, char* filename, int sel[4]);
void exitInterface(int* choiceRetour);

#endif
