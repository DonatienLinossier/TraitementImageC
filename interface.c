#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h> //Pas nativement sur windows ? 
 // les fonctions FindFirst / FindNext / FindClose peuvent remplacer opendir / readdir / closedir (pas besoin si ça marche sur linux) 
#include <string.h>


/* Points d'attention :
    - Ouverture fichier. Le programme s'est arrété une fois en voulant ouvrir une image qui venait d'etre crée.
    - La steganographie ne marche pas tout le temps ?    

    if (activeFile == NULL){
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(1);
    }
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
    //Image image = getImageFromFile(activeFile);



} 

void resizeInterface() {}

/*void redimensionerInterface(FILE* activeFile, float factor ) {
    // facteur de redimension = ratio en pourcentage
    printf("Redimensionner l'image revient à choisir un pourcentage de redimension.\n");
    printf("De quel facteur voulez-vous redimensionner l'image ?\n");
    scanf("%f", &factor);
    printf("Votre image sera redimensioner de %f %", factor);
    redimensionner(&activeFile, factor);
}*/



/*void rognerInterface(FILE* activeFile) {
    int option1;
    printf("Quel est le format choisi pour rogner votre image ?");
    printf("    1 - rapport 1:1\n");
    printf("    2 - rapport 4:3\n");
    printf("    3 - rapport 16:9\n");
    scanf("%d", &option1);

    int option2=0;

    while(option2!=4){
        switch(option1){
            case 1:
                int mesure1 = 1080;
                printf("Vous avez choisi le format carré, la longueur et largeur sont donc égales. \n");
                rogner(&activeFile, mesure1, mesure1);
                break;
            case 2 :
            int longueur;
            int largeur;
                printf("Vous avez choisi le format quatre tiers\n");
                //format 4:3 est de 1.33:1 de résolution
                largeur = Image.DibHeader.width;
                longueur = 1.33*largeur;
                rogner(&activeFile, largeur, longueur);
                break;
            case 3:
                int longueur1, largeur1;
                printf("Vous avez choisi le format large\n");
                //format 4:3 est de 1.77:1 de résolution
                largeur1 = Image.DibHeader.width;
                longueur1 = 1.77*largeur;
                rogner(&activeFile, largeur1, longueur1);
                break;
        }
    }

}*/



void affichageASCIIInterface(Image* image) {
    printf("Voici l'image affichée en ASCII(Attention a la taille de l'image)\n");
    printASCII(image);
    printf("\n");
}


/*void BlackAndWhiteInterface(FILE* activeFile) {
    char answer[4];
    printf("Vous avez choisi de rendre l'image noire et blanche\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui") == 0){
        getP(image.Dibheader , height, width, rgb)
        noir_et_blanc(width, height, pixels[height][width][3]);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }

}*/


void rotationInterface(Image* image) { 
    char answer[4];
    printf("Votre image va effectuer une rotation de 90 degrés \n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui") == 0){
        //rota_90(image);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }
}



void luminositeInterface(Image* image) {
    char answer[4];
    printf(" Vous avez choisi de modifier la luminosité de l'image\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
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
    scanf("%s", answer);
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
    char answer[4];
    int strength; // comment défnir la force
    printf("Votre image va être floutée \n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui") == 0){
        //flou(activeFile, strength);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }
}



void BinariserInterface(Image* image) {
    //binariser
}


void inverserCouleursInterface(Image* image) {
    char answer[4];
    printf(" Vous avez choisi d'inverser les couleurs de l'image.\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui")== 0){
        //inverse_image(image);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }
}


void symetrieInterface(Image* image) {
char answer[4];
    printf(" Vous avez choisi d'effectuer une symétrie\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui") == 0){
        //symetrie_y(image);
        //symetrie_x(image);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }    
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


