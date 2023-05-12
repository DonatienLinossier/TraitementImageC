#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"
#include <dirent.h>
#include <string.h>

void bienvenue() {
    printf("Bienvenu sur CYImage\n");
    printf("Pour commencer, quelle image voulez-vous modifier ?");
    
    
    DIR *d;
    struct dirent *dir;
    FILE *fptr;
    char ch;
    char content[100000];

    char *buf;
    buf=(char *)malloc(100*sizeof(char));
    getcwd(buf,100);
    printf("\n %s \n",buf);

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

}



int main()
{
    
    bienvenue();


}