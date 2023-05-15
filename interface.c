#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h> //Pas nativement sur windows
#include <string.h>



FILE* fileChoice() {
    FILE* activeFile = NULL;
    DIR *d;
    struct dirent *dir;

    //récupérer repertoire actuel(A faire à CYtech pour etre sur de la compatibilité)
    char *rep;
    rep=(char *)malloc(100*sizeof(char));
    getcwd(rep,100);
    //printf("\n %s \n",rep);

    /*
    Stocker tout les noms dans un tableau dynamique de chaine de caractere dynamique
    Afficher tout les fichier .bmp
    Récuperer le choix de l'utilisateur
    Ouvrir l'image(avec le tabl dynamque)
    Choix des opérations sur cette image ...
    */
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


    d = opendir(rep);
    if (d)
    {
        int j = 0;
                while (j < choice && (dir = readdir(d)) != NULL)
        {
            int length = strlen(dir->d_name);
            if(!(dir->d_name[length-4] == '.' && dir->d_name[length-3] == 'b' && dir->d_name[length-2] == 'm' && dir->d_name[length-1] == 'p')) {
                continue;
            }
        	//printf("%d: %s \n", j, dir->d_name);
            j++;
        }
        char * name = dir->d_name;
        activeFile = fopen(name, "wb+");
        //printf("%s opened!\n", name);
        closedir(d);
    }

    return activeFile;
} 

void redimensionerInterface() {

}
void rognerInterface() {
    
}
void affichageASCIIInterface() {
    
}
void noirEtBlancInterface() {
    
}
void rotationInterface() {
    
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
    
}
void symetrieInterface() {
    
}
void saveImageInterface() {
    
}

void changeImageInterface(FILE* activeFile, Image* img) {
    printf("Etes-vous sur de vouloir changer d'image ? Vos modifications non enregistrés seront effacés (Y/N)");
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
    printf("    12 - Enregistrer l'image\n");
    printf("    13 - Changer d'image (Abandonne les modfications)\n");
    printf("    14 - Fermer le programme (Abandonne les modifications)\n");
    scanf("%d", &choice);
    return choice;

}

void main() {
    printf("Bienvenu sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ? (1/2/3/...)");

    FILE* activeFile = NULL;

    activeFile = fileChoice();
    if(activeFile == NULL) {
        printf("Erreur dans l'ouverture du fichier !");
        exit(0);
    }

    Image img = getImageFromFile(activeFile);
    fclose(activeFile); 

    int choice =0;

    while(choice!=14) {
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
                saveImageInterface();
                break;
            case 13:
                changeImageInterface(activeFile, &img);
                break;
        }
    




    }
    


}
