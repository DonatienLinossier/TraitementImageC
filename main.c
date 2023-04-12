#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>

// essai

int decodageLittleEndian(int bytes[], int SIZE) {
    int resulttotal = 0;
    for (int i =0; i<SIZE; i++) {
        int nb = bytes[i];
        int result = 0;
        int decomposition[8];
        for(int h=0; h<8; h++) {  
                decomposition[h] = nb%2;  
                result += decomposition[h] * pow(2, h) * pow(2, 8*i);
                nb = nb/2;
        }
        resulttotal+= result;
        
    }
    return resulttotal;
}



typedef struct {
    char type[2];
    int size;
    int createdBy;
    int createdByBis;
    int offset;  
} Header;

void affichageHeader(Header header) {
    printf("Information sur le header : \n");
    printf("    type : %s\n", header.type);
    printf("    Image size : %d\n", header.size);
    printf("    createdBy : %d\n", header.createdBy);
    printf("    createdByBis : %d\n", header.createdByBis);
    printf("    Offset : %d\n", header.offset);
}

typedef struct {
    int sizeHeader;
    int width;
    int height;
    int nbColorplane; //must be 1
    int nbBitByPixel;
    int compressionMethod;
    int imageSize;
    int horizontalResolution;
    int verticalResolution;
    int nbColorInPalette;
    int nbOfImportantColorUsed;
} DibHeader;

void affichageDIBHeader(DibHeader dibHeader) {
    printf("Information sur le header : \n");
    printf("    sizeHeader : %d\n", dibHeader.sizeHeader);
    printf("    Image width : %d\n", dibHeader.width);
    printf("    Image height : %d\n", dibHeader.height);
    printf("    nbColorplane : %d\n", dibHeader.nbColorplane);
    printf("    nbBitByPixel : %d\n", dibHeader.nbBitByPixel);
    printf("    compressionMethod : %d\n", dibHeader.compressionMethod);
    printf("    imageSize : %d\n", dibHeader.imageSize);
    printf("    horizontalResolution : %d\n", dibHeader.horizontalResolution);
    printf("    verticalResolution : %d\n", dibHeader.verticalResolution);
    printf("    nbColorInPalette : %d\n", dibHeader.nbColorInPalette);
    printf("    nbOfImportantColorUsed : %d\n", dibHeader.nbOfImportantColorUsed);
}

typedef struct {
    Header header;
    DibHeader dibHeader;
    int** image;
} Image;





int main()
{

    Image image;
    image.dibHeader.height = 0;
    image.dibHeader.width = 0;

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
        switch(i) {
            case 0: 
                image.header.type[0] = tab[0];
                image.header.type[1] = tab[1];
                break;
            case 1:
                image.header.size = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 2:
                image.header.createdBy = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 3: 
                image.header.createdByBis = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 4:
                image.header.offset = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
        }
        elementsHeaders[i] = tab;

        
        
        printf("\n");
    }
    affichageHeader(image.header);

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
        /*if(i == 1) {
            image.dibHeader.width = decodageLittleEndian(tab, 4);
        } else if (i == 2) {
            image.dibHeader.height = decodageLittleEndian(tab, 4);
        }*/
        switch(i) {
            case 0: 
                image.dibHeader.sizeHeader = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 1:
                image.dibHeader.width = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 2:
                image.dibHeader.height = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 3: 
                image.dibHeader.nbColorplane = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 4:
                image.dibHeader.nbBitByPixel = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 5:
                image.dibHeader.compressionMethod = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 6:
                image.dibHeader.imageSize = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 7:
                image.dibHeader.horizontalResolution = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 8:
                image.dibHeader.verticalResolution = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 9:
                image.dibHeader.nbColorInPalette = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 10:
                image.dibHeader.nbOfImportantColorUsed = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
        }
        
        printf("\n");
    }
    printf("preaff");
    affichageDIBHeader(image.dibHeader);
    printf("WIDTH : %d; HEIGHT : %d", image.dibHeader.width, image.dibHeader.height);


    printf("\nDecodage IMG...");
    int imagee[image.dibHeader.height][image.dibHeader.width][3];
    int i = 0;


    //vérif le décalage, peut etre un pb. Peut etre qu'on ne commence pas au db; pb avec le -1??
    while(caractereActuel != EOF && i!=3*image.dibHeader.width*image.dibHeader.height) {
        caractereActuel = fgetc(fichier);
        int indiceRGB = i%3;
        int indiceWIDTH = (i/3)%image.dibHeader.height;
        int indiceHEIGHT = (i/3)/image.dibHeader.height;
        //printf("%d: [%d][%d][%d] ", i, indiceHEIGHT, indiceWIDTH, indiceRGB);
        imagee[indiceHEIGHT][indiceWIDTH][indiceRGB] = caractereActuel;
        i++;
    }

    printf("Done\n");
    printf("Affichage Finale :\n");
    for(int i = 0; i<image.dibHeader.height; i++) {
        //printf("[120][%d] : (%d, %d, %d) \n", i, imagee[120][i][0], imagee[120][i][1], imagee[120][i][2]);
    }
    return 0;
    
}