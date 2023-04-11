#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>

//Permet de décoder les infos dans les headers stockées en little Endian(La casi totalité des infos).
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


//Le header du fichier
typedef struct {
    char type[2];
    int size;
    int createdBy;
    int createdByBis;
    int offset;  
} Header;


//Afficher les infos du headers
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


int getP(Image* image, int width, int height, int rgb) {
    printf("%d", image->image[(height*(image->dibHeader.width*3) + width *3 + rgb)]);
    return (int) image->image[(height*(image->dibHeader.width*3) + width *3 + rgb)];
}



Header decoderANDgetHeader(FILE* fichier, Image* image) {
    int caractereActuel;
    int SizeElementsHeaders[5] = {2, 4, 2, 2, 4}; 
    printf("HEADER :\n");
    for(int i =0; i<5; i++) {
        int tab[SizeElementsHeaders[i]];
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(fichier); //On lit le caractère
            printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        switch(i) {
            case 0: 
                image->header.type[0] = tab[0];
                image->header.type[1] = tab[1];
                break;
            case 1:
                image->header.size = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 2:
                image->header.createdBy = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 3: 
                image->header.createdByBis = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
            case 4:
                image->header.offset = decodageLittleEndian(tab, SizeElementsHeaders[i]);
                break;
        }
        printf("\n");
    }
}


//ATTENTION, A MODIFIER EN FONCTION DU FORMAT
Header decoderANDgetDIBHeader(FILE* fichier, Image* image) {
    int caractereActuel;
    int SizeElementsDIBHeaders[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    int* elementsDIBHeaders[11];   
    //printf("\nDIB HEADER :\n");
    for(int i = 0; i<11; i++) {
        //printf("%d : ", i);
        int tab[SizeElementsDIBHeaders[i]];
        for(int j = 0; j < SizeElementsDIBHeaders[i]; j++) {
            //printf("\nj : %d \n", j);
            caractereActuel = fgetc(fichier); // On lit le caractère
            //printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        elementsDIBHeaders[i] = tab;
        switch(i) {
            case 0:
                image->dibHeader.sizeHeader = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
            case 1:
                image->dibHeader.width = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 2:
                image->dibHeader.height = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 3:
                image->dibHeader.nbColorplane = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 4:
                image->dibHeader.nbBitByPixel = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 5:
                image->dibHeader.compressionMethod = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 6:
                image->dibHeader.imageSize = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 7:
                image->dibHeader.horizontalResolution = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 8:
                image->dibHeader.verticalResolution = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 9:
                image->dibHeader.nbColorInPalette = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 10:
                image->dibHeader.nbOfImportantColorUsed = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
        }
        //printf("\n");
    }
}




int main()
{

    Image image;

    int caractereActuel = 0;
    FILE* fichier = NULL;

    fichier = fopen("bmp_24.bmp", "rb");
    //fichier = fopen("img.bmp", "rb");



    decoderANDgetHeader(fichier, &image);
    affichageHeader(image.header);

    decoderANDgetDIBHeader(fichier, &image);
    affichageDIBHeader(image.dibHeader);
    
    int i = 0;



    int imagee[image.dibHeader.height * image.dibHeader.width * 3];
    while(caractereActuel != EOF && i!=3*image.dibHeader.width*image.dibHeader.height) {
        caractereActuel = fgetc(fichier);
        imagee[i] = caractereActuel;
        i++;
    }
    int j=i;
    //A vérifier si besoin de &imagee
    image.image = &imagee; 
    image.image[0] = 0;

    for(int i =0; i<image.dibHeader.height * image.dibHeader.width * 3 && i<j; i++) {
        printf("%d ", image.image[i]);
    }
    printf("Done\n");
    printf("Affichage Finale :\n");
    return 0;
    
}












//-----------------------------------------
// NE PLUS PRENDRE EN COMPTE A PARTIR DE LA
//-----------------------------------------




/*PAS UTILISé CAR TROP GALERE AVEC LES DIFFENTES FORMATS EXISTANTS. ON UTILISERA SUREMENT QUE LE TAILLE DU HEADER, AINSI QUE LA LARGEUR ET LONGUEUR DE L'IMAGE
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
}*/





    /*ON NE PASSE PLUS PAR UN TAB 3D, donc obsolete
    int imagee[image.height][image.width][3];
    //vérif le décalage, peut etre un pb. Peut etre qu'on ne commence pas au db; pb avec le -1??
    while(caractereActuel != EOF && i!=3*image.width*image.height) {
        caractereActuel = fgetc(fichier);
        int indiceRGB = i%3;
        int indiceWIDTH = (i/3)%image.height;
        int indiceHEIGHT = (i/3)/image.height;
        //printf("%d: [%d][%d][%d] ", i, indiceHEIGHT, indiceWIDTH, indiceRGB);
        imagee[indiceHEIGHT][indiceWIDTH][indiceRGB] = caractereActuel;
        i++;
    }*/



        /*int SizeElementsHeaders[5] = {2, 4, 2, 2, 4};
    int* elementsHeaders[5];   

    printf("HEADER :\n");
    for(int i =0; i<5; i++) {
        int tab[SizeElementsHeaders[i]];
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(fichier); //On lit le caractère
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
    */



    //contient la taille de tous les elements du DIBHeader format BITMAPINFOHEADER. Mais on ne vas décoder que les 3 premiers elements qui semblent commun a tous. 
    //Je ne stocke pas la taille du header, mais cela surement indispensable pour réecrire le fichier 
    /*int SizeElementsDIBHeaders[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    int* elementsDIBHeaders[11];   
    //printf("\nDIB HEADER :\n");
    for(int i = 0; i<11; i++) {
        //printf("%d : ", i);
        int tab[SizeElementsDIBHeaders[i]];
        for(int j = 0; j < SizeElementsDIBHeaders[i]; j++) {
            //printf("\nj : %d \n", j);
            caractereActuel = fgetc(fichier); // On lit le caractère
            //printf("%d ", caractereActuel);
            tab[j] = caractereActuel;
        }
        elementsDIBHeaders[i] = tab;
        switch(i) {
            case 0:
                image.dibHeader.sizeHeader = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
            case 1:
                image.width = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 2:
                image.dibHeader.height = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 3:
                image.dibHeader.nbColorplane = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 4:
                image.dibHeader.nbBitByPixel = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 5:
                image.dibHeader.compressionMethod = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 6:
                image.dibHeader.imageSize = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 7:
                image.dibHeader.horizontalResolution = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 8:
                image.dibHeader.verticalResolution = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 9:
                image.dibHeader.nbColorInPalette = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 10:
                image.dibHeader.nbOfImportantColorUsed = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
        }
        printf("\n");
    }*/