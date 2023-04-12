#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>




typedef struct {
    char* type[2];
    int* size[4];
    int* createdBy[2];
    int* createdByBis[2];
    int* offset[4];  
} header;


int decodageLittleEndian(int bytes[], int SIZE) {
    int resulttotal = 0;
    for (int i =0; i<SIZE; i++) {
        int nb = bytes[i];
        int result = 0;
        int decomposition[8];
        //printf("\nDécomposition de %d = ",  bytes[i]);
        for(int h=0; h<8; h++) {  
                decomposition[h] = nb%2;  
                //printf("%d", decomposition[h]);
                result += decomposition[h] * pow(2, h) * pow(2, 8*i);
                nb = nb/2;
        }
        //printf(" - %d devient : %d;", bytes[i], result);
        resulttotal+= result;
        //printf("\n %d * 2**8*%d, %d \n", bytes[i], i, bytes[i] * pow(2, 8*i));
        
    }
    //printf("\nResult final : %d\n", resulttotal);
    return resulttotal;
}


int main()
{
    int WIDTH;
    int HEIGHT;
    int caractereActuel = 0;
    FILE* fichier = NULL;

    fichier = fopen("bmp_24.bmp", "rb");
    //fichier = fopen("img.bmp", "rb");


    int SizeElementsHeaders[5] = {2, 4, 2, 2, 4};
    int* elementsHeaders[5];   

    printf("HEADER :\n");
    for(int i =0; i<5; i++) {
        int tab[SizeElementsHeaders[i]];
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(fichier); // On lit le caractère
            printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        elementsHeaders[i] = tab;
        
        printf("\n");
    }


    int SizeElementsDIBHeaders[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    int* elementsDIBHeaders[11];   
    printf("\nDIB HEADER :\n");
    for(int i = 0; i<11; i++) {
        int tab[SizeElementsDIBHeaders[i]];
        for(int j = 0; j < SizeElementsDIBHeaders[i]; j++) {
            //printf("\nj : %d \n", j);
            caractereActuel = fgetc(fichier); // On lit le caractère
            printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        elementsDIBHeaders[i] = tab;
        if(i == 1) {
            WIDTH = decodageLittleEndian(tab, 4);
        } else if (i == 2) {
            HEIGHT = decodageLittleEndian(tab, 4);
        }
        
        printf("\n");
    }

    printf("WIDTH : %d; HEIGHT : %d", WIDTH, HEIGHT);


    printf("\nDecodage IMG...");
    int image[HEIGHT][WIDTH][3];
    int i = 0;


    //vérif le décalage, peut etre un pb. Peut etre qu'on ne commence pas au db; pb avec le -1??
    while(caractereActuel != EOF && i!=3*600*800) {
        caractereActuel = fgetc(fichier);
        int indiceRGB = i%3;
        int indiceWIDTH = (i/3)%WIDTH;
        int indiceHEIGHT = (i/3)/WIDTH;
        printf("%d: [%d][%d][%d] ", i, indiceHEIGHT, indiceWIDTH, indiceRGB);
        image[indiceHEIGHT][indiceWIDTH][indiceRGB] = caractereActuel;
        i++;
    }

    printf("Done\n");
    printf("Affichage Finale :\n");
    for(int i = 0; i<WIDTH; i++) {
       // printf("[120][%d] : (%d, %d, %d) \n", i, image[120][i][0], image[120][i][1], image[120][i][2]);
    }
    return 0;
    
}