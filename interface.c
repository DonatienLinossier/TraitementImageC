#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h> //Pas nativement sur windows ? 
 // les fonctions FindFirst / FindNext / FindClose peuvent remplacer opendir / readdir / closedir (pas besoin si ça marche sur linux) 
#include <string.h>
//faire un header pour les fonctions de transformations 
//#include "transformations.h"

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

//choisir le fichier
int choice;
    printf("Quelle image choisissez vous ? saisir le numéro :\n");
    printf(" 1\n");
    printf(" 2\n");
    printf(" 3\n");
    printf(" 4 \n");
    printf(" 5 \n");
    printf(" 6 \n");
    printf(" 7 \n");
    scanf("%d", &choice);
    return choice;
//récuperer avec un switch case ?


fscanf(activeFile,"%d", &choice);


//returner le fichier après l'avoir lu
//ouverture du fichier
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
    //Image image = getImageFromFile(activeFile);



} 



void redimensionerInterface(FILE* activeFile, float factor ) {
    // facteur de redimension = ratio en pourcentage
    printf("Redimensionner l'image revient à choisir un pourcentage de redimension.\n");
    printf("De quel facteur voulez-vous redimensionner l'image ?\n");
    scanf("%f", &factor);
    printf("Votre image sera redimensioner de %f %", factor);
    redimensionner(&activeFile, factor);
}



void rognerInterface(FILE* activeFile) {
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

}


void affichageASCIIInterface(FILE* activeFile) {
    FILE* activeFile = NULL;
    // ouverture du fichier en lecture/écriture
    activeFile =fopen ("test.txt", "r+");
    if (activeFile == NULL){
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(1);
    }
    fclose(activeFile);
}


void noirEtBlancInterface(FILE* activeFile) {
    char answer[4];
    printf("Vous avez choisi de rendre l'image noire et blanche\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
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

}


void rotationInterface(FILE* activeFile) { 
    char answer[4];
    printf("Votre image va effectuer une rotation de 90 degrés \n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
        rota_90(activeFile);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }
}



void luminositeInterface(FILE* activeFile) {
    char answer[4];
    printf(" Vous avez choisi de modifier la luminosité de l'image\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
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



void contrasteInterface(FILE* activeFile) {
    char answer[4];
    printf(" Vous avez choisi de modifier la contrsaste de l'image\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
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



void flouInterface(FILE* activeFile) {
    char answer[4];
    int strength; // comment défnir la force
    printf("Votre image va être floutée \n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
        flou(activeFile, strength);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }
}



void BinariserInterface(FILE* activeFile) {
      FILE* activeFile = NULL;
    // ouverture du fichier en lecture/écriture
    activeFile =fopen ("test.txt", "rb+");
    if (activeFile == NULL){
        printf("Ouverture du fichier impossible \n");
        printf("code d'erreur = %d n", errno );
        printf("Message d'erreur = %s \n", strerror(errno));
        exit(1);
    }
    fclose(activeFile);
}


void inverserCouleursInterface(FILE* activeFile) {
    char answer[4];
    printf(" Vous avez choisi d'inverser les couleurs de l'image.\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
         inverse_image(activeFile);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }
}


void symetrieInterface(FILE* activeFile) {
char answer[4];
    printf(" Vous avez choisi d'effectuer une symétrie\n");
    printf("Etes-vous sûr de vouloir continuer ? Saisir oui ou non \n");
    scanf("%s", answer);
    if(strcmp(answer, "oui" == 0)){
        symetrie_y(activeFile);
        symetrie_x(activeFile);
    }
    else if(strcmp(answer, "non")==0){
        printf("0\n");
        exit(0);
    }
    else{
        printf("Veuillez répondre avec 'oui' ou 'non' s'il vous plaît.\n");
    }    
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
