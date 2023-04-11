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
    printf("fin function");
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
    Header header;
    int height;
    int width;
    int** image;
} Image;


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




int main()
{

    Image image;
    image.height = 0;
    image.width = 0;

    int caractereActuel = 0;
    FILE* fichier = NULL;

    fichier = fopen("bmp_24.bmp", "rb");
    //fichier = fopen("img.bmp", "rb");


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
    decoderANDgetHeader(fichier, &image);
    affichageHeader(image.header);

    //contient la taille de tous les elements du DIBHeader format BITMAPINFOHEADER. Mais on ne vas décoder que les 3 premiers elements qui semblent commun a tous. 
    //Je ne stocke pas la taille du header, mais cela surement indispensable pour réecrire le fichier 
    int SizeElementsDIBHeaders[11] = {4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    int* elementsDIBHeaders[11];   
    printf("\nDIB HEADER :\n");
    for(int i = 0; i<11; i++) {
        printf("%d : ", i);
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
            case 1:
                image.width = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 2:
                image.height = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
        }
        printf("\n");
    }
    printf("preaff");
    printf("WIDTH : %d; HEIGHT : %d", image.width, image.height);


    printf("\nDecodage IMG...");
    
    int i = 0;



    int imagee[image.height * image.width * 3];
    while(caractereActuel != EOF && i!=3*image.width*image.height) {
        caractereActuel = fgetc(fichier);
        imagee[i] = caractereActuel;
        i++;
    }

    //A vérifier si besoin de &imagee
    image.image = imagee; 

    printf("Done\n");
    printf("Affichage Finale :\n");
    for(int i = 0; i<image.height; i++) {
        //printf("[120][%d] : (%d, %d, %d) \n", i, imagee[120][i][0], imagee[120][i][1], imagee[120][i][2]);
    }
    return 0;
    
}



//-----------------------------------------
// NE LUS PRENDRE EN COMPTE A PARTIR DE LA
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