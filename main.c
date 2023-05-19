#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include "interface.h"
#include <string.h>

void main() {
    //Déclaration
    Image img;
    FILE* activeFile = NULL;
    int choice;


    //corps de la fonction
    printf("Bienvenu sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ? (1/2/3/...)");


    
    activeFile = fileChoice(); //Selection et ouverture du fichier image

    
    img = getImageFromFile(activeFile); //Chargement de l'image
    fclose(activeFile); 

    
    //Selection des opérations à faire sur l'image
    choice = 0;
    while(choice!=15) {
        choice = choiceImageManipulation(); //Affiche les possibilités à l'utilisateur et retourne son choix
        switch(choice) {
            case 1:
                resizeInterface();
                break;
            case 2:
                rognerInterface();
                break;
            case 3:
                affichageASCIIInterface(&img);
                break;
            case 4:
                BlackAndWhiteInterface();
                break;
            case 5:
                rotationInterface();
                break;
            case 6:
                luminositeInterface();
                break;
            case 7:
                contrasteInterface();
                break;
            case 8:
                flouInterface();
                break;
            case 9:
                BinariserInterface();
                break;
            case 10:
                inverserCouleursInterface();
                break;
            case 11:
                symetrieInterface();
                break;
            case 12:
                steganographieInterface(&img);
                break;
            case 13:
                saveImageInterface(&img);
                break;
            case 14:
                changeImageInterface(activeFile, &img);
                break;
        }
    }
}