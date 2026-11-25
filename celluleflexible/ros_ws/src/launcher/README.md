# Package Launcher

## 1. Description générale

Le but de ce package est de centraliser et d'automatiser le lancement des multiples nœuds ROS 2 composant la simulation. Plutôt que de démarrer manuellement chaque programme (robots, aiguillages, gestionnaires) dans des terminaux séparés, ce paquet utilise des scripts Python pour orchestrer leur démarrage simultané, tout en ouvrant automatiquement des fenêtres dédiées pour chaque noeud démarré (aiguillages, commande, robot 1, robot 2...).

## 2. Composition

Ce dossier est composé de trois éléments principaux :

* Un fichier `package.xml` définissant les métadonnées et dépendances du paquet (rclcpp, std_msgs, etc.).
* Un fichier `CMakeLists.txt` configuré pour ROS 2, dont le rôle principal ici est d'installer le dossier `launch` dans le répertoire d'exécution (`share`) du système ROS.
* Un dossier `launch`, contenant les scripts Python d'orchestration :
  * `launch_alpha.launch.py`
  * `launch_beta.launch.py`

Contrairement aux autres paquets, celui-ci ne possède pas de dossier `src` actif. Toute la logique réside dans les fichiers de lancement.


## 3. Description détaillée des fichiers launch

### 3.1. Fichier launch_alpha.launch.py

Ce fichier sert de lanceur basique. Ses actions sont les suivantes :

* Déclaration d'un argument de lancement nommé `nbRobot`, dont la valeur par défaut est fixée à '4'.
* Lancement de l'exécutable `simulation` appartenant au package `commande_locale`, lançant les 3 codes principaux de celui-ci.
* Ouverture automatique du nœud dans une nouvelle fenêtre terminale via la commande `xterm` intégrée dans le préfixe de lancement (`xterm -geometry '80x20+0+0' -e`).

Ce fichier est lancé depuis le script1 du dossier etu lors de l'exécution par l'étudiant. C'est de lui que part toute la simulation.


### 3.2. Fichier launch_beta.launch.py

Ce fichier est le lanceur principal et avancé de la cellule de production. Il se charge de :

* Récupérer l'argument `nbRobot` pour configurer dynamiquement la simulation.
* Lancer systématiquement les nœuds de contrôle globaux : le nœud `run` (package `commande`), le nœud `run_Aiguillage` (package `aiguillages`), `log_manager` (package `commande_locale`) et `main_ShuttleManager` (package `shuttles`).
* Lancer les bras robotiques `Robot1` et `Robot2` de manière inconditionnelle. Il lance également 'Robot3' et 'Robot4' dans le cas d'une simulation à 4 robots.
* Organiser visuellement l'espace de travail de l'utilisateur en attribuant des coordonnées précises (`-geometry`) à chaque fenêtre `xterm` générée, évitant ainsi la superposition des terminaux.

Ce fichier est lancé depuis le code principal de commande_locale.

### 3.3. Sécurités et conditions mises en place

Pour rendre le système modulable, une logique conditionnelle a été intégrée :

* **Conditionnement des robots** : L'exécution des nœuds `Robot3` et `Robot4` est protégée par une condition `IfCondition`. Ces robots ne seront démarrés que si l'argument `nbRobot` fourni au lancement est strictement égal à '4'.


## 4. Utilisation

Pour compiler et utiliser le code, il faut préalablement avoir exécuté les commandes suivantes à la racine de votre workspace ROS 2 :

```bash
  source /opt/ros/jazzy/setup.bash
  colcon build
  source install/setup.bash
```

* **Lancement de l'environnement complet (Beta)** :

Pour lancer la simulation avec le nombre de robots par défaut (4 robots), exécutez :

```bash
  ros2 launch launcher launch_beta.launch.py
```

* **Lancement partiel avec arguments** :

Pour lancer la simulation avec seulement 2 robots (ce qui ignorera le démarrage des robots 3 et 4), ajoutez l'argument en fin de commande :

```bash
  ros2 launch launcher launch_beta.launch.py nbRobot:=2
```

Ces deux fonctions devraient permettre de lancer la simulation en intégralité.

## 5. Protocole de Test

Ce package ayant pour vocation l'orchestration, les tests sont purement visuels et structurels. Son test doit être réalisé uniquement une fois que l'ensemble des autres paquets ont été migré et testé :

1. Assurez-vous d'avoir l'utilitaire `xterm` installé sur votre système (requis par le préfixe des lanceurs).
2. Lancez le fichier `launch_beta.launch.py` avec `nbRobot:=4`.
3. Vérifiez que 8 fenêtres terminales distinctes s'ouvrent à l'écran sans se superposer totalement (Commande, Menu, Aiguillages, ShuttleManager, et les 4 Robots).
4. Stoppez le lancement (Ctrl+C), puis relancez avec `nbRobot:=2`. Vérifiez que seules 6 fenêtres s'ouvrent, confirmant l'absence des terminaux pour les robots 3 et 4.


