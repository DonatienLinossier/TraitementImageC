
# TraitementImageC

Ce projet a pour but de créer un programme en C permettant le traitement d'images en format bmp. Vous allez pouvoir choisir une image dans un dossier prédéfini lui appliquer différents filtres et l'enregistrer dans un nouveau fichier.

## Sommaire


* [Informations générales](#info-general)
* [Guides des commandes](#commandes)
* [Options du code](#options)
* [Auteurs](#auteurs)
* [Technologies](#technologies)

## Informations générales

Une fois le programme lancé, l'utilisateur a le choix entre plusieurs images prédéfinies enregistrés dans un dossier Images.

L'utilisateur peut ensuite choisir un filtre à apposer sur l'image parmi la liste suivante :
 
| Filtres | Fonctions | 
|----------|:-------------:|
| Redimensionner l'image | resize | 
| Séléctionner une partie de l'image| selection | 
| Passer l'image en noir et blanc| grayscale | 
|  Rogner l'image | crop  | 
| Effectuer une rotation (90°, 180° ou 270°)| rotation | 
| Inverser les couleurs de l'image |  invertcolorsInterface | 
| Gérer la luminosité de l'image | brightness  |
| Gérer le contraste de l'image | contrast | 
| Flouter l'image | blur | 
| Traduit limage en binaire | binarize |
| Effectueur une symétrie selon l'axe horizontal ou vertical | symmetry |
| Permet de cacher un message| steganography |
| Enregistre l'image dans le dossier Output| saveImage  |
| Permet de changer l'image | changeImage  |
| Afficher l'image en ASCII | displayASCII  |




## Guides des commandes

A partir du terminal:

    * Pour la première fois : § make init 
    Cela permet d'initialiser tous les répertoires et fichiers nécessaires à l'execution du programme.

    * Pour compiler : § make
    
    * Pour lancer le programme : § make run



## Options du code

* Il vous faudra ouvrir le terminal et effctuer un zoom négatif pour que les images en ASCII se voient dans leur entièreté.

* Les images s'ouvrent manuellement et elles sont modifiées après enregistrement.

* La fonction Crop doit être précédent d'une séléction


## Technologies

Ce projet a été réalisé sur le logiciel Visual Studio Code.

Les différents fichiers utilisés sont enregistrés sur la plateforme Github et l'addresse de dépot est : https://github.com/DonatienLinossier/TraitementImageC 

Ce projet utilise (technologies au niveau de la gestion de fichiers ??)

## Authors

- [@DonationLinossier](https://www.github.com/DonatienLinossier)
- [@EliasGaudaré](https://www.github.com/Eliasgdr)
- [@AsmaKajeiou](https://www.github.com/asmakaj)



## Appendix

Any additional information goes here

