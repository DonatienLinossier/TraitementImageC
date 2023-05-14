#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h> //Pas nativement sur windows
#include <string.h>

void bienvenue() {
    printf("Bienvenu sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ? (1/2/3/...)");
    
    
    DIR *d;
    struct dirent *dir;


    //récupérer repertoire actuel(A faire à CYtech pour etre sur de la compatibilité)
    char *buf;
    buf=(char *)malloc(100*sizeof(char));
    getcwd(buf,100);
    printf("\n %s \n",buf);

    /*
    Stocker tout les noms dans un tableau dynamique de chaine de caractere dynamique
    Afficher tout les fichier .bmp
    Récuperer le choix de l'utilisateur
    Ouvrir l'image(avec le tabl dynamque)
    Choix des opérations sur cette image ...
    */

    d = opendir("/cergy/homee/l/linossierd/Informatique_PreIng1/projet/Images");
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
        	
            printf("%d: %s \n", i, dir->d_name);
            i++;
        }
        closedir(d);
    }
    int choice;
    //Faire les vérifs
    scanf("%d", &choice);


}



int main()
{
    
    bienvenue();


}