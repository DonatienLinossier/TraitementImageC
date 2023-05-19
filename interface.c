#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h> //Pas nativement sur windows ?
#include <string.h>


/* Points d'attention :
    - Ouverture fichier. Le programme s'est arrété une fois en voulant ouvrir une image qui venait d'etre crée.
    - La steganographie ne marche pas tout le temps ?    
*/


//Retourne true si le nom du fichier est valide, false sinon
int isFilenameValid(char* filename) {
    int length = strlen(filename);
    int filenameValid = 1;

    for(int iChar = 0; iChar < length; iChar++) {
        if(filename[iChar]=='<' || filename[iChar]=='>' || filename[iChar]==':' ||
           filename[iChar]=='"' || filename[iChar]=='/' || filename[iChar]=='\\' || 
           filename[iChar]=='|' || filename[iChar]=='?' || filename[iChar]=='*') {
                filenameValid = 0; 
                printf("'%c' n'est pas un caractere valide pour le nom d'un fichier !\n", filename[iChar]);
                break;
        }
    }
    return filenameValid;
}

//retourne true si le fichier est un bmp, false sinon.
int isBMP(char* filename) {
    int length = strlen(filename);
    if(length<4) { //Si le fichier fait moins de 4 caractere, ce n'est pas un bmp(Eviter les erreurs de segmentation)
        return 0; 
    }
    return (filename[length-4] == '.' && filename[length-3] == 'b' && filename[length-2] == 'm' && filename[length-1] == 'p');
}


//Permet à l'uilisateur de choisir son fichier dans Image dans Images et/ou Output et retourne le fichier ouvert
FILE* fileChoice() {
    FILE* activeFile = NULL;
    char *repertory;
    DIR *imageRepertory;
    struct dirent *file;
    char * name = NULL;


    //recuperer le dossier actuel
    repertory=(char *)malloc(100*sizeof(char));
    if (repertory == NULL) {
        exit(0);
    }
    getcwd(repertory, 100);


    
    //Se déplacer dans le dossier Image et l'ouvrir
    strcat(repertory, "/Images");
    imageRepertory = opendir(repertory);

    //Enumerer les fichiers dans le dossier image + possibilité de lister les images contenu dans output
    int iFile = 1;
    if (imageRepertory)
    {
        printf("\n");
        while ((file = readdir(imageRepertory)) != NULL)
        {
            if(!isBMP(file->d_name)) {
                continue;
            }
        	
            printf("    %d: %s\n", iFile, file->d_name);
            iFile++;
        }
        closedir(imageRepertory);
        printf("    %d: Ouvrir le dossier Output contenant les images precedement modifiees\n", iFile, file->d_name);
    }    


    //recuperer le choix de l'utilisateur
    //Faire les vérifs
    int choice = 0;
    do {
        scanf("%d", &choice);
    } while(choice<1 || choice>iFile);


    if(choice!=iFile) {
        //Si un es fichier dans Images a été choisi 
        //int lengthMAx=0;
        imageRepertory = opendir(repertory);
        if (imageRepertory)
        {
            int j = 0;
            int length;
            //recuperer le fichier choisi
            while (j < choice && (file = readdir(imageRepertory)) != NULL)
            {
                length = strlen(file->d_name);
                if(!isBMP(file->d_name)) {
                    continue;
                }
                j++;
            }
            closedir(imageRepertory);
                
            name = calloc(length, sizeof(char));
            if (name == NULL) {
                exit(0);
            }
            strcat(name, "Images/");
            strcat(name, file->d_name);
        }
    } else {
        //Si l'utilisateur a prefere une image dans le dossier Output
        printf("Quelle image voulez-vous selectionner ? (1/2/3/...)");


        repertory=(char *)malloc(100*sizeof(char));
        if (repertory == NULL) {
            exit(0);
        }
        getcwd(repertory, 100);


        int iFile = 1;
        strcat(repertory, "/Output");
        imageRepertory = opendir(repertory);
        if (imageRepertory)
        {
            printf("\n");
            while ((file = readdir(imageRepertory)) != NULL)
            {
                int length = strlen(file->d_name);
                if(!isBMP(file->d_name)) {
                    continue;
                }
                printf("    %d: %s\n", iFile, file->d_name);
                iFile++;
            }
            closedir(imageRepertory);
        }
        int choice = 0;
        //Faire les vérifs
        do {
            scanf("%d", &choice);
        } while(choice<1 || choice>iFile);

        imageRepertory = opendir(repertory);
        if (imageRepertory)
        {
            int length;
            int j = 0;
            while (j < choice && (file = readdir(imageRepertory)) != NULL)
            {
                length = strlen(file->d_name);
                if(!isBMP(file->d_name)) {
                    continue;
                }
                j++;
            }
            closedir(imageRepertory);
                
            name = calloc(length, sizeof(char));
            if (name == NULL) {
                exit(0);
            }
            strcat(name, "Output/");
            strcat(name, file->d_name);
        }


    }

    //ouvrir le fichier image selection
    activeFile = fopen(name, "rb+");
    if(activeFile == NULL) {
        printf("Erreur dans l'ouverture du fichier !");
        exit(0);
    }
    printf("%s opened!\n", name);
    return activeFile;
} 

void resizeInterface() {

}
void rognerInterface() {
    
}
void affichageASCIIInterface(Image* image) {
    printf("Voici l'image affichée en ASCII(Attention a la taille de l'image)\n");
    printASCII(image);
    printf("\n");
}
void BlackAndWhiteInterface() {
    
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

void steganographieInterface(Image* image) {
    int choice = 0;
    char message[1000]; //1000 Suffisant ? Utiliser un scanf("%1000s") ?

    do  {
        printf("Bienvenue dans le module steganoraphie, que voulez vous faire :\n");
        printf("    1 - Essayer de lire un message dans l'image\n");
        //printf("  2 - Essayer de lire un message dans l'image et enregistrer le msg dans un fichier txt");
        printf("    2 - Cacher un message entre dans la console dans l'image (Ecrasera un potentiel message deja cache)\n");
        //printf("    3 - Cacher un message au prelablement enregistre dans un fichier dans l'image (Ecrasera un potentiel message deja cache)\n");
        printf("    3 - Revenir au menu principal.\n");
        //Sécuriser
        scanf("%d", &choice);
    } while(choice<1 || choice>3);
    switch (choice)
    {
    case 1:
        printf("Le message suivant a ete trouve :\n");
        printf("%s", steganoReading(image));
        printf("\n");
        break;
    case 2:
        printf("Quel msg voulez vous cacher ?");
        scanf("%s", message);
        steganoWriting(image, message);
        printf("Le message a ete inscrit dans l'image.\n");
        break;
    }
}

void saveImageInterface(Image* image) {
    //Déclaration
    char filename[100];
    char finalFilename[100] = {'O', 'u', 't', 'p', 'u', 't', '/', '\0'};
    FILE* writingFile = NULL;

    //Corps de la fonction
    do {
        printf("Comment voulez-vous vous nommer votre image ?\n");

        //    /!\ A SECURISER  /!/
        scanf("%s", filename);
    } while(!isFilenameValid(filename));
    strcat(finalFilename, filename);
    strcat(finalFilename, ".bmp");

    
    writingFile = fopen(finalFilename, "wb+");
 
    if(writingFile == NULL) {
        printf("Erreur dans l'ouverture du fichier !");
        exit(0);
    }

    writeFileFromImage(writingFile, image);
    fclose(writingFile);
    printf("Votre image a ete sauvegarde dans le fichier output/%s.bmp\n", filename);
}

void changeImageInterface(FILE* activeFile, Image* img) {
    //Déclaration
    char input = ' ';
    activeFile = NULL;

    
    //corps de la fonction
    do  {
        printf("Etes-vous sur de vouloir changer d'image ? Vos modifications non enregistres seront effaces (Y/N) ");
        //Faire vérif input
        scanf("%c", &input);
    } while(input!='y'&& input!='Y' && input!='N' && input!='n');

    if(input=='n' || input == 'N') {
        return;
    }
    printf("Quelle image voulez-vous selectionner ? (1/2/3/...)");
    

    activeFile = fileChoice();
    if(activeFile == NULL) {
        printf("Erreur dans l'ouverture du fichier !");
        exit(0);
    }

    freeImage(img);
    *img = getImageFromFile(activeFile);
    fclose(activeFile); 


}

int choiceImageManipulation() {
    int choice = 0;
    do {
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
    } while(choice<1 || choice>15);

    return choice;

}


