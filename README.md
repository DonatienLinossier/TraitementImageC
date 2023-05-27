
# TraitementImageC    

Ce projet a pour but de créer un programme en C permettant le traitement d'images en format bmp. Vous allez pouvoir choisir une image dans un dossier prédéfini lui appliquer différents filtres et l'enregistrer dans un nouveau fichier.

## Sommaire

* [Informations générales](#informations-générales)
* [Guides des commandes](#guides-des-commandes)
* [Options du code](#options-du-code)
* [Auteurs](#auteurs)
* [Technologies](#technologies)

## Informations générales

Une fois le programme lancé, l'utilisateur a le choix entre plusieurs images prédéfinies enregistrés dans un dossier Images, ainsi que les images créés précédemment par l'utilisateur.

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
| Permet d'annuler le dernier filtre | undo |
| Afficher l'image en ASCII | displayASCII  |


## Guides des commandes

A partir du terminal:

* Pour compiler : `make`. Permet d'initialiser/vérifier tous les répertoires et fichiers nécessaires à l'execution du programme, puis compile le code. 
* Pour lancer le programme :  `make run`.
* Pour nettoyer le programme : `make clean`. Permet de nettoyer tous les répertoires et fichiers nécessaires à l'execution du programme. Cela comprend le code compilé, les images créées par l'utilisateur, les fichiers temporaires ainsi que les fichiers utilisés par la stéganographie.



## Options du code

* Le module `2 - Modifier la selection` permet de selectionner une zone. Les filtres suivants ne seront appliqués que sur la zone selectionnée. A l'ouverture du programme, la selection représente l'image entière.
* La fonction `3 - Rogner` doit être précéder d'une séléction de la partie l'image à traiter (Module `2 - Modifier la selection`).
* Pour les images de grande taille, il est conseillé de dézoomer dans le module `4 - Affichage en ASCII` pour les afficher correctement. 
* La fonction `13 - Steganographie` permet de cacher un message entré manuellement dans l'interface, ou au préalablement inscrit dans le fichier *Stegano/Input.txt*, dans l'image active. De même, il est possible de récupérer un message caché dans l'image active pour l'afficher dans le terminal. Il est aussi possible d'inscrire le message caché directement dans le fichier *Stegano/Output.txt*.
* Les images doivent être enregistrées manuellement avec le module `14 - Enregistrer l'image` pour être sauvegardés.
* Les images s'ouvrent manuellement au lancement du programme. Il est aussi possible de changer d'image au cours du programme avec le module `15 - Changer d'image`. 
* La fonction `16 - Annuler le dernier changement` ne fonctionne qu'une seule fois à la suite et ne retiens que la denière image. Elle permet d'annuler, à la manière d'un *ctrl+z*, la modification précédente.

## Technologies

Ce projet a été réalisé sur les logiciel Visual Studio Code et VSCodium. Le langage utilisé est le C.

Les différents fichiers utilisés sont enregistrés sur la plateforme Github et l'addresse de dépôt est : https://github.com/DonatienLinossier/TraitementImageC 



## Auteurs

- [@DonatienLinossier](https://www.github.com/DonatienLinossier)
- [@EliasGaudaré](https://www.github.com/Eliasgdr)
- [@AsmaKajeiou](https://www.github.com/asmakaj)





