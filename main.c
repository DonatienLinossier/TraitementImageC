#include <stdlib.h>
#include <stdio.h>
#include "imageManagement.h"
#include "interface.h"

#define FILE_TMP_IMG "tmp/tmpImage"

/* Reste a faire :
                Finir les interfaces : 
                    RedimensionerInterface(Ajouter une liste de taille prédéfini,
                                           car spécifié dans le cahier des charges) -> ?


                Selectioner un lot d'image -> ?

                ReadMe -> Asma


    ERREUR :
        -Fonction ecraser si meme fichier -  normalement OK - A verif
        -crash ouverture fichier cree manuellement - normalement OK  (Si pb -> crash du programme avec fopen. )  /!\ GROSSE VERIF A FAIRE !!! /!\
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
    addImageToImageTmp(&img, FILE_TMP_IMG);
    fclose(activeFile); 
    int selection[] = {0, 0, img.dibHeader.width, img.dibHeader.height}; //Création de la sélection qui comprend l'image entière
    
    //Selection des opérations à faire sur l'image
    choice = 0;
    while(choice!=17) {
        choice = choiceImageManipulation(); //Affiche les possibilités à l'utilisateur et retourne son choix
        if(choice!=2 && choice != 4 && choice != 14 && choice != 16) {
            addImageToImageTmp(&img, FILE_TMP_IMG);
        }
        switch(choice) {
            case 1:
                resizeInterface(&img);
                break;
            case 2:
                selectionInterface(&img,selection);
                break;
            case 3:
                cropInterface(&img,selection);
                break;
            case 4:
                displayInterfaceASCII(&img);
                break;
            case 5:
                grayscaleInterface(&img,selection);
                break;
            case 6:
                rotateInterface(&img);
                break;
            case 7:
                brightnessInterface(&img,selection);
                break;
            case 8:
                contrastInterface(&img,selection);
                break;
            case 9:
                blurInterface(&img, selection);
                blurInterface(&img, selection);
                break;
            case 10:
                binaryInterface(&img, selection);
                break;
            case 11:
                reverseColorsInterface(&img, selection);
                break;
            case 12:
                symmetryInterface(&img, selection);
                symmetryInterface(&img, selection);
                break;
            case 13:
                steganographyInterface(&img);
                steganographyInterface(&img);
                break;
            case 14:
                saveImageInterface(&img);
                break;
            case 15:
                changeImageInterface(activeFile, &img);
                break;
            case 16:
                getLastImage(&img, FILE_TMP_IMG);
                break;
        }
    }

    printf("Fermeture du programme");
    return 0;
}


