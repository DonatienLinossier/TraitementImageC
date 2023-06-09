//Ce fichier contient les differentes fonctions d'interface qui demandent a l'utilisateur les valeurs a utiliser pour le tranformations
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "imageManagement.h"
#include "transformations.h"
#include "interface.h"


#define IMAGE_OUTPUT_DIRECTORY "Images/Output"
#define IMAGE_DIRECTORY "Images"

#define STEGANO_OUTPUT_FILE "Stegano/Output.txt"
#define STEGANO_INPUT_FILE "Stegano/Input.txt"
#define STEGANO_MESSAGE_SIZE_MAX 10001 //Message de 10000 + '\0'

#define FILENAME_SIZE_MAX 256 //255 + '\0'
#define FOLDER_SIZE_MAX 256 //255 + '\0'






//Fonction qui vide le buffer pour les scanf
void clearBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

//Fonction qui verifie si le fichier existe deja
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
    getcwd(repertory, FOLDER_SIZE_MAX);
    strcat(filenameBis, ".bmp");


    
    //Se deplacer dans le dossier Image et l'ouvrir
    strcat(repertory, "/");
    strcat(repertory, IMAGE_OUTPUT_DIRECTORY);
    outputRepertory = opendir(repertory);

    //Enumerer les fichiers dans le dossier image + possibilite de lister les images contenu dans output
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
        printf("Verifiez que le dossier %s existe bien et reessayez", repertory);
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
    return (filename[length-4] == '.' && (filename[length-3] == 'b' || filename[length-3] == 'B' )&& (filename[length-2] == 'm' || filename[length-2] == 'M' ) && (filename[length-1] == 'p' || filename[length-1] == 'P'));
}

//Permet a l'uilisateur de choisir son fichier dans Image dans Images et/ou Output et retourne le fichier ouvert
FILE* fileChoice() {
    FILE* activeFile = NULL;
    char *repertory = NULL;
    DIR *imageRepertory = NULL;
    struct dirent *file = NULL;
    char * name = NULL;


    //recuperer le dossier actuel
    repertory=(char *)malloc(FOLDER_SIZE_MAX*sizeof(char));
    if (repertory == NULL) {
        exit(0);
    }
    getcwd(repertory, FOLDER_SIZE_MAX);


    
    //Se deplacer dans le dossier Image et l'ouvrir
    strcat(repertory, "/");
    strcat(repertory, IMAGE_DIRECTORY);
    imageRepertory = opendir(repertory);

    //Enumerer les fichiers dans le dossier image + possibilite de lister les images contenu dans output
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
        printf("    %d: Ouvrir le dossier %s contenant les images precedement modifiees\n", iFile, IMAGE_OUTPUT_DIRECTORY);
    } else {
        printf("Nous n'avons pas reussi a ouvrir le dossier %s. Veuillez verifier son integrite et qu'il existe bien, puis relancer le programme", repertory);
        exit(0);
    }  


    //recuperer le choix de l'utilisateur
    int choice = -1;
    int ret;
    do {
        ret = scanf("%d", &choice);
        clearBuffer();
    } while(choice<1 || choice>iFile || ret!=1);

    if(choice!=iFile) {
        //Si un fichier dans Images a ete choisi 
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
                
            strcat(name, IMAGE_DIRECTORY);
            strcat(name, "/");
            strcat(name, file->d_name);
        }  else {
            printf("Nous n'avons pas reussi a ouvrir le dossier %s. Veuillez verifier son integrite et qu'il existe bien, puis relancer le programme", repertory);
            exit(0);
        }
    } else {
        //Si l'utilisateur a prefere une image dans le dossier Output
        printf("\nQuelle image voulez-vous selectionner ? (1/2/3/...)");


        repertory=(char *) malloc(FOLDER_SIZE_MAX*sizeof(char));
        if (repertory == NULL) {
            exit(0);
        }
        getcwd(repertory, FOLDER_SIZE_MAX);


        int iFile = 1;
        strcat(repertory, "/");
        strcat(repertory, IMAGE_OUTPUT_DIRECTORY);
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
            printf("    %d: Retourner aux images du dossier %s\n", iFile, IMAGE_DIRECTORY);
        } else {
            printf("Nous n'avons pas reussi a ouvrir le dossier %s. Veuillez verifier son integrite et qu'il existe bien, puis relancer le programme", repertory);
            printf("En attendant, veuillez choisir une image contenu dans %s (parmi les images de base)", IMAGE_DIRECTORY);
            return fileChoice();
        }
        int choice = 0;
        int ret;
        do {
            ret = scanf("%d", &choice);
            clearBuffer();
        } while(choice<1 || choice>iFile || ret!=1);

        if(choice!=iFile) {  //Si un fichier dans Output a ete choisi 
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
                strcat(name, IMAGE_OUTPUT_DIRECTORY);
                strcat(name, "/");
                strcat(name, file->d_name);
                
            } else {
                printf("Nous n'avons pas reussi a ouvrir le dossier %s. Veuillez verifier son integrite et qu'il existe bien, puis relancer le programme", repertory);
                printf("En attendant, veuillez choisir une image contenu dans %s (parmi les images de base)", IMAGE_DIRECTORY);
                return fileChoice();
            }
        } else { //si l'utilisateur veut dans le dossier image
            printf("Quelle image voulez-vous selectionner ? (1/2/3/...)");
            return fileChoice();
        }
    }

        
    //ouvrir le fichier image selectionne 
    activeFile = fopen(name, "rb");
    if(activeFile == NULL) {
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        printf("Nous avons rencontre un probleme en essayant d'ouvrir le fichier %s, veuillez verifier son existence et son integrite, puis relancez le programme.\n", name);
        printf("En attendant, veuillez choisir une autre image :\n");
        return fileChoice();
    }
    printf("%s ouverte!\n", name);
    return activeFile;
} 

//Permet de changer quelle partie de l'image sera impactee par les changements
void selectionInterface(Image* image, int *sel){
    int ret, choice;
    printf("Bienvenue dans le module selection\n");
    printf("Voulez vous selectionner l'image en entier ?\n");
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
    } else if(choice==2) {
        printf("Votre image est de taille %d x %d \n", image->dibHeader.width, image->dibHeader.height );
        printf("Quelle coordonnee x voulez vous pour le coin haut-gauche de la selection ?\n");
        do {
            ret = scanf("%d", &sel[0]);
            clearBuffer();
        } while(ret!=1 || sel[0]<0);
        printf("Quelle coordonnee y voulez vous pour le coin haut-gauche de la selection ?\n");
        do {
            ret = scanf("%d", &sel[1]);
            clearBuffer();
        } while(ret!=1 || sel[1]<0);
        printf("Quelle coordonnee x voulez vous pour le coin bas-droite de la selection ?\n");
        do {
            ret = scanf("%d", &sel[2]);
            clearBuffer();
        } while(ret!=1 || sel[2]>=image->dibHeader.width || sel[2]<= sel[0]);
        printf("Quelle coordonnee y voulez vous pour le coin bas-droite de la selection ?\n");
        do {
            ret = scanf("%d", &sel[3]);
            clearBuffer();
        } while(ret!=1 || sel[3]>=image->dibHeader.height || sel[3]<= sel[1]);
    }
}


//Toutes les fonctions suivantes sont les versions utilisateurs des transformations

void resizeInterface(Image* image, int sel[4]) {
    int new_x = 0;
    int new_y = 0;
    int ret;
    int selection = 0;

    printf("\nBienvenue dans le module redimensioner\n");
    printf("Les differents formats de redimension sont a choisir:\n");
    printf("    1 - 0.5\n");
    printf("    2 - 2\n");
    printf("    3 - 5\n");
    printf("    4 - Taille personnalisee\n");
    printf("    5 - Revenir au menu principal\n");
    do{
        ret=scanf("%d", &selection);
        clearBuffer();
    } while(selection>5 || ret!=1 || selection<1);

    switch(selection) {
        case 1:
            resize(image, image->dibHeader.width*0.5, image->dibHeader.height*0.5);
            break;
        case 2:
            resize(image, image->dibHeader.width*2, image->dibHeader.height*2);
            break;
        case 3:
            resize(image, image->dibHeader.width*5, image->dibHeader.height*5);
            break;
        case 4:
            printf("Quelle taille de redimension x voulez-vous appliquer a l'image ?\n");
            do{
                ret=scanf("%d", &new_x);
                clearBuffer();
            }while(ret!=1 || new_x<=0);

            printf("Quelle taille de redimension y voulez-vous donner a l'image ?\n");
            do{
                ret = scanf("%d", &new_y);
                clearBuffer();
            } while(selection>4 || ret!=1 || selection<1);
            resize(image, new_x, new_y);
            break;
        case 5:
            return;
        }
        
        sel[0]=0;
        sel[1]=0;
        sel[2]=image->dibHeader.width;
        sel[3]=image->dibHeader.height;
}

void cropInterface(Image *image, int sel[4]) {
    crop(image, sel);
    printf("Votre image a bien ete redimensionnee, elle fait maintenant %d x %d pixels\n", image->dibHeader.width, image->dibHeader.height);
        
    sel[0]=0;
    sel[1]=0;
    sel[2]=image->dibHeader.width;
    sel[3]=image->dibHeader.height;
}

void displayInterfaceASCII(Image* image) {
    printf("Bienvenue dans le module ASCII. Pour les images de tailles consequentes, nous vous invitons a dezoomer dans le terminal afin de permettre l'affichage de l'image en entier.\n");
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

void grayscaleInterface(Image* image, int sel[4]) {
        grayscale(image,sel);
        printf("L'image a bien ete passe en noir et blanc\n");
}

void rotateInterface(Image* image, int sel[4]) { 
    printf("De combien de degres voulez-vous faire pivoter l'image ?\n");
    printf("    1 - 90 \n");
    printf("    2 - 180 \n");
    printf("    3 - 270 \n");
    printf("    4 - Retour au menu principal \n");
    int choice = 0;
    int ret;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 4);

    switch (choice)
    {
    case 4:
        return;
    case 3:
        rotate_90(image);
    case 2:
        rotate_90(image);
    case 1:
        rotate_90(image);
        break;
    }
    sel[0]=0;
    sel[1]=0;
    sel[2]=image->dibHeader.width;
    sel[3]=image->dibHeader.height;

    printf("Rotation effectuee\n");
}

void brightnessInterface(Image* image, int sel[4]) {
    int percent = 0;
    printf("De quel pourcentage voulez vous changer la luminosite ?\nMoins de 100 baisse la luminosite, plus de 100 l'augmente.\n");
    int ret;
    do {
        ret = scanf("%d", &percent);
        clearBuffer();
    }while(ret!=1 || percent < 0);
    brightness(image,percent,sel);
    printf("La luminosite de l'image a bien ete modifiee\n");
}

void contrastInterface(Image* image, int sel[4]) {
    printf("De quel pourcentage voulez vous changer la luminosite ?\nMoins de 100 baisse la luminosite, plus de 100 l'augmente.\n");
    int ret,percent;
    do {
        ret = scanf("%d", &percent);
        clearBuffer();
    }while(ret!=1 || percent < 0);
    contrast(image,sel);
    printf("La luminosite de l'image a bien ete modifiee\n");
}

void blurInterface(Image* image, int sel[4]) {
    printf("Par quel facteur voulez vous flouter l'image ?\nValeur minimum 1, valeur maximum 10. Entrez 0 pour revenir au menu principal.\n");
    int choice = -1;
    int ret;
    do {
        ret = scanf("%d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<0 || choice>10);

    if(choice==0) {
        return;
    } else {
        blur(image, choice * 2 - 1 , sel);
        printf("Effet de flou applique !\n");
    }
    
}

void binaryInterface(Image* image, int sel [4]) {
    binary(image, sel);
    printf("Votre image est bien binarisee\n");

}

void reverseColorsInterface(Image* image, int sel[4]) {
    reverse_colors(image, sel);
    printf("Couleurs de l'image inversees\n");
}

void symmetryInterface(Image* image, int sel[4]) {
    printf("\nBienvenue dans le module symetrie, quelle symetrie voulez-vous executer ?\n");
    printf("    1 - Horizontale \n");
    printf("    2 - Verticale \n");
    printf("    3 - Retour au menu principal\n");
    int ret;
    int choice = 0;
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 3);
    switch (choice)
    {
    case 1:
        symmetry_x(image, sel);
        printf("Symetrie effectuee!\n");
        break;

    case 2:
        symmetry_y(image, sel);
        printf("Symetrie effectuee!\n");
        break;
    }
    
}

void steganographyInterface(Image* image) {
    int choice = 0;
    char message[STEGANO_MESSAGE_SIZE_MAX];
    int ret;
    do  {
        printf("\nBienvenue dans le module steganoraphie, que voulez vous faire :\n");
        printf("    1 - Essayer de lire un message dans l'image\n");
        printf("    2 - Essayer de lire un message dans l'image et enregistrer le msg dans un fichier txt\n");
        printf("    3 - Cacher un message entre dans la console dans l'image (Ecrasera un potentiel message deja cache)\n");
        printf("    4 - Cacher un message au prelablement enregistre dans un fichier dans l'image (Ecrasera un potentiel message deja cache)\n");
        printf("    5 - Revenir au menu principal.\n");
        //Securiser
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(choice<1 || choice>5 || ret!=1);

    FILE* file = NULL;
    switch (choice)
    {
    case 1:
        printf("Le message suivant a ete trouve :\n");
        printf("%s", steganoReading(image));
        printf("\n");
        break;
    case 2:
        file = fopen(STEGANO_OUTPUT_FILE, "w");
        if(file==NULL) {
            printf("Nous n'avons pas reussi a ouvrir le fichier. Verifiez que l'image %s exite bien et reessayez.\n", STEGANO_OUTPUT_FILE);
        }
        fprintf(file, "%s", steganoReading(image));
        fclose(file);
        printf("Le message a ete sauvegarde dans le fichier Stegano/Output.txt\n");
        break;
    case 3:
        printf("Quel message voulez vous cacher ? (Veuillez remplacer les espaces par des underscores)\n");
        int ret;
        do {
            ret = scanf("%10000s", message);
            clearBuffer();
        } while(ret!=1);
        steganoWriting(image, message);
        printf("Le message a ete inscrit dans l'image.\n");
        break;
    case 4:
        file = fopen(STEGANO_INPUT_FILE, "r");
        if(file==NULL) {
            printf("Aucun fichier Stegano/Input.txt n'a ete trouve. Verifier que le fichier %s existe bien et retentez.", STEGANO_INPUT_FILE);
            break;
        }
        fscanf(file,"%10000s", message);
        steganoWriting(image, message);
        printf("Le message a ete inscrit dans l'image.\n");
        fclose(file);
        break;
    }
}

//Sauvegarde l'image en .bmp
void saveImageInterface(Image* image) {
    //Declaration
    char filename[FILENAME_SIZE_MAX];
    char finalFilename[FILENAME_SIZE_MAX] = IMAGE_OUTPUT_DIRECTORY;
    strcat(finalFilename, "/\0");
    FILE* writingFile = NULL;
    int ret = 0;
    //Corps de la fonction
    do {
        printf("\nComment voulez-vous vous nommer votre image ?\n");
        ret = scanf("%255s", filename);
        clearBuffer();
    } while(!isFilenameValid(filename) || fileAlreadyExisting(filename) || ret!=1);
    strcat(finalFilename, filename);
    strcat(finalFilename, ".bmp");

    
    writingFile = fopen(finalFilename, "wb+");
 
    if(writingFile == NULL) {
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        printf("Nous n'avons pas reussi a sauvegarder votre image, veuillez reesayer plus tard.\n");
    }

    writeFileFromImage(writingFile, image);
    fclose(writingFile);
    printf("Votre image a ete sauvegarde dans le fichier output/%s.bmp\n", filename);
}


//Permet de modifier une autre image
void changeImageInterface(FILE* activeFile, Image* img, int sel[4]) {
    //Declaration
    char input = ' ';
    activeFile = NULL;
    int ret = 0;

    //corps de la fonction
    do  {
        printf("\nEtes-vous sur de vouloir changer d'image ? Vos modifications non enregistrees seront effacees (Y/N) ");
        ret = scanf("%c", &input);
        clearBuffer();
    } while((input!='y'&& input!='Y' && input!='N' && input!='n') || ret!= 1);

    if(input=='n' || input == 'N') {
        return;
    }
    printf("Quelle image voulez-vous selectionner ? (1/2/3/...)");
    

    activeFile = fileChoice();
    if(activeFile == NULL) {
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d \n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        printf("Veuillez choisir un autre fichier ou reessayer plus tard\n");
        return;
    }

    freeImage(img);
    
    *img = getImageFromFile(activeFile);
    sel[0]=0;
    sel[1]=0;
    sel[2]=img->dibHeader.width;
    sel[3]=img->dibHeader.height;
    fclose(activeFile); 


}

void addImageToImageTmp(Image* img, char* filename) {
    FILE* file = NULL;
    file = fopen(filename, "wb");
    if(file==NULL) {
        return;
    }
    writeFileFromImage(file, img);
    fclose(file);
}

void getLastImage(Image *img, char* filename) {
    FILE* file = NULL;
    file = fopen(filename, "rb");
    if(file==NULL) {
        return;
    }
    *img = getImageFromFile(file);

    fclose(file);
}

void undoInterface(Image *img, char* filename, int sel[4]) {
    int choice = 0;
    int ret = 0;
    printf("Etes-vous sur de vouloir annuler votre derniere modification ? \n");
    printf("    1 - Oui\n");
    printf("    2 - Non, revenir au menu principal\n");
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==1) {
        getLastImage(img, filename);
        sel[0]=0;
        sel[1]=0;
        sel[2]=img->dibHeader.width;
        sel[3]=img->dibHeader.height;
        printf("Image precedente restauree\n");
    }



}



void exitInterface(int* choiceRetour){
    if(choiceRetour==NULL) {
        return;
    }
    int choice = 0;
    int ret = 0;
    printf("Etes-vous sur de vouloir fermer le programme et d'abandonner les modifications apportees ? (N'oubliez pas d'enregistrer votre image avant de quitter.) : \n");
    printf("    1 - Oui\n");
    printf("    2 - Non\n");
    do {
        ret = scanf("%1d", &choice);
        clearBuffer();
    } while(ret!=1 || choice<1 || choice > 2);

    if(choice==2) {
        *choiceRetour = 0;
    }
}



//Affiche le menu principal, avec toutes les options possibles
int choiceImageManipulation() {
    int choice = 0;
    int ret;
    do {
        printf("\n\nQue voulez vous faire ?\n");
        printf("     1 - Redimensioner\n");
        printf("     2 - Modifier la selection\n");
        printf("     3 - Rogner\n");
        printf("     4 - Afficher l'image en ASCII\n");
        printf("     5 - Passer l'image en noir et blanc\n");
        printf("     6 - Rotation\n");
        printf("     7 - Changer la luminosite\n");
        printf("     8 - Changer le contraste\n");
        printf("     9 - Flouter l'image\n");
        printf("    10 - Binariser l'image\n");
        printf("    11 - Inverser les couleurs\n");
        printf("    12 - Effectuer une symetrie\n");
        printf("    13 - Steganographie\n");
        printf("    14 - Enregistrer l'image\n");
        printf("    15 - Changer d'image (Abandonne les modfications)\n");
        printf("    16 - Annuler le dernier changement. (Applicable 1 fois).\n");
        printf("    17 - Fermer le programme (Abandonne les modifications)\n");
        
        ret = scanf("%2d", &choice);
        clearBuffer();
    } while(choice<1 || choice>17 || ret!=1);

    return choice;
}
