# Package commande

## 1. Description générale

Ce fichier sert à développer le réseau de Pétri et se charge de traduire les envois de message sur des topics en fonction simples pour les étudiants. Dans le réseau de Pétri les étudiants auront recours au fonctions :
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

Ce fichier sert aussi bien à contrôler toute la simulation sur CoppeliaSim qu'à contrôler la ligne réelle physique.

## 2. Composition
Ce dossier est composé de 3 éléments:
* Un fichier package.xml
* Un fichier CMakeLists.txt
* Un dossier src, où se trouve le code source C++ et composé des fichiers suivants :
  * main_commande.cpp, qui contient le réseau de pétri haut niveau des étudiants
  * actionneurs.cpp et actionneurs.h, qui transforme les tableaux de données en nombres entiers, pour les redistribuer soit à CoppeliaSim, soit à la vraie ligne physique
  * AigsInterface.cpp et AigsInterface.h, qui sert d'intermédiaire entre le réseau de Pétri et les aiguillages
  * capteurs.cpp et capteurs.h, qui s'occupent de récupérer les données des capteurs et de les traduire pour donner l'état individuel de chaque capteur
  * commande.cpp et commande.h, qui sert de soutien à main_commande et qui permet d'arrêter la navette
  * communicationbaxter.cpp et communicationbaxter.h, qui 
  * RobotsInterface.cpp et RobotsInterface.h, qui gère les mouvements des robots
 
Tous ces fichiers sont lancés à partir d'un exécutable noté "run".


## 3. Description détaillée des fichiers src
### 3.1. Fichier main_commande.cpp
Ce fichier est le fichier principal de la simulation, et c'est celui-là qui sera modifié par les étudiants. Il contient le réseau de Pétri modifié par les étudiants et des fonctions d'initialisation.
Ce fichier est composé de la fonction *display*, qui affiche à chaque changement le marquage du Réseau de Pétri et de la fonction *main*, qui :
* Initialise les robots, les aiguillages et les capteurs
* Attend la fin de l'initialisation des robots
* Demande à l'utilisateur le nombre de navettes voulu
* Contient l'intégralité du réseau de Pétri (appelle dedans des fonctions pour faire les tâches, tourner des aiguillages, stopper les navettes...)


### 3.2. Fichiers actionneurs.cpp et actionneurs.h
Ces fichiers sont composés des fonctions suivantes (en plus du constructeur, qui sert à initialiser les variables du fichier dans un état sécuritaire) : 
* *Envoyer*, qui prend en entrée de grands tableaux de booléens et qui les prépare pour l'envoi
* *Ecrire_ligne_STx*, *Ecrire_ligne_RxD*, *Ecrire_ligne_RxG*, *Ecrire_ligne_PIx*, *Ecrire_ligne_Vx*, *Ecrire_ligne_Dx*, qui servent à envoyer des ordres sur la vrai ligne physique au bon endroit
* *publish_actionneurs_ligne*, qui prend les nombres envoyés par WRITE et qui les envoie à l'automate physique
* *publish_actionneurs_simulation*, qui publie pour la simulation CoppeliaSim
* *WRITE*, qui transforme les tableaux de booléens en nombres entiers, pour les envoyer à CoppeliaSim ou aux automates Siemens

Ce fichier sert donc autant à la simulation CoppeliaSim qu'à la vraie ligne physique.

### 3.3. Fichiers AigsInterface.cpp et AigsInterface.h
Ce fichier est composé de deux fonctions, une pour envoyer un message pour tourner l'aiguillage à droite et une pour tourner l'aiguillage à gauche. Ces messages sont envoyés directement sur des topics lus par le package aiguillages.
C'est de fichier qui permet à l'étudiant de n'avoir qu'à marquer "aiguillage.Droite" ou "aiguillage.Gauche" dans le réseau de Pétri pour tourner ceux-ci.


### 3.3. Fichiers capteurs.cpp et capteurs.h
C'est le fichier qui permet aux étudiants de faire les instructions suivantes :
* *capteur.get_CP(capteur)*, qui retourne true si une navette est présente sur le capteur CP
* *capteur.get_CPI(capteur)*, qui retourne true si une navette est présente sur le capteur CPI
* *capteur.get_PS(capteur)*, qui retourne true si une navette est présente sur le capteur PS
* *capteur.get_DG(aiguillage)* et *capteur.get_DD(aiguillage)*, qui permettent de vérifier qu'un aiguillage est bien à droite ou à gauche

En plus de ces fonctions, nous avons aussi :
* *Callback_capteurs_simulation*, qui le message reçu (MsgSensorState) en donnée pour les différents capteurs. Ainsi, elle isole les données renvoyées par chaque capteur depuis un message unique.
* *MASK*, qui extrait la valeur d'un seul capteur à partir d'un grand nombre en entrée (contenant tous les états de tous les aiguillages)

### 3.3. Fichiers commande.cpp et commande.h
Ce fichier contient les instructions étudiantes suivantes :
* *cmd.Stop_PS(actionneur)*, qui permet de fermer un PS, c'est-à-dire que la navette s'arrêtera quand elle arrivera à cet actionneur
* *cmd.Ouvrir_PS(actionneur)*, qui permet d'ouvrir un PS, c'est-à-dire que la navette passera quand elle arrivera à cet actionneur

En plus, nous retrouvons les fonctions :
* *Initialisation*, qui initialise les capteurs et actionneurs : on met tous les capteurs à 0, et tous les actionneurs à 1, donc en STOP
* *PlayCallback*, *PauseCallback* et *getPlay* qui servent de mémoire à la simulation : elle met une variable play à true ou à false en fonction de l'état de la simulation, et la renvoie avec *getPlay*.
* *DeverouilleAiguillagesCallback* et *VerouilleAiguillagesCallback*, qui mettent à jour les variables lock et unlock pour les aiguillages
* *AiguillagesgauchesCallback* et *AiguillagesdroitsCallback*, qui met à jour l'état des aiguillages
* *SortirErgot* et *RentrerErgot*, qui mettent à jour l'état des pins

### 3.3. Fichiers communicationbaxter.cpp et communicationbaxter.h
Si dans le menu principal nous choisissons de travailler non pas sur la Simulation mais sur l'Atelier, alors ce code nous sert. On publie alors sur des topics propres aux robots réels.

Ce fichier est composé des fonctions :
* *Callback_prise_effectuee_bras_droit* et *Callback_prise_effectuee_bras_gauche*, où le robot annonce que la pièce a bien été attrapée
* *Callback_attente_prise_bras_droit* et *Callback_attente_prise_bras_gauche*, qui met à jour l'état de la mémoire pour dire que le robot est prêt à prendre la pièce
* *Prise_effectuee_bras_droit* et *Prise_effectuee_bras_gauche*, qui renvoit true si la pièce a correctement été prise
* *Attente_prise_bras_droit* et *Attente_prise_bras_gauche*, qui renvoit true si on attend de prendre une pièce
* *Demander_prise_bras_droit* et *Demander_prise_bras_gauche*, qui met un message de demande à true
* *Afficher_Communication_Baxter*, qui permet d'afficher en temps réel l'état du bras du robot
* *Update*, qui publie les booléens sur le réseau ROS2


### 3.3. Fichiers RobotsInterface.cpp et RobotsInterface.h
Ce fichier contient les instructions étudiantes suivantes :
* *robot.DeplacerPiece(robot,position1,position2)*, qui permet de faire déplacer une pièce à un robot d'une position 1 à une position 2
* *robot.FinDeplacerPiece(robot)*, qui renvoit true lorsque le déplacement est terminé
* *robot.FaireTache(poste,durée)*, qui permet de faire une tâche sur un certain poste durant une certaine durée
* *robot.TacheFinie(poste)*, qui renvoit true lorsque la tâche est terminée
* *robot.AjouterProduit(poste,numero_produit)*, qui ajoute sur un poste un produit donné
* *robot.Evacuer()*, qui fait disparaitre un produit sur le Poste3

En plus, nous retrouvons les fonctions définies dans le package Robot, qui permettent par exemple d'envoyer un bras robot dans une certaine position ou de contrôler la pince.




## 4. Utilisation
Pour lancer le code depuis le répertoire ros, on fait d'abord :
```bash
  source /opt/ros/jazzy/setup.bash
  colcon build
  source install/setup.bash
```

Ensuite, on exécute la ligne suivante, qui permet d'exécuter tous les fichiers :
```bash
  ros2 run commande run 4
```
L'avant dernier mot ("run") correspond au nom de l'exécutable. Le dernier chiffre correspond au nombre de robots voulus, sachant que nous acceptons seulement le nombre 2 et le nombre 4. 

Au lancement de ce fichier, nous n'aurons rien, car la première étape consiste à attendre la fin de l'initialisation des robots. Pour obtenir l'exécution du réseau de Pétri, il faut donc que tous les robots aient fini leur initialisation.

