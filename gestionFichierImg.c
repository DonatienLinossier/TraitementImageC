#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "gestionFichierImg.h"
#include "interface.h"

#define COEF_RED 0.2126
#define COEF_GREEN 0.7152
#define COEF_BLUE 0.0722


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
      |------>Padding(Interne à l'image, ne sert pas en dehors. N'est pas stocké dans les fichiers.)






Recap:
|-------------------------------------------------------------|
|Fonction                | Interne/Externe | Etat | accesible |
|-------------------------------------------------------------|
|                          DECODAGE                           |
|getImageFromFile        | Externe         | OK   |    OUI    |
|decodeANDgetHeader      | Interne         | OK   |    X      |
|decodeANDgetDIBHeader   | Interne         | OK   |    X      |
|getImg                  | Interne         | OK   |    X      |
|                                                             |
|                          ENCODAGE                           |
|writeFileFromImage      | Externe         | OK   |    OUI    |
|encodeHeader            | Interne         | OK   |    X      |
|encodeDIBHeader         | Interne         | OK   |    X      |
|ImgToFile               | Interne         | OK   |    X      |
|                                                             |
|                         AFFICHAGE                           |
|afficherASCII           | Externe         | OK   |    OUI    |
|                                                             |
|                    OPERATION SUR IMAGE                      |
|getP                    | Externe         | OK   |    OUI    | 
|setP                    | Externe         | OK   |    OUI    | 
|copy                    | Externe         | OK   |    OUI    | 
|freeImage               | Externe         | OK   |    OUI    | 
|rogner                  | Externe         | OK   |    OUI    |
|ClearAndRedimensioner   | Externe         | OK   |    OUI    |
|                                                             |
|                       STEGANOGRAPHIE                        |
|readingStegano          | Externe         | OK   |    OUI    |
|wrintingStegano         | Externe         | OK   |    OUI    |
|ecriture_steganoCache   | Externe         |abdon |    NON    |
|                                                             |
|                           DEBUG                             |
|printInfoHeader         | Interne         | OK   |    X      |
|printInfoDIBHeader      | Interne         | OK   |    X      |
|                                                             |
|                          BINAIRE                            |
|decodageLittleEndian    | Interne         | OK   |    X      |
|littleEndianEncoding    | Interne         | OK   |    X      |
|decompositionBinaire8Bit| Interne         | OK   |    X      |
|bin8bitToInt            | Interne         | OK   |    X      |
|-------------------------------------------------------------|

*/

//--------------------------------
//Manipulation binaire
//--------------------------------

//Renvoie la valeur int stockée dans bits en binaire little endian 
//Utile pour décoder les infos dans les headers stockées en little Endian(La casi totalité des infos).
int littleEndianDecoding(int* bits, int SIZE) {
    if(bits==NULL) {
        exit(0);
    }
    int resultFinal = 0;
    for (int i =0; i<SIZE; i++) {
        int nb = bits[i];
        int result = 0;
        int decomposition[8];
        for(int h=0; h<8; h++) {  
                decomposition[h] = nb%2;  
                result += decomposition[h] * pow(2, h) * pow(2, 8*i);
                nb = nb/2;
        }
        resultFinal+= result;
        
    } 
    return resultFinal;
}

//Renvoie le binaire encodé en little endian  de la valeur value
void littleEndianEncoding(int* bitsResult, int SIZE, int value) {
    if(bitsResult==NULL) {
        exit(0);
    }
    int i =0;
    int* binaire = NULL;
    binaire = calloc(SIZE*8, sizeof(int));
    if(binaire==NULL) {
        exit(0);
    }
    while(i < SIZE*8) {
        binaire[i] = value%2;
        value = value/2;
        i++;
    }

    for(int i=0; i<SIZE; i++) {
        int result = 0;
        for (int j=0; j<8; j++) {
            result += binaire[i*8 +j] * pow(2, j);
        }
        bitsResult[i] = result;
    }
}

//Renvoie un octet binaire de la valeur value entrée
void intTo8bitBinary(char* result, unsigned char value) {
    if(result==NULL) {
        exit(0);
    }
    int i = 0;
    while(i<8) {
        result[8-1-i] = value%2;
        value/=2;
        i++;
    }
}

//Recupere la valeur int contenu dans un octet
char bin8bitToInt(char* bin) {
    if(bin == NULL) {
        exit(0);
    }
    char result = 0;
    for(int i = 0; i<8; i++) {
        result+= bin[i] * pow(2, 8-1-i);
    }
    return result;
}



//--------------------------------
//Steganographie
//--------------------------------
char* steganoReading(Image* image) {
    if(image == NULL) {
        exit(0);
    }
    char* hiddenTextbin = NULL;
    hiddenTextbin = calloc(image->dibHeader.height *  image->dibHeader.width * 3 * 2, sizeof(char));
    if(hiddenTextbin==NULL) {
        exit(0);
    }

    int height =0;
    int width = 0;
    int rgb = 0;
    int bit = 0;


    char decomposition[8];
    
    intTo8bitBinary(decomposition, getP(image, height, width, rgb));

    char *hiddenText = NULL;

    int lenght = (image->dibHeader.height *  image->dibHeader.width * 3 * 2)/8;
    
    for(int i = 0; i<image->dibHeader.height *  image->dibHeader.width * 3 * 2; i++) {

        //detecter fin de chaine de caractere(si somme nb binaire = 0, signifie tout les bits = 0, en ASCII on a donc /0, donc fin de chaine)
        if(i%8==7) {
            int EOS = 1; //Boolean de si on est a la fin de la chaine (EndOfString)
            for (int t = 0; t < 8; t++)
            {
                if(hiddenTextbin[i-t]==1) {
                    EOS = 0;
                    break;
                }
            }
            if(EOS) {
                lenght = (i+1)/8;
                hiddenText = calloc(lenght, sizeof(char));
                if(hiddenText == NULL) {
                    exit(0);
                }
                break;
            }
        }

        hiddenTextbin[i] = decomposition[7-bit];
        bit++;
        if(bit>1) {
            bit=0;
            rgb++;
            if(rgb>2) {
                rgb=0;
                width++;
                if(width>image->dibHeader.width) {
                    width=0;
                    height++;
                    if(height>image->dibHeader.height) {
                        printf("Vous etes arrive au bout de l'image lors de la lecture d'un potentiel message cache. Il se peut que le message soit stocke sur plusieurs fichier, ou tout simplement qu'il n'y ait pas de message cache.\n");
                        break;
                    }
                }
            }
            intTo8bitBinary(decomposition, getP(image, height, width, rgb));
        }
    }
    
    if(hiddenText==NULL) {
        printf("Aucun message trouvé, ou le message n'a pas pu etre correctement encodé. Trop long ?"); //Attention au cas ou le /0 serait apres la fin du fichier
        hiddenText = calloc(5, sizeof(char));
        if(hiddenText == NULL) {
            exit(0);
        }
        hiddenText[0] = 'N';
        hiddenText[1] = 'o';
        hiddenText[2] = 'n';
        hiddenText[3] = 'e';
        hiddenText[4] = '\0';
        return hiddenText;
    }
    for(int t =0; t< lenght; t++) {
        hiddenText[t] = bin8bitToInt(hiddenTextbin + t*8);
    }
    
    return hiddenText;
}





void steganoWriting(Image* image, char* value) {
    if(image == NULL || value == NULL) {
        exit(0);
    }
    //Tableau 1D de tout les nombres binaires a écrire.
    //printf("Message a cacher : %s", value);
    int size = strlen(value);
    char *textBin = NULL;
    textBin = calloc((size +1) * 8, sizeof(char)); //le +1 puor ajouter le caractere /0(fin de chaine), qui est encodé en ascii par 00000000
    if (textBin == NULL) {
        exit(0);
    }

    for(int i =0; i< size; i++) {
        intTo8bitBinary(textBin + i * 8, value[i]);
    }

    int height = 0;
    int width = 0;
    int rgb = 0;
    int bit = 0;


    char decompt[8];
    intTo8bitBinary(decompt, getP(image, height, width, rgb));

    for(int i = 0; i<(size+1) * 8; i++) {
        decompt[7-bit] = textBin[i];
        bit++;
        if(bit>1) {
            bit=0;
            setP(image, height, width, rgb, bin8bitToInt(decompt));
            rgb++;
            if(rgb>2) {
                rgb=0;
                width++;
                if(width>=image->dibHeader.width) {
                    width=0;
                    height++;
                    if(height>=image->dibHeader.height) {
                        printf("Votre est message n'a pas pu etre inscrit en entier. Votre message est trop long par rapport a la taille de votre image.\nEssayer avec une image plus grande ou un message plus court pour inscrire l'integralite de votre message.\n");
                        break;
                    }
                }
            }
            intTo8bitBinary(decompt, getP(image, height, width, rgb));
        }
    }
}


//--------------------------------
//Decodage
//--------------------------------
//Remplis les valeurs du Header de Image à partir des informations trouvées dans le fichier FILE
void decodeANDgetHeader(FILE* file, Image* image) {
    if(file == NULL || image == NULL) {
        exit(0);
    }
    int caractereActuel;
    int SizeElementsHeaders[5] = {2, 4, 2, 2, 4}; 
    for(int i =0; i<5; i++) {
        int* tab = NULL;
        tab = calloc(SizeElementsHeaders[i], sizeof(char));
        if(tab == NULL) {
            printf("ERREUR ALLOCATION decoderANDgetHeader!");
            exit(0);
        }
        for(int j = 0; j< SizeElementsHeaders[i]; j++) {
            caractereActuel = fgetc(file);
            tab[j] = caractereActuel;
        }
        switch(i) {
            case 0: 
                image->header.type[0] = tab[0];
                image->header.type[1] = tab[1];
                break;
            case 1:
                image->header.size = littleEndianDecoding(tab, SizeElementsHeaders[i]);
                break;
            case 2:
                image->header.createdBy = littleEndianDecoding(tab, SizeElementsHeaders[i]);
                break;
            case 3: 
                image->header.createdByBis = littleEndianDecoding(tab, SizeElementsHeaders[i]);
                break;
            case 4:
                image->header.offset = littleEndianDecoding(tab, SizeElementsHeaders[i]);
                break;
        }
        free(tab);
    }
    
}

/*
//Remplis les valeurs du DIBHeader de Image à partir des informations trouvées dans le fichier FILE:

On ne recupere que la taille du DIBHeader, ainsi que la width et la height de l'image. Le padding est aussi calculer.
Le fin du DIB Header depend de son type, on preferera donc ne pas le decoder pour permettre une compatibilité avec tout les types de DIBHeader
*/
void decodeANDgetDIBHeader(FILE* file, Image* image) {
    if(file == NULL || image == NULL) {
        exit(0);
    }
    int caractereActuel;
    int sizeElementsDIBHeaders[3] = {4, 4, 4};
    for(int i = 0; i<3; i++) {
        int* tab = NULL;
        tab = calloc(sizeElementsDIBHeaders[i], sizeof(char));
        if(tab == NULL) {
            printf("ERREUR ALLOCATION decoderANDgetDIBHeader 1!");
            exit(0);
        }

        for(int j = 0; j < sizeElementsDIBHeaders[i]; j++) {
            caractereActuel = fgetc(file);
            tab[j] = caractereActuel;
        }
        switch(i) {
            case 0:
                image->dibHeader.sizeHeader = littleEndianDecoding(tab, sizeElementsDIBHeaders[i]);
                break;
            case 1:
                image->dibHeader.width = littleEndianDecoding(tab, sizeElementsDIBHeaders[i]);
                break;
            case 2:
                image->dibHeader.height = littleEndianDecoding(tab, sizeElementsDIBHeaders[i]);
                break;
        }
        free(tab);
    }

    
    //récuperer le reste, sans le décoder
    image->dibHeader.rest = calloc((image->header.offset-14-4-4-4), sizeof(unsigned char));
    if(image->dibHeader.rest == NULL) {
            printf("ERREUR ALLOCATION decoderANDgetDIBHeader 2!");
            exit(0);
    }

    for(int i = 0; i<image->header.offset-14-4-4-4; i++) {
        caractereActuel = fgetc(file);
        image->dibHeader.rest[i] = caractereActuel;
    }
}


//Décoder et stocker l'image
void getImg(FILE* file, Image* image) {
    if(file == NULL || image == NULL) {
        exit(0);
    }
    image->image = calloc((image->dibHeader.height*image->dibHeader.width*3 + image->dibHeader.height * image->padding ), sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION getImg!");
            exit(0);
    }

    int caractereActuel;
    int i = 0;
    while(caractereActuel != EOF && i<3*image->dibHeader.width*image->dibHeader.height + image->dibHeader.height * image->padding) {
        caractereActuel = fgetc(file);
        image->image[i] = caractereActuel;
        i++;
    }
}

//Fonction regroupant le nécessaire pour creer une image à partir d'un file
Image getImageFromFile(FILE *file) {
    if(file == NULL) {
        exit(0);
    }

    Image image;
    image.dibHeader.rest = NULL;
    image.image = NULL;

    //Récuperer et stocker le Header du fichier
    decodeANDgetHeader(file, &image);

    //Récuperer et stocker le DIBHeader de l'image
    decodeANDgetDIBHeader(file, &image);

    //Calculer le padding, necessaire a l'utilisation de l'image. (Necessite de connaitre la width de l'image)
    image.padding = (4-(image.dibHeader.width*3)%4)%4;//surement une meilleur formule

    //Récuperer et stocker l'image. Le stockage est 'brut', aucune manipulation n'est effectuée, les valeurs sont stockées de la meme maniere que dans le fichier.
    //Pour acceder aux pixels il faudra donc obligatoirement utiliser getP et setP.
    getImg(file, &image);


    return image;
}




//--------------------------------
//Encodage
//--------------------------------
//Encode les infos du header du file
void encodeHeader(FILE* file, Image* image) {
    if(file == NULL || image == NULL) {
        exit(0);
    }
    int sizeElementsHeaders[5] = {2, 4, 2, 2, 4}; 
    int size;   
    for(int i =0; i<5; i++) {
        size = sizeElementsHeaders[i];
        int* result = calloc(size, sizeof(int));
        switch(i) {
            case 0: 
                littleEndianEncoding(result, 1, image->header.type[0]);
                littleEndianEncoding(result, 1, image->header.type[1]);
                fputc(image->header.type[0], file); 
                fputc(image->header.type[1], file); 
                break;
            case 1:
                littleEndianEncoding(result, sizeElementsHeaders[i], image->header.size);
                break;
            case 2:
                littleEndianEncoding(result, sizeElementsHeaders[i], image->header.createdBy);
                break;
            case 3: 
                littleEndianEncoding(result, sizeElementsHeaders[i], image->header.createdByBis);
                break;
            case 4:
                littleEndianEncoding(result, sizeElementsHeaders[i], image->header.offset);
                break;
        }

        if(i!=0) {
            for(int j = 0; j< sizeElementsHeaders[i]; j++) {
                fputc(result[j], file);
            }
        }

           
        free(result);
    }
    
}

//Stocke l'image dans le fichier.
void ImgToFile(FILE* file, Image* image) {
    if(file == NULL || image == NULL) {
        exit(0);
    }
    int caractereActuel;
    int i = 0;
    while(i<3*image->dibHeader.width*image->dibHeader.height + image->dibHeader.height * image->padding) {
        fputc(image->image[i], file);
        i++;
    }
}

//Encode les infos du DIBheader de l'image
void encodeDIBHeader(FILE* file, Image* image) {
    if(file == NULL || image == NULL) {
        exit(0);
    }
    int SizeElementsDIBHeaders[3] = {4, 4, 4}; 
    int size;   
    for(int i =0; i<3; i++) {
        size = SizeElementsDIBHeaders[i];
        int* result = calloc(size, sizeof(int));
        switch(i) {
            case 0:
                littleEndianEncoding(result, SizeElementsDIBHeaders[i], image->dibHeader.sizeHeader);
                break;
            case 1:
                littleEndianEncoding(result, SizeElementsDIBHeaders[i], image->dibHeader.width);
                break;
            case 2: 
                littleEndianEncoding(result, SizeElementsDIBHeaders[i], image->dibHeader.height);
                break;
        }

        for(int j = 0; j< SizeElementsDIBHeaders[i]; j++) {
            fputc(result[j], file);
        }
        free(result);
    }


    for(int i = 0; i<image->header.offset-14-4-4-4; i++) {
        fputc(image->dibHeader.rest[i] , file);
    }
    
}

//Stocke une image Image dans le fichier file. 
void writeFileFromImage(FILE* file, Image* image)
{   
    if(file == NULL || image == NULL) {
        exit(0);
    }
    //Stockage du Header dans le file
    encodeHeader(file, image);

    //Stockage du DIBHeader dans le file
    encodeDIBHeader(file, image);

    //Stockage de l'image dans le file
    ImgToFile(file, image);
 

}

//--------------------------------
//Manipulation pixel
//--------------------------------
//Récuperer la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image)
unsigned char getP(Image* image, int height,  int width, int rgb) {
    if(image == NULL) {
        exit(0);
    }
    //(image->dibHeader.height-1-height) car l'image en encodé du bas vers le haut
    //((rgb-1)*-1)+1 permet de passer de bgr à rgb
    // + height * image.padding car il a un padding pour especter des octet de 4 par ligne
    if(height>image->dibHeader.height) {
        printf("Valeur trop de height trop haute");
        return 0;
    } else if (height<0) {
        printf("Valeur de Height trop basse.");
    }
    if(width>image->dibHeader.width) {
        printf("Valeur trop de width trop haute");
        return 0;
    } else if(width<0) {
        printf("valeur de rgb trop basse.");
        return 0;
    }
    if(rgb>2) {
        printf("Valeur de rgb trop haute.");
        return 0;
    } else if (rgb<0) {
        printf("Valeur de rgb trop basse");
        return 0;
    }
    return image->image[((image->dibHeader.height-1-height)*(image->dibHeader.width*3) + width *3 + ((rgb-1)*-1)+1 + (image->dibHeader.height-1-height) * image->padding)];
}

//Mettre la valeur de la composante (rgb) de la ligne (height) à la colonne (width) de l'image (*image) à (valeur)
void setP(Image* image, int height, int width, int rgb, int value) {
    if(image == NULL) {
        exit(0);
    }
    //(image->dibHeader.height-1-height) car l'image en encodé du bas vers le haut
    //((rgb-1)*-1)+1 permet de passer de bgr à rgb
    // + height * image.padding car il a un padding pour especter des octet de 4 par ligne
    if(height>image->dibHeader.height) {
        printf("Valeur trop de height trop haute");
        return;
    } else if (height<0) {
        printf("Valeur de Height trop basse.");
    }
    if(width>image->dibHeader.width) {
        printf("Valeur trop de width trop haute");
        return;
    } else if(width<0) {
        printf("valeur de rgb trop basse.");
        return;
    }
    if(rgb>2) {
        printf("Valeur de rgb trop haute.");
        return;
    } else if (rgb<0) {
        printf("Valeur de rgb trop basse");
        return;
    }
    image->image[((image->dibHeader.height-1-height)*(image->dibHeader.width*3) + width *3 + ((rgb-1)*-1)+1 + (image->dibHeader.height-1-height) * image->padding)]=value;
}






//--------------------------------
//Manipulation d'image
//--------------------------------

//Change la taille de l'image entrée et les variables qui y sont liées. Vide l'image précédente.
void clearAndResize(Image *image, int height, int width) {
    if(image == NULL) {
        exit(0);
    }
    free(image->image);

    image->dibHeader.height = height;
    image->dibHeader.width = width;
    image->padding = (4-(image->dibHeader.width*3)%4)%4;

    image->image = calloc(height * width * 3 + image->dibHeader.height * image->padding , sizeof(unsigned char));
    if(image->image == NULL) {
            printf("ERREUR ALLOCATION clearAndResize!");
            exit(0);
    }
}

//Créé une copie de image. Les deux images sont indépendantes.
Image copy(Image *image) {
    if(image == NULL) {
        exit(0);
    }

    //Creation de la copie d'image et affectation des variable avec allocation statique. Les pointeurs d'allocation dynamique sont mit a NULL. (Pour empecher que la suppr d'une image suppr des infos encore utile a une autre image)
    Image NewImage;
    NewImage.header = image->header;
    NewImage.dibHeader = image->dibHeader;
    NewImage.dibHeader.rest = NULL;
    NewImage.image = NULL;
    NewImage.padding = image->padding;



    //Affectation avec allocation dynamique

    //Copie du reste du header
    NewImage.dibHeader.rest = calloc((NewImage.header.offset-14-4-4-4), sizeof(unsigned char));
    if(NewImage.dibHeader.rest == NULL) {
        printf("ERREUR ALLOCATION copy 1!");
        exit(0);
    }
    for(int i =0; i< NewImage.header.offset-14-4-4-4; i++) {
        NewImage.dibHeader.rest[i] = image->dibHeader.rest[i];
    }

    //Copie de l'image
    NewImage.image = calloc((NewImage.dibHeader.height*NewImage.dibHeader.width*3 + NewImage.dibHeader.height * NewImage.padding ), sizeof(unsigned char));
    if(NewImage.image == NULL) {
        printf("ERREUR ALLOCATION copy 2!");
        exit(0);
    }
    for(int i = 0; i<NewImage.dibHeader.height*NewImage.dibHeader.width*3 + NewImage.dibHeader.height * NewImage.padding; i++) {
        NewImage.image[i] = image->image[i];
    }

    return NewImage;
}

//Libere l'espace alloué dynamiquement par image
void freeImage(Image* image) {
    if(image == NULL) {
        exit(0);
    }
    //Liberation de l'image
    free(image->image);
    image->image = NULL;

    //liberation du reste du DibHeader
    free(image->dibHeader.rest);
    image->dibHeader.rest = NULL;
}

void rogner(Image *image, int y, int x, int height, int width) {
    if(image == NULL) {
        exit(0);
    }
    if(height+y>image->dibHeader.height) {
        printf("Depassement y de l'image");
        return;
    }
    if(width+x>image->dibHeader.width) {
        printf("Depassement x de l'image");
        return;
    }
    if(x<0) {
        printf("Depassement x de l'image");
        return;
    }
    if(y<0) {
        printf("Depassement y de l'image");
        return;
    }
    
    //copie de l'image pour conserver les valeurs des pixels le temps de la manipulation
    Image copie = copy(image);


    //Redimensionne l'image en lui donnant sa taille height et width
    clearAndResize(image, height, width);

    //copie les pixels de la selection dans l'image
    for(int i =0; i<height; i++) {
        for(int j = 0; j<width; j++) {
            for(int k =0; k<3; k++) {
                setP(image, i, j, k, getP(&copie, y + i, x + j, k));
            }
        }
    }

    //liberer la copie
    freeImage(&copie);
}



//--------------------------------
//Debug/Affichage
//--------------------------------

//Afficher les infos du headers
void printInfoHeader(Header header) {
    printf("\nInformation sur le header : \n");
    printf("    type : %s\n", header.type);
    printf("    Image size : %d\n", header.size);
    printf("    createdBy : %d\n", header.createdBy);
    printf("    createdByBis : %d\n", header.createdByBis);
    printf("    Offset : %d\n", header.offset);
}

//Afficher les infos du DIBHeaders
void printInfoDIBHeader(DibHeader dibHeader) {
    printf("\nInformation sur le header : \n");
    printf("    sizeHeader : %d\n", dibHeader.sizeHeader);
    printf("    Image width : %d\n", dibHeader.width);
    printf("    Image height : %d\n", dibHeader.height);

    printf("    reste : ");
    for(int i=0; i<dibHeader.sizeHeader -4-4-4; i++ ) {
        printf("%d ", dibHeader.rest[i]);
    }
}

//Affiche l'image en caractere ASCII avec " .:?#"
void printASCII(Image* image) {
    if(image == NULL) {
        exit(0);
    }
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
            int valueRgb = COEF_RED * getP(image, i, j, 0) + COEF_GREEN * getP(image, i, j, 1) + COEF_BLUE * getP(image, i, j, 1); //des coefs basés sur la perception humaine des couleurs ont été appliqué afin de rendre compte des couleurs meme en ASCII
            printf("%c%c", tab[valueRgb/(255/5)], tab[valueRgb/(255/5)]); //les caracteres étant plus haut que large dans le terminal, on les affiche en double pour eviter que l'image apparaisse étirée verticalement: On preserve plus ou moins les proportions. 
        }
    }
}