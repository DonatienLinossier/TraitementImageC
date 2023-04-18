#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gestionFichierImg.h"


/*
- - - STRUCTURE DE IMAGE - - -
    Image---->Header-----------|- type
      |                        |- size
      |                        |- createdBy
      |                        |- CreatedByBis
      |                        |- Offset
      |                   
      |------>DIBHeader--------|- SizeHeadergit 
      |                        |- width
      |                        |- height
      |                        |- ... 
      |                        |- Le reste du DIBHeader comporte des informations non utiles aux projets et qui dépendent du type de Header de l'image, voir directement dans la fonction de decodage du DIBHeader concerné si besoin
      | 
      |------>Tableau 1D avec l'ensemble des valeurs rgb de l'image (Utiliser getP() pour accéder aux pixels, et setP() pour les modifier.)
      |------>Padding(Interne à l'image, ne sert pas en dehors)






Recap:
|-------------------------------------------------------------|
|fonction                | Interne/Externe | Etat | accesible |
|-------------------------------------------------------------|
|                          DECODAGE                           |
|getImageFromFile        | Externe         | OK   |    OUI    | //Corrigé !
|decoderANDgetHeader     | Interne         | OK   |    X      |
|decoderANDgetDIBHeader  | Interne         | OK   |    X      | //Corrigé !
|getImg                  | Interne         | OK   |    X      | //Corrigé !
|decodageLittleEndian    | Interne         | OK   |    X      |
|                                                             |
|                          ENCODAGE                           |
|writeFileFromImage      | Externe         | OK   |    OUI    |
|encoderHeader           | Interne         | OK   |    X      |
|encoderDIBHeader        | Interne         | OK   |    X      |
|ImgToFile               | Interne         | OK   |    X      |
|encodageLittleEndian    | Interne         | OK   |    X      |
|                                                             |
|                         AFFICHAGE                           |
|afficherASCII           | Externe         | OK   |    OUI    | //Corrigé !
|                                                             |
|                    OPERATION SUR IMAGE                      |
|getP                    | Externe         | OK   |    OUI    | //Corrigé !
|setP                    | Externe         | OK   |    OUI    | //Corrigé !
|copy                    | Externe         | OK   |    OUI    | //Corrigé !
|freeImage               | Externe         | OK   |    OUI    | //Corrigé !
|rogner                  | Externe         | OK   |    OUI    | //ajouter des verifs pour eviter erreur de segmentation
|redimensioner           | Externe         | BOF  |    OUI    | //Aproximatif, à verif
|ClearAndRedimensioner   | Externe         | OBS  |    NON    | //Remplacé par redimensioner, a suppr
|                                                             |
|                           DEBUG                             |
|affichageHeader         | Interne         | OK   |    X      |
|affichageDIBHeader      | Interne         | OK   |    X      |
|-------------------------------------------------------------|

*/





//Permet de décoder les infos dans les headers stockées en little Endian(La casi totalité des infos).
int decodageLittleEndian(int* bytes, int SIZE) {
    int resulttotal = 0;
    for (int i =0; i<SIZE; i++) {
        int nb = bytes[i];
        int result = 0;
        int decomposition[8];
        for(int h=0; h<8; h++) {  
                decomposition[h] = nb%2;  
                //printf("%d ",decomposition[h] );
                result += decomposition[h] * pow(2, h) * pow(2, 8*i);
                nb = nb/2;
        }
        resulttotal+= result;
        
    } 
    return resulttotal;
}

int encodageLittleEndian(int* bytesResult, int SIZE, int value) {
    int i =0;
    int* binaire = calloc(SIZE*8, sizeof(int));
    //printf("]\n[Binaire:");
    while(i < SIZE*8) {
        binaire[i] = value%2;
        //printf("%d ",binaire[i]);
        value = value/2;
        i++;
    }

    for(int i=0; i<SIZE; i++) {
        int result = 0;
        for (int j=0; j<8; j++) {
            result += binaire[i*8 +j] * pow(2, j);
        }
        bytesResult[i] = result;
        //printf("(Total %d : Character %c) ",  bytesResult[i], bytesResult[i]);
    }
   

}

void encoderHeader(FILE* fichier, Image* image) {
    int SizeElementsHeaders[5] = {2, 4, 2, 2, 4}; 
    int size;   
    for(int i =0; i<5; i++) {
        size = SizeElementsHeaders[i];
        int* result = calloc(size, sizeof(int));
        switch(i) {
            case 0: 
            //printf("B");
                encodageLittleEndian(result, 1, image->header.type[0]);
                encodageLittleEndian(result, 1, image->header.type[1]);
                //printf("result :%c", result[0]);
                //printf("B");
                fputc(image->header.type[0], fichier); 
                fputc(image->header.type[1], fichier); 
                break;
            case 1:
                encodageLittleEndian(result, SizeElementsHeaders[i], image->header.size);
                //printf("result 1 : %c", result[0]);
                break;
            case 2:
                encodageLittleEndian(result, SizeElementsHeaders[i], image->header.createdBy);
                break;
            case 3: 
                encodageLittleEndian(result, SizeElementsHeaders[i], image->header.createdByBis);
                break;
            case 4:
                encodageLittleEndian(result, SizeElementsHeaders[i], image->header.offset);
                break;
        }
        if(i!=0) {
            for(int j = 0; j< SizeElementsHeaders[i]; j++) {
                //printf("%c",result[j]);
                fputc(result[j], fichier);
            }
        }

           
        free(result);
    }
    
}

void ImgToFile(FILE* fichier, Image* image) {
    int caractereActuel;
    int i = 0;
    while(i<3*image->dibHeader.width*image->dibHeader.height + image->dibHeader.height * image->padding) {
        fputc(image->image[i], fichier);
        i++;
    }
}


void encoderDIBHeader(FILE* fichier, Image* image) {
    int SizeElementsDIBHeaders[3] = {4, 4, 4}; 
    int size;   
    for(int i =0; i<3; i++) {
        size = SizeElementsDIBHeaders[i];
        int* result = calloc(size, sizeof(int));
        switch(i) {
            case 0:
                encodageLittleEndian(result, SizeElementsDIBHeaders[i], image->dibHeader.sizeHeader);
                break;
            case 1:
                encodageLittleEndian(result, SizeElementsDIBHeaders[i], image->dibHeader.width);
                break;
            case 2: 
                encodageLittleEndian(result, SizeElementsDIBHeaders[i], image->dibHeader.height);
                break;
        }

        for(int j = 0; j< SizeElementsDIBHeaders[i]; j++) {
            fputc(result[j], fichier);
        }
        free(result);
    }


    for(int i = 0; i<image->header.offset-14-4-4-4; i++) {
        fputc(image->dibHeader.reste[i] ,fichier);
    }
    
}


void writeFileFromImage(FILE* fichier, Image* image)
{


    encoderHeader(fichier, image);
    encoderDIBHeader(fichier, image);
    ImgToFile(fichier, image);
 

}



//Afficher les infos du headers
void affichageHeader(Header header) {
    printf("\nInformation sur le header : \n");
    printf("    type : %s\n", header.type);
    printf("    Image size : %d\n", header.size);
    printf("    createdBy : %d\n", header.createdBy);
    printf("    createdByBis : %d\n", header.createdByBis);
    printf("    Offset : %d\n", header.offset);
}

//Afficher les infos du DIBHeaders
void affichageDIBHeader(DibHeader dibHeader) {
    printf("\nInformation sur le header : \n");
    printf("    sizeHeader : %d\n", dibHeader.sizeHeader);
    printf("    Image width : %d\n", dibHeader.width);
    printf("    Image height : %d\n", dibHeader.height);

    printf("    reste : ");
    for(int i=0; i<dibHeader.sizeHeader -4-4-4; i++ ) {
        printf("%d ", dibHeader.reste[i]);
    }
}


//Récuperer la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image)
unsigned char getP(Image* image, int height,  int width, int rgb) {
    //(image->dibHeader.height-1-height) car l'image en encodé du bas vers le haut
    //((rgb-1)*-1)+1 permet de passer de bgr à rgb
    // + height * image.padding car il a un padding pour especter des octet de 4 par ligne
    return image->image[((image->dibHeader.height-1-height)*(image->dibHeader.width*3) + width *3 + ((rgb-1)*-1)+1 + (image->dibHeader.height-1-height) * image->padding)];
}

//Mettre la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image) à (valeur)
void setP(Image* image, int height, int width, int rgb, int value) {
    //(image->dibHeader.height-1-height) car l'image en encodé du bas vers le haut
    //((rgb-1)*-1)+1 permet de passer de bgr à rgb
    // + height * image.padding car il a un padding pour especter des octet de 4 par ligne
    image->image[((image->dibHeader.height-1-height)*(image->dibHeader.width*3) + width *3 + ((rgb-1)*-1)+1 + (image->dibHeader.height-1-height) * image->padding)]=value;
}






void decoderANDgetHeader(FILE* fichier, Image* image) {
    int caractereActuel;
    int SizeElementsHeaders[5] = {2, 4, 2, 2, 4}; 
    for(int i =0; i<5; i++) {
        int* tab = NULL;
        tab = calloc(SizeElementsHeaders[i], sizeof(char));
        if(tab == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
        }
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(fichier);
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
        free(tab);
    }
    
}




/*
On recupere que la taille du DIBHeader, ainsi que la width et la height de l'image
Le fin du DIB Header depend de son type, on preferera donc ne pas le decoder pour permettre une compatibilité avec tout les types de DIBHeader
*/
void decoderANDgetDIBHeader(FILE* fichier, Image* image) {
    int caractereActuel;
    int SizeElementsDIBHeaders[3] = {4, 4, 4};
    for(int i = 0; i<3; i++) {
        int* tab;
        tab = calloc(SizeElementsDIBHeaders[i], sizeof(char));
        if(tab == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
        }

        for(int j = 0; j < SizeElementsDIBHeaders[i]; j++) {
            caractereActuel = fgetc(fichier);
            tab[j] = caractereActuel;
        }
        switch(i) {
            case 0:
                image->dibHeader.sizeHeader = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 1:
                image->dibHeader.width = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
            case 2:
                image->dibHeader.height = decodageLittleEndian(tab, SizeElementsDIBHeaders[i]);
                break;
        }
        free(tab);
    }

    
    //récuperer le reste, sans le décoder
    image->dibHeader.reste = calloc((image->header.offset-14-4-4-4), sizeof(unsigned char));
    if(image->dibHeader.reste == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
    }

    for(int i = 0; i<image->header.offset-14-4-4-4; i++) {
        caractereActuel = fgetc(fichier);
        image->dibHeader.reste[i] = caractereActuel;
    }
}


//Décoder et stocker l'image
void getImg(FILE* fichier, Image* image) {
    image->image = calloc((image->dibHeader.height*image->dibHeader.width*3 + image->dibHeader.height * image->padding ), sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
    }

    int caractereActuel;
    int i = 0;
    while(caractereActuel != EOF && i<3*image->dibHeader.width*image->dibHeader.height + image->dibHeader.height * image->padding) {
        caractereActuel = fgetc(fichier);
        image->image[i] = caractereActuel;
        i++;
    }
    // A enlever, est juste la au cas ou pour verifier
    if(caractereActuel == EOF) {
        printf("nop");
    }
}

//Fonction regroupant le nécessaire pour creer une image à partir d'un fichier
Image getImageFromFile(FILE *fichier) {
    Image image;
    image.dibHeader.reste = NULL;
    image.image = NULL;

    decoderANDgetHeader(fichier, &image);

    decoderANDgetDIBHeader(fichier, &image);

    image.padding = (4-(image.dibHeader.width*3)%4)%4;//surement une meilleur formule

    getImg(fichier, &image);


    return image;
}


//Obsolete
void ClearAndRedimensioner(Image *image, int height, int width, unsigned char value) {
    free(image->image);

    image->dibHeader.height = height;
    image->dibHeader.width = width;
    image->padding = (4-(image->dibHeader.width*3)%4)%4;

    image->image = calloc(height * width * 3, sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
    }


    for(int i = 0; i<image->dibHeader.height; i++) {
        for(int j = 0; j<image->dibHeader.width; j++) {
            for(int k = 0; k<3; k++) { 
                setP(image, i, j, k, value);
            }
        }
    }
}

//Créé une copie de image. Les deux images deviennent indépendantes.
Image copy(Image *image) {
    Image NewImage;
    NewImage.header = image->header;
    NewImage.dibHeader = image->dibHeader;
    NewImage.dibHeader.reste = NULL;
    NewImage.image = NULL;
    NewImage.padding = image->padding;


    NewImage.dibHeader.reste = calloc((NewImage.header.offset-14-4-4-4), sizeof(unsigned char));
    if(NewImage.dibHeader.reste == NULL) {
        printf("ERREUR ALLOCATION !");
        exit(0);
    }
    for(int i =0; i< NewImage.header.offset-14-4-4-4; i++) {
        NewImage.dibHeader.reste[i] = image->dibHeader.reste[i];
    }


    NewImage.image = calloc((NewImage.dibHeader.height*NewImage.dibHeader.width*3 + NewImage.dibHeader.height * NewImage.padding ), sizeof(unsigned char));
    if(NewImage.image == NULL) {
        printf("ERREUR ALLOCATION !");
        exit(0);
    }
    for(int i = 0; i<NewImage.dibHeader.height*NewImage.dibHeader.width*3 + NewImage.dibHeader.height * NewImage.padding; i++) {
        NewImage.image[i] = image->image[i];
    }
    return NewImage;
}

//Libere l'espace alloué dynamiquement par image
void freeImage(Image* image) {
    free(image->image);
    image->image = NULL;
    free(image->dibHeader.reste);
    image->dibHeader.reste = NULL;
}


//Affiche l'image en caractere ASCII avec " .:?#"
void afficherASCII(Image* image) {
    unsigned char tab[5];
    tab[0] = ' ';
    tab[1] = '.';
    tab[2] = ':';
    tab[3] = '?';
    tab[4] = '#';
    printf("\n");
    for(int i = 0; i<image->dibHeader.height; i++) {
        printf("\n");
        for(int j = 0; j<image->dibHeader.width; j++) {
            int valueRgb = 0.2126 * getP(image, i, j, 0) + 0.7152 * getP(image, i, j, 1) + 0.0722 * getP(image, i, j, 1);
            printf("%c", tab[valueRgb/(255/5)]);
            printf("%c", tab[valueRgb/(255/5)]);
        }
        
    }
}



void rogner(Image *image, int y, int x, int height, int width) {

    Image copie = copy(image);
    free(image->image);
    image->dibHeader.height = height;
    image->dibHeader.width = width;
    image->padding = (4-(image->dibHeader.width*3)%4)%4;

    image->image = calloc((image->dibHeader.height*image->dibHeader.width*3 + image->dibHeader.height * image->padding ), sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
    }  


    for(int i =0; i<height; i++) {
        for(int j = 0; j<width; j++) {
            for(int k =0; k<3; k++) {
                setP(image, i, j, k, getP(&copie, y + i, x + j, k));
            }
        }
    }

    freeImage(&copie);
}



// A peaufiner, nottament sur l'agandissement
//lisse toute l'image, voir le resultat en couleur
void redimensioner(Image* image, int height, int width) {

    Image copie = copy(image);
    free(image->image);
    image->dibHeader.height = height;
    image->dibHeader.width = width;
    image->padding = (4-(image->dibHeader.width*3)%4)%4;

    image->image = calloc((image->dibHeader.height*image->dibHeader.width*3 + image->dibHeader.height * image->padding ), sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
    }



    float tailleBlocY =(float) copie.dibHeader.height/height;
    float tailleBlocX =(float) copie.dibHeader.width/width;

    //Pour chaque pixel de la nouvelle image
    for (int h =0; h<height; h+=1) {
        for (int w =0; w<width; w+=1) {
            unsigned char RED = 0;      
            unsigned char GREEN = 0;                                   
            unsigned char BLUE = 0;

            for (int i =0; i<tailleBlocY; i+=1) {
                for (int j =0; j<tailleBlocX; j+=1) {
                    RED += getP(&copie, (float) h*tailleBlocY + i, (float) w*tailleBlocX + j, 0);
                    GREEN += getP(&copie, (float) h*tailleBlocY + i, (float) w*tailleBlocX + j, 1);
                    BLUE += getP(&copie, (float) h*tailleBlocY + i, (float) w*tailleBlocX + j, 2);
                }
            }


            setP(image, h, w, 0, RED/((float) tailleBlocX*tailleBlocY));
            setP(image, h, w, 1, GREEN/((float) tailleBlocX*tailleBlocY));
            setP(image, h, w, 2, BLUE/((float) tailleBlocX*tailleBlocY));
        }
    }

    freeImage(&copie);



}








int main()
{


    FILE* fichier = NULL;

    fichier = fopen("Images/img.bmp", "rb");

    if(fichier == NULL) {
        printf("Erreur dans la lecture du fichier !");
        exit(0);
    }

    Image image = getImageFromFile(fichier);
    
    fclose(fichier);





    afficherASCII(&image);



    FILE* fichierF = NULL;
    fichierF = fopen("testEcriture.bmp", "wb+");
 
    if(fichierF == NULL) {
        printf("Erreur dans l'ouverture du fichier !");
        exit(0);
    }

    writeFileFromImage(fichierF, &image);
    fclose(fichierF);



    freeImage(&image);



   
    return 0;
    
}



/*
|--------------------------------------------
|NE PAS CONSIDERER LA SUITE (globalement utilisé comme corbeille)
|--------------------------------------------
void redimensioner(Image* image, int height, int width) {

    Image copie = copy(image);
    free(image->image);
    image->dibHeader.height = height;
    image->dibHeader.width = width;
    image->padding = (4-(image->dibHeader.width*3)%4)%4;

    image->image = calloc((image->dibHeader.height*image->dibHeader.width*3 + image->dibHeader.height * image->padding ), sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION !");
            exit(0);
    }



    float tailleBlocY =(float) copie.dibHeader.height/height;
    float tailleBlocX =(float) copie.dibHeader.width/width;


    printf("CoefY %f; coefX: %f", tailleBlocY, tailleBlocX);
    //Pour chaque pixel de la nouvelle image
    for (int h =0; h<height; h+=1) {
        for (int w =0; w<width; w+=1) {

            int r =0;
            int g =0;
            int b=0;
            for (int i =0; i<tailleBlocY; i+=1) {
                for (int j =0; j<tailleBlocX; j+=1) {
                    int tempr = getP(&copie, h*tailleBlocY + i, w*tailleBlocX + j, 0);
                    int tempg = getP(&copie, h*tailleBlocY + i, w*tailleBlocX + j, 1);
                    int tempb = getP(&copie, h*tailleBlocY + i, w*tailleBlocX + j, 2);

                    if(i==0) {
                        //printf("coef 0: %f", 1-((float) (h*tailleBlocY) - (int) (h*tailleBlocY)));
                        tempr*= 1-((float) (h*tailleBlocY) - (int) (h*tailleBlocY));
                        tempg*= 1-((float) (h*tailleBlocY) - (int) (h*tailleBlocY));
                        tempb*= 1-((float) (h*tailleBlocY) - (int) (h*tailleBlocY));
                    } else if (i+1>tailleBlocY) {
                        printf("%f ", 1- (float) (i+1-tailleBlocY));
                        tempr*= 1- (float) (i+1-tailleBlocY);
                        tempg*= 1- (float) (i+1-tailleBlocY);
                        tempb*= 1- (float) (i+1-tailleBlocY);
                    }

                    if(j==0) {
                        tempr*= 1-((float) (w*tailleBlocX) - (int) (w*tailleBlocX));
                        tempg*= 1-((float) (w*tailleBlocX) - (int) (w*tailleBlocX));
                        tempb*= 1-((float) (w*tailleBlocX) - (int) (w*tailleBlocX));
                    } else if (j+1>tailleBlocX) {
                        tempr*= 1- (float) (j+1-tailleBlocX);
                        tempg*= 1- (float) (j+1-tailleBlocX);
                        tempb*= 1- (float) (j+1-tailleBlocX);
                    }

                    r+=tempr;
                    g+=tempg;
                    b+=tempb;
                }   
            }
            setP(image, h, w, 0, r/(tailleBlocY*tailleBlocX));
            setP(image, h, w, 1, g/(tailleBlocY*tailleBlocX));
            setP(image, h, w, 2, b/(tailleBlocY*tailleBlocX));
        }
    }

    freeImage(&copie);

}
*/