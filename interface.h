#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "gestionFichierImg.h"


FILE* fileChoice();
void resizeInterface(Image* image);
//void rognerInterface();
void affichageASCIIInterface(Image* image);
//void BlackAndWhiteInterface();
void rotationInterface();
void luminositeInterface();
void contrasteInterface();
void flouInterface();
void BinariserInterface();
void inverserCouleursInterface();
void symetrieInterface();
void steganographieInterface(Image* image);
void saveImageInterface(Image* image);
void changeImageInterface(FILE* activeFile, Image* img);
int choiceImageManipulation();



#endif