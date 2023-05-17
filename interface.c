#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h> //Pas nativement sur windows ? 
 // les fonctions FindFirst / FindNext / FindClose peuvent remplacer opendir / readdir / closedir sauf si ça marche sur linux 
#include <string.h>

#include "transformations.c" 



FILE* fileChoice() {
    FILE* activeFile = NULL;
    DIR *d;
    struct dirent *dir;

    /*
    Focntionnement :
        Recuperer dossier actuel
        Ouvrir le dossier Images
        Lister les fichier .bmp
        Recuperer choix user
        ouvrir fichier
        renvoyer fichier
    
    */
    char *rep;
    rep=(char *)malloc(100*sizeof(char));
    getcwd(rep,100);


    strcat(rep, "/Images");
    d = opendir(rep);

    if (d)
    {
        printf("\n");
        int i = 1;
        while ((dir = readdir(d)) != NULL)
        {
            int length = strlen(dir->d_name);
            if(!(dir->d_name[length-4] == '.' && dir->d_name[length-3] == 'b' && dir->d_name[length-2] == 'm' && dir->d_name[length-1] == 'p')) {
                continue;
            }
        	
            printf("    %d: %s \n", i, dir->d_name);
            i++;
        }
        closedir(d);
    }

    int choice;
    //Faire les vérifs
    scanf("%d", &choice);
    // fscanf( activeFile,"%d", &choice);

    int lengthMAx=0;
    d = opendir(rep);
    if (d)
    {
        int j = 0;
        while (j < choice && (dir = readdir(d)) != NULL)
        {
            int length = strlen(dir->d_name);
            if (length>lengthMAx) {
                lengthMAx = length;
            }
            if(!(dir->d_name[length-4] == '.' && dir->d_name[length-3] == 'b' && dir->d_name[length-2] == 'm' && dir->d_name[length-1] == 'p')) {
                continue;
            }
            j++;
        }
        char * name = NULL;
        name = calloc(lengthMAx, sizeof(char));
        if (name == NULL) {
            exit(0);
        }
        strcat(name, "Images/");
        strcat(name, dir->d_name);
        activeFile = fopen(name, "rb+");
        if(activeFile == NULL) {
            printf("Erreur dans l'ouverture du fichier !");
            exit(0);
        }
        printf("%s opened!\n", name);
        closedir(d);
    }

    return activeFile;
} 

void redimensionerInterface(FILE* activeFile, Image* img ) {
    // facteur de redimension = ratio en pourcentage
    float facteur;
    printf("De quel facteur voulez-vous redimensionner l'image ?");
    scanf("%f", &facteur);
    printf("Votre image sera redimensioner de %f %", facteur);
    redimensionner(Image *img, float facteur);
}

void rognerInterface() {
    
}
void affichageASCIIInterface() {
    
}
void noirEtBlancInterface() {
    
}

void rotationInterface(FILE* activeFile) {
    printf("Votre image va effectuer une rotation de 90 degrés:");
    rota_90(activeFile);
}
void luminositeInterface() {
    
}
void contrasteInterface() {
    
}
void flouInterface() {
    
}
void BinariserInterface() {
    
}
void inverserCouleursInterface() {
    printf(" L'inversion des couleurs rendra")
    inverse_image(Image img)
}


void symetrieInterface() {
    
}


void steganographieInterface() {

}

void saveImageInterface() {
    
}

void changeImageInterface(FILE* activeFile, Image* img) {
    printf("Etes-vous sur de vouloir changer d'image ? Vos modifications non enregistres seront effacées (Y/N)");
    char input = ' ';
    while(input!='y'&& input!='Y' && input!='N' && input!='n') {
        //Faire vérif input
        scanf("%c", &input);
    }

    if(input=='n' || input == 'N') {
        return;
    }
    printf("Quelle image voulez-vous modifier ? (1/2/3/...)");
    activeFile = NULL;

    activeFile = fileChoice();
    if(activeFile == NULL) {
        printf("Erreur dans l'ouverture du fichier !");
        exit(0);
    }

    freeImage(img);
    
    *img = getImageFromFile(activeFile);
    fclose(activeFile); 


}

int choixManipulationImage() {
    int choice;
    printf("Que voulez vous faire ?\n");
    printf("     1 - Redimensioner\n");
    printf("     2 - Rogner\n");
    printf("     3 - Afficher l'image en ASCII\n");
    printf("     4 - Passer l'image en noir et blanc\n");
    printf("     5 - Rotation\n");
    printf("     6 - Changer la luminosite\n");
    printf("     7 - Changer le contraste\n");
    printf("     8 - Flouter l'image\n");
    printf("     9 - Binariser l'image\n");
    printf("    10 - Inverser les couleurs\n");
    printf("    11 - Effectuer une symetrie\n");
    printf("    12 - Steganographie\n");
    printf("    13 - Enregistrer l'image\n");
    printf("    14 - Changer d'image (Abandonne les modfications)\n");
    printf("    15 - Fermer le programme (Abandonne les modifications)\n");
    //Verif inputs
    scanf("%d", &choice);
    return choice;

}

void main() {

    FILE* activeFile = NULL;

    printf("Bienvenu sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ? (1/2/3/...)");

    

    activeFile = fileChoice();

    Image img = getImageFromFile(activeFile);
    fclose(activeFile); 

    int choice =0;

    while(choice!=15) {
        choice = choixManipulationImage();
        switch(choice) {
            case 1:
                redimensionerInterface();
                break;
            case 2:
                rognerInterface();
                break;
            case 3:
                affichageASCIIInterface();
                break;
            case 4:
                noirEtBlancInterface();
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
                steganographieInterface();
                break;
            case 13:
                saveImageInterface();
                break;
            case 14:
                changeImageInterface(activeFile, &img);
                break;
        }
    }
}
