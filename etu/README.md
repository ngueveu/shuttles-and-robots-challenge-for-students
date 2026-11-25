# Dossier etu

## 1) Composition des dossiers

Ce dossier est composé de :
* Un dossier **`dataprodconfig`**, contenant des exemples de séquences pour les étudiants. Chaque séquence définit :
  * Un vecteur Prod_type, qui définit le type de produits à faire (définit la couleur du premier cube du produit)
  * Un vecteur Prod_seqdeposte, qui définit quel est l'ordre de passage du produit sur les postes
  * Un vecteur Prod_dureeparposte, qui définit combien de temps dure chaque tâche
  * Un vecteur Prod_qte, qui définit combien d'objet de chaque type sont à produire
* Un dossier **`exemple`**, contenant des exemples de réseau de Pétri déjà fonctionnels. Nous avons alors les trois scripts de commande suivants :
  * `TeamEMPTY.main_commande.cpp` : Ce fichier est une coquille vide. Il a pour vocation de servir de base vierge et peut être utilisé pour réinitialiser le code étudiant en cas de besoin ou d'erreur majeure dans votre propre fichier de commande.
  * `TeamSALLAG.main_commande.cpp` : Il s'agit d'une implémentation "en dur" des développeurs précédents, conçue spécifiquement pour l'instance de données `data4_3_2_n0.prodconfig.h`. Ce fichier a été conçu pour faire la vidéo de présentation du ReadMe général. Cependant, il n'est aujourd'hui plus d'actualité, l'emplacement de départ des navettes et `data4_3_2_n0.prodconfig.h` ayant changés entre temps.
  * `Tuto_Basique.main_commande.cpp` : Ce code correspond au tutoriel du sujet d'atelier flexible. Il met en place un réseau de Petri très basique et permet de vérifier que la simulation CoppeliaSim fonctionne correctement de bout en bout (compilation, exécution et génération des résultats). Il ne fonctionne pas pour tous les jeux de données, et ne permet que de valider le niveau le plus bas du TER.
* Un dossier **`output`**, contenant de la documentation utile à la fin d'une simulation. Cette documentation est créée et disponible à partir du script4 (voir protocole de lancement plus bas).
* Un fichier **`XXX_XXX.main_commande.cpp`**, qui est une copie de `TeamEMPTY.main_commande.cpp`. Le nom de ce fichier va être modifié par les étudiants avec leurs initiales, et va servir de support pour leur réseau de Pétri. La dernière partie montre les fonctions à utiliser par les étudiants.
* Des scripts à exécuter, qui sont :
  * `script0.sh`, qui sert à compiler la simulation
  * `script1.sh`, qui sert à lancer la simulation
  * `script2.sh`, qui sert à importer les résultats de la simulation
  * `script3.sh`, qui sert à valider l'exécution de la simulation
  * `script4.sh`, qui sert à créer la documentation de l'exécution
 
  Chaque script met une erreur s'il n'est pas correctement appelé. La partie suivante montre comment exécuter proprement ces codes depuis un terminal.


## 2) Consigne pour les étudiants 

### 2.1) Fichier à modifier

  Lors de la **première séance du TER uniquement**, renommer le fichier principal XXX_XXX.main_commande.cpp en y mettant vos initiales (majuscule, sans espace, sans accent, sans symboles spéciaux) par la commande suivante (ou manuellement):
```bash
	mv XXX_XXX.main_commande.cpp VOS_INITIALES.main_commande.cpp
```

  Ce fichier sera **le seul à être modifié par les étudiants**. Il leur permettra d'écrire leur réseau de Pétri pour respecter les consignes.


### 2.2) Consignes pour lancer la simulation

* **Pour compiler la simulation**

A **chaque nouvelle ouverture d'un terminal** directement **dans le dossier etu**, on fait :
```bash
	source script0_Compile.sh VOS_INITIALES dataprodconfig
```

Veillez à remplacer VOS_INITIALES par le nom du fichier .cpp que vous voulez utiliser. Remplacez également dataprodconfig par une des séquences disponible dans dataprodconfig.

Par exemple, si vos initiales sont SUN_SC (et donc le fichier commande associé est SUN_SC.main_commande.cpp), et que le fichier data à considérer est dataP34_1_1_n0.prodconfig.h, alors l'instruction pour compiler sera : 
```bash
	source script0_Compile.sh SUN_SC dataP34_1_1_n0
```

Il faut veillez à exécuter cette ligne de commande **à chaque nouvelle modification de votre fichier** VOS_INITIALES.


* **Pour lancer la simulation**
Après avoir vérifié qu'aucune erreur de compilation ne s'est précédemment produite, dans le même terminal, exécuter :
```bash
	./script1_Run.sh 
```

* **Pour importer les résultats de la simulation**

Après avoir choisi  "4-Fin programme" dans le menu de la fenêtre XTERMINAL, on peut faire :
```bash
	./script2_GetSimuOutput.sh VOS_INITIALES NOM_FICHIER_DATA
```

Après chaque exécution sans bug de la simulation et après l'import des résultats avec le script2, un fichier log nommé "VOS_INITIALES_NOM_FICHIER_DATA.log.txt" doit apparaitre dans le dossier "output". 
Si ce fichier n'existe pas, c'est qu'il y a eu une erreur lors de votre simulation.

Par exemple, si vos initiales sont SUN_SC (et donc le fichier commande associé est SUN_SC.main_commande.cpp), et que le fichier data à considérer est dataP34_1_1_n0.prodconfig.h, alors l'instruction pour compiler sera : 
```bash
	./script2_GetSimuOutput.sh SUN_SC dataP34_1_1_n0
```

* **Pour vérifier la validité de l'exécution de la simulation**

Le fichier log précédemment généré permet de vérifier la validité de votre simulation par rapport aux données d'entrée.

Pour ce faire, depuis la racine du dossier "etu", vous pouvez exécuter:

```bash
	./script3_Validation.sh VOS_INITIALES NOM_FICHIER_DATA
```

Cela vous dit alors si la simulation s'est passée correctement.


* **Pour générer la documentation**

Pour générer la documentation associée à votre code, depuis la racine du dossier "etu", exécuter:
```bash
	./script4_Documentation.sh VOS_INITIALES
```

Pour accéder à la documentation, aller dans le dossier output/docfromcpp/html et ouvrir index.html. A partir de ce fichier, aller dans Files/File
List et choisir celui correspondant à notre fichier. Cela permet d'afficher les détails sur la simulation. On y retrouve par exemple les commentaires de chaque étape du réseau de Pétri. 


### 2.3) Exemple à exécuter

Pour vérifier que la simulation Coppelia fonctionne correctement, on utilise le fichier Tuto_Basique contenu dans le dossier exemple :

```bash
	cp exemple/Tuto_Basique.main_commande.cpp .
	source script0_Compile.sh Tuto_Basique dataP2_1_1_n0
	./script1_Run.sh
	./script2_GetSimuOutput.sh Tuto_Basique dataP2_1_1_n0
	./script3_Validation.sh Tuto_Basique dataP2_1_1_n0
	./script4_Documentation.sh Tuto_Basique
	rm Tuto_Basique.main_commande.cpp
```

## 3) Création du réseau de Pétri
Pour réaliser leur réseau de Pétri, les étudiants auront recours aux fonctions suivantes :
* *cmd.Stop_PS(actionneur)*, qui permet de fermer un PS, c'est-à-dire que la navette s'arrêtera quand elle arrivera à cet actionneur
* *cmd.Ouvrir_PS(actionneur)*, qui permet d'ouvrir un PS, c'est-à-dire que la navette passera quand elle arrivera à cet actionneur
* *capteur.get_CP(capteur)*, qui retourne true si une navette est présente sur le capteur CP
* *capteur.get_CPI(capteur)*, qui retourne true si une navette est présente sur le capteur CPI
* *capteur.get_PS(capteur)*, qui retourne true si une navette est présente sur le capteur PS
* *capteur.get_DG(aiguillage)* et *capteur.get_DD(aiguillage)*, qui permettent de vérifier qu'un aiguillage est bien à droite ou à gauche
* *aiguillage.Droite(aiguillage)* et *aiguillage.Gauche(aiguillage)*, qui permet d'orienter un aiguillage à droite ou à gauche
* *robot.DeplacerPiece(robot,position1,position2)*, qui permet de faire déplacer une pièce à un robot d'une position 1 à une position 2
* *robot.FinDeplacerPiece(robot)*, qui renvoit true lorsque le déplacement est terminé
* *robot.FaireTache(poste,durée)*, qui permet de faire une tâche sur un certain poste durant une certaine durée
* *robot.TacheFinie(poste)*, qui renvoit true lorsque la tâche est terminée
* *robot.AjouterProduit(poste,numero_produit)*, qui ajoute sur un poste un produit donné
* *robot.Evacuer()*, qui fait disparaitre un produit sur le Poste3

On rappelle que :
* Le type du produit doit être compris entre 1 et 6
* Le nombre de tâches à faire par produit doit être compris entre 1 et 5
* L'évacuation se fait impérativement par le poste 3
* Un poste ne peut faire qu'une seule tâche d'un seul produit en même temps
* Interdiction de circuler sur les voies près des postes si ce n'est pour y amener/récupérer un produit
