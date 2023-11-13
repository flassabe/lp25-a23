Sujet en version du 7 novembre 2023

# Description succincte

Le projet a pour but d'écrire un programme qui va analyser deux dossiers et les synchroniser dans une direction.
Le programme va s'appeler lp25-backup. Il prendra 2 paramètres obligatoires, `source` et `destination`. Des options seront possibles pour ajuster son comportement.

# Architecture du programme

Pour permettre une analyse rapide, les traitements seront parallélisés. Il y aura 2 types de processus (en plus du main) :

- le listeur d'arborescence, qui va parcourir les fichiers et dossiers et établir une liste complète (avec des chemins complets également) à transmettre aux analyseurs de fichiers et dossiers;
- l'analyseur de fichiers, qui va extraire pour les fichiers leurs taille, dates de modification et accès, et calculer l'empreinte MD5, et retourner le résultat au listeur d'arborescence;

# Options du programme

- --date-size-only : ne pas faire la comparaison des empreintes MD5, se limiter à comparer les dates et les tailles.
- -n \<nombre> : nombre de processus analyseur **par côté** (il y en aura donc 2 fois ce nombre en parallèle)
- --no-parallel : exécuter tout le programme dans un seul processus
- -v pour verbose (affichage des listes et des opérations)
- --dry-run pour exécution de test (juste lister les opérations à faire, ne pas faire les copies réellement)

# Fonctionnement global

Lors de son lancement, le programme reçoit deux paramètres (plus d'éventuelles des options) qui sont le dossier source et le dossier de destination (dans cet ordre, le premier paramètre est la source, le second est la destination). Une liste de l'arborescence est construite (avec ses propriétés) pour chacun des deux dossiers. Il sera important dans l'étape du parcours récursif de ne pas traiter les éléments suivants :

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

# Ordre d'implémentation conseillé

L'ordre proposé vous permettra d'avancer de manière optimale en implémentant de plus en plus des fonctionnalités demandées, de façon modulaire et de manière à pouvoir les tester. L'ordre est composé d'ensembles de fonctionnalités orthogonales, c'est à dire indépendantes les unes des autres. Chaque ensemble devra être testé autant que possible sur des exemples simples pour s'assurer qu'il fonctionne.

- Ensemble 1
	- Analyse des options du programme (voir la section option des instructions)
	- Gestion des listes chaînées de fichiers
- Ensemble 2
	- Opérations de tests sur les répertoires (existence, écrivable, etc.)
	- Recherche des informations de fichiers/dossiers (stat)
	- Calcul de la somme de contrôle des fichiers (MD5). Peut se tester manuellement sur des fichiers en comparant le résultat de l'implémentation avec l'appel à la commande md5sum
- Ensemble 3
	- Analyse séquentielle (pas de processus)
	- Copie des fichiers à synchroniser (avec conservation des droits et de la `mtime`)
- Ensemble 4
	- Implémentation multiprocessus
		- Création des processus
		- Gestion de la MQ
		- Implémentation du protocole entre processus
- Ensemble 5
	- Option verbose
	- Option dry-run
	- Exécution de la bonne implémentation (parallèle ou non) en fonction de l'option de parallélisme

# Précisions sur le sujet

Le sujet du projet peut sembler complexe (et il l'est). Cette complexité est réduite par la fourniture du squelette de code, et par la proposition d'ordre d'implémentation. Cependant, il est certain que des questions se poseront lors de l'implémentation (ne dit-on pas que le diable est dans les détails ?). Certaines trouveront naturellement une réponse après un peu de réflexion et de travail d'équipe dans le groupe projet. D'autres, en revanche, peuvent vous présenter un obstacle sérieux. Dans ce cas, n'hésitez pas à poser vos questions (en séances de cours/TD/TP, ou par e-mail) à vos enseignants, à commencer par moi-même (j'ai implémenté une version quasi complète du projet avant de le publier, j'ai donc été confronté à ces questions).
La durée du projet est courte, ne perdez pas de temps sur des questions auxquelles nous avons sans doute des réponses.

# Implémentation

Les détails de l'implémentation sont donnés indirectement par les signatures de fonctions dans les .h, conjointement à des commentaires doxygen en entête des définitions de fonctions (fichiers .c). Je détaille également ci dessous des aspects spécifiques à la version parallèle du programme, qui fait appel à deux notions importantes dans les systèmes distribués par passage de messages :
- Protocole
- Programmation par événements

## Protocole entre processus

Un protocole est un ensemble de règles de communication et de formats de messages pour permettre les interactions entre systèmes ou processus amenés à coopérer. Il y a donc deux points importants pour un protocole :
- les messages, définis par leurs champs (type, longueur)
- les communications (qui communique avec qui, quel(s) message(s), quelles doivent être les réactions à la réception d'un message)

### Messages

Le format des messages vise à permettre l'échange de toutes les informations nécessaires à l'exécution du protocole par ses différents éléments (expéditeur, destinataire, etc.). Les sous-sections suivantes décrivent les différents types de messages et leur usage par les processus (rappel : il y a 3 types de processus, le main, les listers, et les analyseurs).

#### Commande simple

C'est une commande avec deux champs : le numéro de destinataire sur la file de messages, et le code de commande comme second champ :

```c
typedef struct {
    long mtype;
    char message;
} simple_command_t;
```

#### Commande d'analyse de fichiers

Cette commande comporte 3 champs : le destinataire, le code de commande et une entrée de liste de fichiers `files_list_entry_t`. Cette dernière comporte tous les champs de la structure `files_list_entry_t`.

```c
typedef struct {
    long mtype;
    char op_code; // Contains the analyze file opcode
    files_list_entry_t payload;
} analyze_file_command_t;
```

Les champs de l'entrée de liste de fichiers ne sont pas encore affectés, à l'exception du type (`DT_FILE` ou `DT_DIR`) et du chemin du fichier à analyser. Les autres champs seront remplis par le processus, qui fera pour cela appel à la fonction `stat` ainsi qu'au calcul de somme MD5 (voir annexe du sujet pour plus d'informations sur l'usage de la libcrypto et des fonctions de hachage de OpenSSL). Une fois que les champs ont été remplis, le processus peut répondre en renvoyant le même message, dont le code de commande est une analyse terminée, et l'entrée complétée. Ce type de message peut être utilisé avec 3 signification (donc 3 `op_code` différents) :
- demande d'analyse à un processus analyseur
- réponse du processus analyseur au processus listeur
- envoi d'un élément de liste de fichier d'un listeur au main

#### Commande de listage d'un répertoire

Cette commande consiste à demander le listage d'un répertoire par un processus. Elle contient les deux champs incontournables `mtype` et `op_code` avec le destinataire et le code commande. Un troisième paramètre est transmis : la cible du listage, i.e. une chaîne de caractères contenant le chemin du répertoire à analyser.

```c
typedef struct {
    long mtype;
    char op_code;
    char dir_path[4096];
} analyze_dir_command_t;
```

Il est possible de ne pas envoyer 4096 caractères pour le chemin s'il en fait moins.

La réponse du processus, une fois la liste terminée, sera une séquence de messages qui permettront de transmettre la liste complète. Chaque message contiendra les champs suivants :

- le mtype du message
- le code de commande (soit une commande de type transmission d'entrée, soit fin de transmission de liste)
- une entrée de la liste, s'il s'agit d'une commande de transmission d'entrée.
- le numéro de canal de la MQ de l'expéditeur pour savoir à quelle liste ajouter l'entrée (source ou destination)

```c
typedef struct {
    long mtype;
    char op_code; // Contains the analyze file opcode
    files_list_entry_t payload;
    int reply_to; // MQ id of the sender, to build either source or destination list
} files_list_entry_transmit_t;
```

#### Demande de terminaison

La demande de terminaison est effectuée par le processus parent avec une `simple_command_t` et le code de commande de terminaison. Le processus recevant cette commande répondra au parent par une `simple_command_t` également, dont le code sera la confirmation de la terminaison.

Le type `simple_command_t` sera également utilisé par les listeurs pour signaler la fin de la transmission de leur liste de fichiers.

## Programmation par événements

La seconde spécificité de la programmation distribuée est de souvent partager les informations par passage de messages. En effet, un système distribué peut fonctionner entre des processus exécutés sur une même machine avec une mémoire partagée (ce qui provoque en retour des problèmes liés à la synchronisation des accès aux ressources partagées), ou sur des systèmes différents (ou via des méthodes de communications similaires dans les principes aux accès réseau).

Nous choisirons le fonctionnement par passage de messages via une file de messages. Cette méthode délègue la synchronisation des ressources à l'API des MQ. Elle suppose en revanche de raisonner non plus en séquence continue d'instructions, mais en réactions à des messages. Schématiquement, un processus va s'initialiser (ce qui peut provoquer l'émission de messages), puis, à chaque réception de message, il va mettre à jour son état, et éventuellement envoyer des messages pour partager ces mises à jour avec les autres noeuds du réseau. Ce type de fonctionnement est très fréquent dans les réseaux numériques et dans les réseaux de robots modulaires distribués (cf. journée de la recherche, stand FEMTO-ST/DISC/OMNI à Belfort).

L'implémentation est réalisée d'une manière simple par une boucle infinie (qui peut accepter son interruption lors de la réception de certains messages) qui va mettre le processus en attente de message. À la réception d'un message, celui est traité (générant toute opération possible, dont des envois), puis le processus se remet en attente d'un message. Appliqué à notre cas d'utilisation, on pourrait imaginer (dans les grandes lignes) :

- le main crée ses processus, puis envoie les commandes de listage de répertoires aux deux listeurs. Il rentre ensuite dans sa boucle infinie pour attendre des entrées de liste de fichiers (à ajouter aux listes qu'il gère). La boucle se termine quand le main a reçu les messages de fin de liste de chacun des listeurs. Il peut alors comparer les listes et faire la synchronisation entre la source et la destination.
- le listeur attend la demande d'analyse de répertoire. À sa réception, il crée une liste de fichiers (il n'y affectera que les chemins des fichiers), puis il envoie les `n` premiers éléments de la liste aux analyseurs (`n` paramétré). Il entre ensuite dans sa boucle, où il va tenir à jour un compte des requêtes d'analyse envoyées, et attendre des messages des analyseurs. À chaque réponse reçue d'un analyseur, il renvoie à celui ci l'entrée suivante. La boucle se termine quand les deux conditions suivantes sont remplies :

	- la liste à analyser est vide
	- toutes les requêtes ont obtenu leurs réponses

Alors, toutes les entrées à jour vont être transmises au main. Puis le processus s'arrêtera quand il recevra une demande de terminaison.
- Les analyseurs se mettent tout de suite en attente d'un message. Si celui ci est le message de terminaison, ils sortent de la boucle et se terminent. Si le message est un message d'analyse de fichier, ce dernier sera analysé (traitement selon dossier ou fichier)

# Proposition pour optimiser le déroulement

Proposer une idée d'amélioration (sans l'implémenter) du cheminement des requêtes et réponses pour que le programme s'exécute plus rapidement.

# Modalités d'évaluation

L'évaluation porte sur 3 éléments :

- Le rapport
	- un canvas vous sera fourni plus tard dans le semestre; vous devrez détailler plus précisément ce qui sera présenté à la soutenance, ainsi que des résultats quantitatifs et/ou qualitatifs de votre travail.
- La soutenance
	- une présentation de 10 minutes de tout le groupe (en équilibrant les temps de parole) dans laquelle :
		- vous rappellerez le contexte du projet, les grandes lignes de son fonctionnement (2 minutes)
		- vous expliquerez les choix que vous avez eu à faire et pourquoi (3 minutes)
		- vous présenterez les difficultés les plus importantes rencontrées et vos analyse à ce sujet (2 minutes)
		- vous proposerez un RETEX (retour d'expérience) dans lequel vous répondrez du mieux que possible à la question suivante : "_fort(e)s de notre expérience sur le projet, que ferions-nous différemment si nous devions le recommencer aujourd'hui ?_"
		- vous ferez une brève conclusion (1 minute)
	- vous répondrez ensuite à des questions posées par les enseignants pendant une dizaine de minutes.
- Le code
	- fourni dans un dépôt git avec l'historique des contributions de tous les membres,
	- avec un Makefile qui permet la compilation simplement avec la commande `make`
	- capacité à effectuer les traitements demandés dans le sujet,
	- capacité à traiter les cas particuliers sujets à erreur (pointeurs NULL, etc.)
	- Respect des conventions d'écriture de code
	- Documentation du code
		- Avec des commentaires au format doxygen en entêtes de fonction (si nécessaire)
		- Des commentaires pertinents sur le flux d'une fonction (astuces, cas limites, détails de l'algorithme, etc.)
	- Absence ou faible quantité de fuites mémoire (vérifiables avec `valgrind`)
	- **ATTENTION !** le code doit compiler sous Linux ! Un code non compatible avec un système Linux sera pénalisé de 5 points sur 20.

# Annexes

## Convention de code

Il est attendu dans ce projet que le code rendu satisfasse un certain nombre de conventions (ce ne sont pas des contraintes du langages mais des choix au début d'un projet) :

- indentations : les indentations seront faites sur un nombre d'espaces à votre discrétion, mais ce nombre **doit être cohérent dans l'ensemble du code**.
- Déclaration des pointeurs : l'étoile du pointeur est séparée du type pointé par un espace, et collée au nom de la variable, ainsi :
	- `int *a` est correct
	- `int* a`, `int*a` et `int * a` sont incorrects
- Nommage des variables, des types et des fonctions : vous utiliserez le *snake case*, i.e. des noms sans majuscule et dont les parties sont séparées par des underscores `_`, par exemple :
	- `ma_variable`, `variable`, `variable_1` et `variable1` sont corrects
	- `maVariable`, `Variable`, `VariableUn` et `Variable1` sont incorrects
- Position des accolades : une accolade s'ouvre sur la ligne qui débute son bloc (fonction, if, for, etc.) et est immédiatement suivie d'un saut de ligne. Elle se ferme sur la ligne suivant la dernière instruction. L'accolade fermante n'est jamais suivie d'instructions à l'exception du `else` ou du `while` (structure `do ... while`) qui suit l'accolade fermante. Par exemple :

```c
for (...) {
	/*do something*/
}

if (true) {
	/*do something*/
} else {
	/*do something else*/
}

int max(int a, int b) {
	return a;
}
```

sont corrects mais :

```c
for (int i=0; i<5; ++i)
{ printf("%d\n", i);
}

for (int i=0; i<5; ++i) {
	printf("%d\n", i); }

if () {/*do something*/
}
else
{
	/*do something else*/}
```

sont incorrects.
- Espacement des parenthèses : la parenthèse ouvrante après `if`, `for`, et `while` est séparée de ces derniers par un espace. Après un nom de fonction, l'espace est collé au dernier caractère du nom. Il n'y a pas d'espace après une parenthèse ouvrante, ni avant une parenthèse fermante :
	- `while (a == 3)`, `for (int i=0; i<3; ++i)`, `if (a == 3)` et `void ma_fonction(void)` sont corrects
	- `while(a == 3 )`, `for ( i=0;i<3 ; ++i)`, `if ( a==3)` et `void ma_fonction (void )` sont incorrects
- Basé sur les exemples ci dessus, également, les opérateurs sont précédés et suivis d'un espace, sauf dans la définition d'une boucle `for` où ils sont collés aux membres de droite et de gauche.
- Le `;` qui sépare les termes de la boucle `for` ne prend pas d'espace avant, mais en prend un après.
