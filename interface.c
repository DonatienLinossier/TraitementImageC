#include <stdlib.h>
#include <stdio.h>
#include "interface.h"
#include "gestionFichierImg.h"
#include "transformations.h"
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void clearBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int fileAlreadyExisting(char* filename) {
    char *repertory = NULL;
    DIR *outputRepertory = NULL;
    struct dirent *file = NULL;
    int changeFilename = 0;


    //recuperer le dossier actuel
    repertory=(char *)malloc(100*sizeof(char));
    if (repertory == NULL) {
        exit(0);
    }

    char *filenameBis;
    filenameBis = strdup(filename);
    getcwd(repertory, 100);
    strcat(filenameBis, ".bmp");


    
    //Se déplacer dans le dossier Image et l'ouvrir
    strcat(repertory, "/Output");
    outputRepertory = opendir(repertory);

    //Enumerer les fichiers dans le dossier image + possibilité de lister les images contenu dans output
    if (outputRepertory)
    {
        printf("\n");
        while ((file = readdir(outputRepertory)) != NULL)
        {
            if(strcmp(filenameBis, file->d_name)==0) {
                int ret;
                int choice = 0;
                do {
                    printf("Ce nom est deja utilise par un autre fichier, voulez-vous changer de nom ou ecraser le fichier possedant le meme nom ? (1: changer / 2: ecraser) :\n");
                    ret = scanf("%1d", &choice);
                    clearBuffer();
                } while(ret!=1 || (choice!= 1 && choice!=2));

                switch(choice) {
                    case 1:
                        changeFilename = 1;
                        break;
                    case 2:
                        changeFilename = 0;
                        break;
                }
            }
        }
        closedir(outputRepertory);
        
    } else {
        exit(0);
    }
    free(filenameBis);
    return changeFilename;


}

//Retourne true si le nom du fichier est valide, false sinon
int isFilenameValid(char* filename) {
    int length = strlen(filename);
    int filenameValid = 1;

    for(int iChar = 0; iChar < length; iChar++) {
        if(filename[iChar]=='\0'|| filename[iChar]=='<' || filename[iChar]=='>' ||
           filename[iChar]==':' || filename[iChar]=='"' || filename[iChar]=='/' ||
           filename[iChar]=='\\'|| filename[iChar]=='|' || filename[iChar]=='&' ||
           filename[iChar]=='?' || filename[iChar]=='*') {
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
    char *repertory = NULL;
    DIR *imageRepertory = NULL;
    struct dirent *file = NULL;
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
    int choice = -1;
    int ret;
    do {
        ret = scanf("%d", &choice);
        clearBuffer();
    } while(choice<1 || choice>iFile || ret!=1);

    if(choice!=iFile) {
        //Si un fichier dans Images a été choisi 
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
        //Si l'utilisateur a preferé une image dans le dossier Output
        printf("\nQuelle image voulez-vous selectionner ? (1/2/3/...)");


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
                if(!isBMP(file->d_name)) {
                    continue;
                }
                printf("    %d: %s\n", iFile, file->d_name);
                iFile++;
            }
            closedir(imageRepertory);
            printf("    %d: Retourner aux images du dossier Images\n", iFile, file->d_name);
        }
        int choice = 0;
        int ret;
        do {
            ret = scanf("%d", &choice);
            clearBuffer();
        } while(choice<1 || choice>iFile || ret!=1);

        if(choice!=iFile) {  //Si un fichier dans Output a été choisi 
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
        } else { //si l'utilisateur veut dans le dossier image
            printf("Quelle image voulez-vous selectionner ? (1/2/3/...)");
            return fileChoice();
        }
    }

        

    //ouvrir le fichier image selectionné 
    activeFile = fopen(name, "rb");
    if(activeFile == NULL) {
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(1);
    }
    printf("Image %s ouverte!\n", name);
    return activeFile;
} 

//Permet de changer quelle partie de l'image sera impactée par les changements
void selectionInterface(Image* image, int *sel){
    int ret, choice;
    printf("Bienvenue dans le module sélection\n");
    printf("Voulez vous sélectionner l'image en entier ?\n");
    printf("    1 - Oui\n");
    printf("    2 - Non\n");
    do {
        ret = scanf("%d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice>2);
    if (choice == 1){
        sel[0]=0;
        sel[1]=0;
        sel[2]=image->dibHeader.width;
        sel[3]=image->dibHeader.height;
    }
    printf("Quelle coordonnée x voulez vous pour le coin haut-gauche de la sélection ?\n");
    do {
        ret = scanf("%d", &sel[0]);
        clearBuffer();
    } while(ret!=1 || sel[0]<0);
    printf("Quelle coordonnée y voulez vous pour le coin haut-gauche de la sélection ?\n");
    do {
        ret = scanf("%d", &sel[1]);
        clearBuffer();
    } while(ret!=1 || sel[1]<0);
    printf("Quelle coordonnée x voulez vous pour le coin bas-droite de la sélection ?\n");
    do {
        ret = scanf("%d", &sel[2]);
        clearBuffer();
    } while(ret!=1 || sel[2]>=image->dibHeader.width);
    printf("Quelle coordonnée y voulez vous pour le coin bas-droite de la sélection ?\n");
    do {
        ret = scanf("%d", &sel[3]);
        clearBuffer();
    } while(ret!=1 || sel[3]>=image->dibHeader.height);
}

void resizeInterface(Image* image) {
    int new_x = 0;
    int new_y = 0;
    int ret;

    printf("\nBienvenue dans le module redimensioner\n");
    printf("Quelle taille x voulez vous donner a l'image ?\n");
    do {
        ret = scanf("%d", &new_x);
        clearBuffer();
    } while(ret!=1 || new_x<=0);

    printf("Quelle taille y voulez vous donner à l'image ?\n");
    do {
        ret = scanf("%d", &new_y);
        clearBuffer();
    } while(ret!=1 || new_y<=0);

    resize(image, new_x, new_y);
}

void rognerInterface(Image *image) {
    printf("\nBienvenue dans le module rogner\n");

    int ret = 0;
    int xStart;
    int xEnd;
    int yStart;
    int yEnd;

    printf("Veuillez entrer les coordonnees de la selection, suivant le format suivant : 'x1 y1 x2 y2'. Tapez '0 0 0 0' puis pour revenir au menu principal\n");
    printf("A titre informatif, votre image est de taille %d H x %d L, veuillez a ne pas depasser ces valeurs pour vos coordonnees \n", image->dibHeader.width, image->dibHeader.height);
    do {
        xStart = -1;
        xEnd = -1;
        yStart = -1;
        yEnd = -1;
        ret = scanf("%d %d %d %d", &xStart, &yStart, &xEnd, &yEnd);
        clearBuffer();
    } while((ret!=4 || xStart < 0 || xEnd <0 || yStart<0 || yEnd<0 || yEnd <= yStart || xEnd <= xStart || xEnd >= image->dibHeader.width || yEnd >= image->dibHeader.height) && !(xStart==0 && yStart == 0 && xEnd == 0 && yEnd == 0));
    //Le while est completer/verifier

    if(xStart==0 && yStart == 0 && xEnd == 0 && yEnd == 0) {
        return;
    }

}



void affichageASCIIInterface(Image* image) {
    printf("\nBienvenue dans le module ASCII. Pour les images de tailles consequentes, nous vous invitons a dezoomer dans le terminal afin de permettre l'affichage de l'image en entier.\n");
    printf("A titre informatif, votre image est de taille %d H x %d L\n", image->dibHeader.width, image->dibHeader.height);
    printf("Une fois pret, veuillez entrer 1 pour afficher l'image en ASCII. Entrez 2 pour revenir au menu principal.\n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==1) {
        printf("Voici l'image affichee en ASCII(Attention a la taille de l'image)\n");
        printASCII(image);
        printf("\n");
    }
}


void grayscaleInterface(Image* image) {
    printf("\nBienvenue dans le module noir et blanc\n");
    printf("Voulez-vous passer l'image en noir et blanc ?\n");
    printf("    1: Oui \n");
    printf("    2: Non, retour au menu principal \n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==1) {
        grayscale(image);
        printf("L'image a bien ete passe en noir et blanc !\n");
    }

}


void rotationInterface(Image* image) { 
    printf("\nBienvenue dans le module de rotation des images\n");
    printf("De combien de degres voulez-vous faire pivoter l'image ?\n");
    printf("    1: 90 \n");
    printf("    2: 180 \n");
    printf("    3: 270 \n");
    printf("    4: Retour au menu principal \n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 4);

    switch (choice)
    {
    case 3:
        rotate_90(image);
    case 2:
        rotate_90(image);
    case 1:
        rotate_90(image);
        break;
    }
    printf("Rotation effectué !\n");
}

void luminositeInterface(Image* image) {
    char answer[4];
    printf(" Vous avez choisi de modifier la luminosité de l'image\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    int ret;
    do {
        ret = scanf("%3s", answer);
        clearBuffer();
    } while(ret!=1);
    if(strcmp(answer, "oui") == 0){
        // fonction à mettre dans le header transformations
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }    
}

void contrasteInterface(Image* image) {
    char answer[4];
    printf(" Vous avez choisi de modifier la contrsaste de l'image\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    int ret;
    do {
        ret = scanf("%3s", answer);
        clearBuffer();
    } while(ret!=1);
    if(strcmp(answer, "oui")  == 0){
        // fonction contraste à mettre dans le header transformations
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    } 
}

void flouInterface(Image* image) {
    printf("\nBienvenue dans le module de floutage des images\n");
    printf("Voulez-vous flouter l'image ?\n");
    printf("    1: Oui \n");
    printf("    2: Non, retour au menu principal \n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==1) {
        blur(image, 10);
        printf("Effet de flou appliqué !\n");
    }
    
}



void binariserInterface(Image* image) {
    printf("\nBienvenue dans le module de binarisation des images\n");
    printf("Voulez-vous binariser l'image ?\n");
    printf("    1: Oui \n");
    printf("    2: Non, retour au menu principal \n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==1) {
        binary(image);
        printf("Image binarisee !\n");
    }
}

void inverserCouleursInterface(Image* image) {
    printf("\nBienvenue dans le module d'inversion des couleurs\n");
    printf("Voulez-vous inverser les couleurs de l'image ?\n");
    printf("    1: Oui \n");
    printf("    2: Non, retour au menu principal \n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==1) {
        reverse_image(image);
         printf("Couleurs de l'image inversees !\n");
    }  
}

void symetrieInterface(Image* image) {
    
    printf("\nBienvenue dans le module symetrie, quelle symetrie voulez-vous executer ?\n");
    printf("    1: Horizontale \n");
    printf("    2: Verticale \n");
    printf("    3: Retour au menu principal\n");
    int ret;
    int choice = 0;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 3);
    switch (choice)
    {
    case 1:
        symmetry_x(image);
        printf("Symetrie effectuee!\n");
        break;

    case 2:
        symmetry_y(image);
        printf("Symetrie effectuee!\n");
        break;
    }
    
}

void steganographieInterface(Image* image) {
    int choice = 0;
    char message[1000]; //1000 Suffisant ? Utiliser un scanf("%1000s") ?
    int ret;
    do  {
        printf("\nBienvenue dans le module steganoraphie, que voulez vous faire :\n");
        printf("    1 - Essayer de lire un message dans l'image\n");
        //printf("  2 - Essayer de lire un message dans l'image et enregistrer le msg dans un fichier txt");
        printf("    2 - Cacher un message entre dans la console dans l'image (Ecrasera un potentiel message deja cache)\n");
        //printf("    3 - Cacher un message au prelablement enregistre dans un fichier dans l'image (Ecrasera un potentiel message deja cache)\n");
        printf("    3 - Revenir au menu principal.\n");
        //Sécuriser
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(choice<1 || choice>3 || ret!=1);
    switch (choice)
    {
    case 1:
        printf("Le message suivant a ete trouve :\n");
        printf("%s", steganoReading(image));
        printf("\n");
        break;
    case 2:
        printf("Quel msg voulez vous cacher ? (Veuillez remplacer les espaces par des underscores)\n");
        int ret;
        do {
            ret = scanf("%s", message);
            clearBuffer();
        } while(ret!=1);
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
        printf("\nComment voulez-vous vous nommer votre image ?\n");

        //    /!\ A SECURISER  /!/
        scanf("%90s", filename);
        clearBuffer();
    } while(!isFilenameValid(filename) || fileAlreadyExisting(filename));
    strcat(finalFilename, filename);
    strcat(finalFilename, ".bmp");

    
    writingFile = fopen(finalFilename, "wb+");
 
    if(writingFile == NULL) {
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(1);
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
        printf("\nEtes-vous sur de vouloir changer d'image ? Vos modifications non enregistres seront effaces (Y/N) ");
        //Faire vérif input
        scanf("%c", &input);
        clearBuffer();
    } while(input!='y'&& input!='Y' && input!='N' && input!='n');

    if(input=='n' || input == 'N') {
        return;
    }
    printf("Quelle image voulez-vous selectionner ? (1/2/3/...)");
    

    activeFile = fileChoice();
    if(activeFile == NULL) {
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(1);
    }

    freeImage(img);
    
    *img = getImageFromFile(activeFile);
    fclose(activeFile); 


}

int choiceImageManipulation() {
    int choice = 0;
    int ret;
    do {
        printf("\n\nQue voulez vous faire ?\n");
        printf("     1 - Redimensioner\n");
        printf("     2 - Modifier la sélection\n");
        printf("     3 - Rogner\n");
        printf("     4 - Afficher l'image en ASCII\n");
        printf("     5 - Passer l'image en noir et blanc\n");
        printf("     6 - Rotation\n");
        printf("     7 - Changer la luminosite\n");
        printf("     8 - Changer le contraste\n");
        printf("     9 - Flouter l'image\n");
        printf("     10 - Binariser l'image\n");
        printf("    11 - Inverser les couleurs\n");
        printf("    12 - Effectuer une symetrie\n");
        printf("    13 - Steganographie (WIP)\n");
        printf("    14 - Enregistrer l'image\n");
        printf("    15 - Changer d'image (Abandonne les modfications)\n");
        printf("    16 - Fermer le programme (Abandonne les modifications)\n");
        
        ret = scanf("%2d", &choice);
        clearBuffer();
    } while(choice<1 || choice>16 || ret!=1);

    return choice;
}


