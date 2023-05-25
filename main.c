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

    
    //Selection des opérations à faire sur l'image
    choice = 0;
    while(choice!=15) {
        choice = choiceImageManipulation(); //Affiche les possibilités à l'utilisateur et retourne son choix
        switch(choice) {
            case 1:
                resizeInterface(&img);
                break;
            case 2:
                rognerInterface(&img);
                break;
            case 3:
                affichageASCIIInterface(&img);
                break;
            case 4:
                grayscaleInterface(&img);
                break;
            case 5:
                rotationInterface(&img);
                break;
            case 6:
                luminositeInterface();
                break;
            case 7:
                contrasteInterface();
                break;
            case 8:
                flouInterface(&img);
                break;
            case 9:
                binariserInterface(&img);
                break;
            case 10:
                inverserCouleursInterface(&img);
                break;
            case 11:
                symetrieInterface(&img);
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

    printf("Fermeture du programme");
    return 0;
}