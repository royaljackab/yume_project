# Normes d'écriture du code #

Ce document est à lire et à **RESPECTER** le plus que possible pendant votre expérience de code
du projet.

## Définitions ##

1. Structures

Pour les nommages de structures, employer le PascalCase.

2. Fonctions

Privilégier le snake_case, et faire en sorte que le premier mot de la fonction  
est le nom du système dans lequel il vit.
Ex : input_get_key -> du systeme input
movement_set_velocity...

3. Variables globales et Macros

Utiliser dans la majorite des cas le SCREAMING_SNAKE_CASE

## Organisation du code ##

Risque potentiellement de changer, dans ce cas, bien updater cette partie  

- Tous les fichier headers .h sont a ranger dans le fichier lib situé à la source du projet (mrc Claudine)  
- Maintenir une organisation la plus **claire** possible pour les .c :
  - Essayez de classer les fichiers dans des dossiers correspondant a leur niveau dans le code.
  - core -> bahh le coeur du jeu quoi donc systeme de game_state, ecs, input, etc...
  - systeme -> Code pour les systemes des composantes pour l'ecs, on y retrouvera notamment  
  le mouvement, les collisions, les sprites, lasers...
  - components -> définitions des composantes, a priori que des fichiers courts (pas sur que ce soit utilisé meme, a voir...)
  - Mais plus tard, j'imagine une structure avec un dossier enemi, un dossier pour les pattern etc... Bref, soyez **lisible**,  
  éviter le plus possible d'avoir tout en pagaille

## Ecriture du code ##

**A PRIVILEGIER SURTOUT :** la compréhension humaine.  

Par là, je ne parle pas spécialement de l'implémentation interne des systèmes, il faut juste s'assurer qu'elles marchent  
et quelles soient assez flexibles. PAR CONTRE, on attend d'un systeme qu'il fournisse un ensemble de fonctions **complètes**  
qui ne nécessitent pas aux autres dev de devoir comprendre comment le code de ce dernier fonctionne.  
Ca demande d'écrire des fonctions du style systeme_get_attribut(..), pour que quand on utilise derriere un système,  
on puisse comprendre directement ce que fait une ligne rien qu'en lisant le nom de la fonction

**OUI**, cela nécessite sans doute un peu plus de temps a l'implementation de chaque systeme car ça demande d'écrire des  
fonctions faisant souvent pas plus de 5 lignes, mais ça va aider tout le monde quand on utilisera les systemes des autres,
et meme nous-même, puisqu'apres un certain temps on oublie souvent comment on a codé les choses...


