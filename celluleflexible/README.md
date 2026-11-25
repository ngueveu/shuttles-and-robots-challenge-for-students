# Dossier Cellule Flexible

## 1) Contenu du dossier

Ce répertoire regroupe toute la partie "bas niveau" de la simulation, contenant à la fois l'environnement virtuel, la documentation technique, et l'espace de travail ROS 2 permettant de faire le pont entre les commandes et les actionneurs/capteurs.

Voici le détail du contenu de chaque sous-dossier :

### 1.1) CoppeliaSim

Ce dossier contient le logiciel de simulation robotique.

* **Version utilisée** : Nous utilisons actuellement la version **V4.3.0 Edu**.
* **Fichiers importants pour la simulation** :
  * Dans le répertoire `models/montrac`, nous retrouvons les fichiers `.ttm` correspondant aux modèles 3D de nos navettes. Ils apparaissent dans la scène CoppeliaSim après avoir rentré le nombre de navettes voulues.
  * À la racine de ce dossier se trouve le plugin `simROS2Interface`. C'est l'élément central qui permet de réaliser la communication et l'échange de messages entre nos scripts `.lua` (dans CoppeliaSim) et notre espace de travail ROS 2 (`ros_ws`).
  * Les autres fichiers de ce dossier sont des fichiers natifs Coppelia, qui n'ont très certainement jamais été modifié ni utilisés. La plupart de ces dossiers contiennent surtout des exemples et des aides. Il serait nécessaire de faire un tri.
* **En cas de mise à jour (MAJ)** : Si vous décidez de passer à une version plus récente de CoppeliaSim (comme la V4.10.0), sachez que l'API Lua a subi de nombreuses modifications. Cela impliquera de devoir réécrire et adapter les codes `.lua` intégrés dans la scène `.ttt` (changement du nom de certaines fonctions natives), ainsi que de recompiler potentiellement le plugin `simROS2Interface` pour l'adapter à la nouvelle architecture du logiciel. De plus, il faudra surement aussi adapter l'ensemble des objets de l'arborescence de la scène de la simulation (robot : joint, actionneur...), ce qui devrait représenter un travail assez important compte tenu de l'exhaustivité des objets physiques de la scène 'simulation4robots'
* **Protocole pour lancer CoppeliaSim** : En se plaçant dans CoppeliaSim, nous pouvons facilement lancer le logiciel CoppeliaSim par la commande suivante :

```bash
   ./coppeliaSim.sh
```
Pour visualiser la scène (.ttt) ou les modèles (.ttm), on fait alors un glisser-déposer dans CoppeliaSim.

### 1.2) sim

C'est ici que se trouve l'environnement de la simulation. On y retrouve la scène principale de notre simulation (le fichier `.ttt`), ainsi que l'ensemble des scripts `.lua` appelés par les différents éléments de la maquette virtuelle (capteurs, actionneurs, etc.).
*(Pour plus de détails, voir le readme du dossier 'sim')*

### 1.3) Doc

Ce répertoire rassemble la documentation annexe. On y retrouve notamment l'historique détaillant comment a été réalisé ce projet au fil des années, ainsi que des photos utiles de la maquette réelle de la MFJA. On les met dans ce dossier pour ensuite les appeler et les visualiser dans les ReadMe.

### 1.4) ros_ws

Il s'agit de l'espace de travail (workspace) ROS 2. Il contient le code source (`src`) des différents packages développés pour la cellule flexible (aiguillages, commande, navettes, robots, etc.) ainsi que les dossiers générés lors de la compilation (`build`, `install`, `log`).

**Point important** : tous les dossiers build, install et log sont créés par la compilation (colcon build), et peuvent donc être supprimés à tout moment, puis recréés par cette commande.

*(Pour plus de détails, voir le readme du dossier 'ros_ws')*

### 1.5) forTER

Ce dossier contient des outils et scripts spécifiquement destinés à l'évaluation du projet TER des étudiants. On y retrouve principalement deux fichiers essentiels :

* **`checker.py`** : Il s'agit d'un script d'évaluation écrit en Python. Son rôle est de comparer les données de production attendues (le cahier des charges défini dans un fichier de configuration) avec ce qui a réellement été exécuté lors de la simulation (via la lecture du fichier `.log`).
* **Ce qu'il vérifie** : Il s'assure qu'il n'y a pas d'erreurs dans l'exécution : respect du bon enchaînement des tâches sur les 8 postes, respect des durées de chaque opération, vérification du nombre de produits créés et évacués (limité à 6 types de produits et 5 tâches max par produit). Il calcule également des métriques de performance comme le *makespan* (temps de production) moyen.


* **`XXX_XXX_Doxyfile_config`** : C'est un fichier de configuration conçu pour **Doxygen**, un outil permettant de générer automatiquement une documentation complète et navigable (en HTML) à partir des commentaires du code source C++ de l'espace de travail. Il permet de réaliser le script4 de documentation dans le TER Atelier flexible.

### 1.6) Autres fichiers
Nous avons également deux autres fichiers dans ce dossier :
* Le fichier **`log.txt`**, où est écrit à l'aide du fichier LogManager (commande_locale) les différentes étapes de la simulation. On y retrouve par exemple l'arrivée des nouveaux produits, avec le temps d'apparition et le numéro d'identification. Cela permet à la fin de la simulation de vérifier que toutes les actions de la séquence définie ont bien été réalisées.
* Le fichier **`launch.sh`**, qui permet de lancer la simulation proprement. C'est ce fichier qui va afficher un message de bienvenue lors de l'entrée dans la simulation, et qui va lancer toute la simulation.

## 2) Protocole complet d'exécution avec ROS 2

Il existe différents moyens de lancer la simulation générale. Ce protocole d'execution est utile en tant que développeur pour tester les noeuds et les scripts .lua sans trop rentrer dans le détail des packages, avant de passer par la procédure d'exécution à partir de scripts dans le dossier 'cellule_flexible_coppelia-2025-2026/etu' (procédure "étudiant", voir ReadMe 'etu').

* Ouvrez un terminal

* Exécutez :
    ```bash
        cd ros_ws
        source /opt/ros/jazzy/setup.bash
        colcon build
        source install/setup.bash
        cd ../
    ```

   Avec :
   * **cd ros_ws** : Rentre dans le dossier ros-ws pour compiler les packages.
   * **`source /opt/ros/jazzy/setup.bash`** : Charge les commandes globales de ROS 2 dans le terminal.
   * **`colcon build`** : Compile l'ensemble des paquets présents dans le dossier `src`. Cela crée les dossiers build, install et log.
   * **`source install/setup.bash`** : Rend vos paquets fraîchement compilés exécutables par le terminal courant.
   * **cd ros_ws** : Retourne dans le dossier initial.


* Exécutez :
    ```bash
        ./launch.sh
    ```

  Cette ligne permet de lancer la simulation comme un étudiant, mais à partir des dossiers bas niveau. En effet, elle affiche un message de lancement dans la console, et exécute ensuite le fichier de démarrage principal (launch_alpha), situé dans le package `launcher`, qui va instancier tous les éléments de la simulation.
  
  Pour plus de détails, voir les readme des dossiers `/ros_ws/src` et `/ros_ws/src/launcher`, qui expliquent plus en détails que fait ce script `launch_alpha`.

