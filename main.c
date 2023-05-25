#include <stdlib.h>
#include <stdio.h>
#include "gestionFichierImg.h"
#include "interface.h"

/* Reste a faire :
                Finir les interfaces : 
                    luminositeInterface(); -> Asma 
                    contrasteInterface(); -> Asma 
                    RedimensionerInterface(Ajouter une liste de taille prédéfini,
                                           car spécifié dans le cahier des charges) -> ?

                Appliquer une fonction que sur une selection -> Elias 

                transformation image :
                    luminosite -> Asma
                    contraste -> Asma

                Selectioner un lot d'image -> ?

                ReadMe -> Asma
*/

int main() {
    //Déclaration
    Image img;
    FILE* activeFile = NULL;
    int choice;

    //corps de la fonction
    printf("Bienvenue sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ? (1/2/3/...)");


    
    activeFile = fileChoice(); //Selection et ouverture du fichier image

    
    img = getImageFromFile(activeFile); //Chargement de l'image
    fclose(activeFile); 
    int selection[] = {0, 0, img.dibHeader.width, img.dibHeader.height}; //Création de la sélection qui comprend l'image entière
    
    //Selection des opérations à faire sur l'image
    choice = 0;
    while(choice!=16) {
        choice = choiceImageManipulation(); //Affiche les possibilités à l'utilisateur et retourne son choix
        switch(choice) {
            case 1:
                resizeInterface(&img);
                break;
            case 2:
                selectionInterface(&img, selection);
                break;
            case 3:
                rognerInterface(&img);
                break;
            case 4:
                affichageASCIIInterface(&img);
                break;
            case 5:
                grayscaleInterface(&img);
                break;
            case 6:
                rotationInterface(&img);
                break;
            case 7:
                luminositeInterface();
                break;
            case 8:
                contrasteInterface();
                break;
            case 9:
                flouInterface(&img);
                break;
            case 10:
                binariserInterface(&img);
                break;
            case 11:
                inverserCouleursInterface(&img);
                break;
            case 12:
                symetrieInterface(&img);
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

    printf("Fermeture du programme");
    return 0;
}