#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include "interface.h"
#include <string.h>

int main() {
    //Déclaration
    Image img;
    FILE* activeFile = NULL;
    int choice;
    int selection[4];

    //corps de la fonction
    printf("Bienvenue sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ? (1/2/3/...)");


    
    activeFile = fileChoice(); //Selection et ouverture du fichier image

    
    img = getImageFromFile(activeFile); //Chargement de l'image
    fclose(activeFile); 
    int selection[] = {0, 0, img.dibHeader.width, img.dibHeader.height}; //Création de la sélection qui comprend l'image entière
    
    //Selection des opérations à faire sur l'image
    choice = 0;
    while(choice!=15) {
        choice = choiceImageManipulation(); //Affiche les possibilités à l'utilisateur et retourne son choix
        switch(choice) {
            case 1:
                resizeInterface(&img);
                break;
            case 2:
                selectionInterface(&img, &selection);
                break;
            case 3:
                //rognerInterface();
                break;
            case 4:
                affichageASCIIInterface(&img);
                break;
            case 5:
                //BlackAndWhiteInterface();
                break;
            case 6:
                rotationInterface();
                break;
            case 7:
                luminositeInterface();
                break;
            case 8:
                contrasteInterface();
                break;
            case 9:
                flouInterface();
                break;
            case 10:
                BinariserInterface();
                break;
            case 11:
                inverserCouleursInterface();
                break;
            case 12:
                symetrieInterface();
                break;
            case 13:
                steganographieInterface(&img);
                break;
            case 14:
                saveImageInterface(&img);
                break;
            case 15:
                changeImageInterface(activeFile, &img);
                break;
        }
    }

    printf("fin du programme");
    return 0;
}