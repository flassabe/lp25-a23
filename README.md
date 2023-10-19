Sujet en version du 19 octobre 2023

# Description succincte

Le projet a pour but d'écrire un programme qui va analyser deux dossiers et les synchroniser dans une direction.
Le programme va s'appeler lp25-backup. Il prendra 2 paramètres obligatoires, `source` et `destination`. Des options seront possibles pour ajuster son comportement.

# Architecture du programme

Pour permettre une analyse rapide, les traitements seront parallélisés. Il y aura 2 types de processus (en plus du main) :

- le listeur d'arborescence, qui va parcourir les fichiers et dossiers et établir une liste complète (avec des chemins complets également) à transmettre aux analyseurs de fichiers et dossiers;
- l'analyseur de fichiers, qui va extraire pour les fichiers leurs taille, dates de modification et accès, et calculer l'empreinte MD5, et retourner le résultat au listeur d'arborescence;

# Options du programme

--date-size-only : ne pas faire la comparaison des empreintes MD5, se limiter à comparer les dates et les tailles.
-n \<nombre> : nombre de processus analyseur **par côté** (il y en aura donc 2 fois ce nombre en parallèle)
--no-parallel : exécuter tout le programme dans un seul processus.

# Fonctionnement global

Lors de sont lancement, le programme reçoit deux paramètres (plus d'éventuelles des options) qui sont le dossier source et le dossier de destination (dans cet ordre, le premier paramètre est la source, le second est la destination). Une liste de l'arborescence est construite (avec ses propriétés) pour chacun des deux dossiers. Il sera important dans l'étape du parcours récursif de ne pas traiter les éléments suivants :

- `.` et `..` qui mèneraient le programme à tourner indéfiniment en traitant toujours le même dossier;
- les fichiers autres que les dossiers ou les fichiers réguliers (devices, fifo, sockets, liens symboliques seront ignorés)

Le résultat de cette étape est, pour chacun des deux dossiers, une liste (ce sera une liste chaînée) de l'arborescence complète du dossier, limitée aux fichiers réguliers et aux dossiers. Les autres types de fichiers (liens symboliques, device en mode caractère ou bloc, tubes et sockets, seront ignorés). Les listes devront être ordonnées. Les éléments de la liste seront définis par leurs chemins complets à partir du dossier considéré. Par exemple, l'arborescence suivante prise à partir du dossier source :

```bash
.
├── code
│   ├── arbre.c
│   ├── arbre.h
│   ├── liste.c
│   ├── liste.h
│   ├── liste_v2.c
│   ├── liste_v2.h
│   ├── main_arbre.c
│   ├── main_list.c
│   ├── main_listv2.c
│   └── Makefile
├── arg-retour
└── arg-retour.c
```

Générera une liste contenant les chemins :

- `"arg-retour"`
- `"arg-retour.c"`
- `"code/arbre.c"`
- `"code/arbre.h"`
- etc.

L'ordre peut respecter la casse ou pas, mais il faut que ce soit cohérent entre les listes de la source et de la destination.

Une fois les deux listes constituées, le programme va les comparer. En parcourant la liste de la source, il va rechercher dans la destination les entrées inexistantes ou modifiées depuis. Toutes les différences seront stockées dans la liste des différences (elle aussi composée de dossiers et de fichiers). Une fois cette liste établie, la dernière étape consiste à créer les dossiers de la source inexistants dans la destination, et à copier les fichiers inexistants dans la destination, ou modifiés depuis la dernière copie (taille et date, et éventuellement somme de contrôle MD5). La copie a une particularité : il faut qu'elle préserve la date de dernière modification du fichier source. Il faudra donc modifier la date de modification du fichier copié pour qu'elle soit égale à celle du fichier source (voir la fonction `utimensat`).

# Squelette du code

Un squelette de code vous est fourni. Vous devrez le compléter, et éventuellement proposer de nouvelles fonctions si cela vous paraît intéressant (pour faciliter la compréhension, la réutilisabilité, etc.)

Le code est commenté pour définir le rôle et le fonctionnement des fonctions.

# Relation avec les TD et les TP

Le projet ne peut pas être réalisé entièrement dès le début des TD et TP en C de LP25. En effet, les notions utilisées par le projet sont développées au fur et à mesure des séances LP25. Vous avancerez donc progressivement, en vous assurant de bien implémenter les fonctions demandées (et notamment, avec un contrôle des erreurs qui ne laisse pas passer d'erreur faisant planter le programme).

Les différentes parties remarquables du projet sont les suivantes :
- Analyse des options et paramètres du programme, réalisable dès TD1 et TP1
- Gestion des listes de fichiers et dossiers, réalisable à partir de TD et TP 1
- Parcours de l'arborescence de la source et de la destination, réalisable à partir des TD 3 et 4 et TP 2, et 3
- Gestion des processus et des files de message, à partir du TD5 et du TP4.
