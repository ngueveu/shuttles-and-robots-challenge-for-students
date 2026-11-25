# Espace de travail ROS 2 : ros_ws

Ce répertoire constitue l'espace de travail (workspace) ROS 2 du projet Cellule Flexible. Il contient l'ensemble des paquets (packages) développés pour faire le lien entre la simulation physique sous CoppeliaSim et le Réseau de Pétri.

## 1. Structure globale et fichiers standards

Dans cet espace de travail, vous trouverez les dossiers générés par la compilation (`build`, `install`, `log`) ainsi que le dossier source (`src`) contenant les différents paquets. Les fichiers `build`, `install` et `log` sont recréés à chaque compilation, et donc peuvent être supprimés.

De manière générale, chaque paquet ROS 2 contenu dans `src` possède la structure suivante :

* **`package.xml`** : Ce fichier définit les métadonnées du paquet (nom, version, description) et, surtout, liste l'ensemble de ses dépendances (comme `rclcpp` pour C++, `std_msgs` pour les messages standards, etc.) nécessaires à sa compilation et à son exécution.
* **`CMakeLists.txt`** : C'est le fichier de configuration pour l'outil de compilation CMake. Il indique comment compiler les codes source C++, quels exécutables générer, et comment construire les interfaces de communication personnalisées.
* **`msg/` et `srv/`** : Ces dossiers contiennent les définitions des interfaces de communication personnalisées. Les fichiers `.msg` (Messages) définissent les structures de données échangées sur les *topics* (communication asynchrone), et les fichiers `.srv` (Services) définissent les structures requête/réponse.
* **`src/`** : Ce dossier contient les codes source C++ (fichiers `.cpp` et `.h`) qui font tourner les nœuds ROS.

## 2. Rôle des packages et interactions

Voici un aperçu du rôle de chaque package et de la manière dont ils interagissent entre eux et avec CoppeliaSim :

### 2.1. L'interface avec CoppeliaSim : `commande_locale`

Ce paquet est l'interface avec la simulation. C'est lui qui démarre CoppeliaSim, charge la scène et fait apparaître les navettes.

Il agit comme un traducteur : il récupère les données brutes des capteurs depuis CoppeliaSim (via le plugin `simROS2Interface`) et les traduit en messages ROS binaires compréhensibles par les autres paquets. Inversement, il reçoit les ordres des autres paquets et les traduit en commandes exécutables par les actionneurs de CoppeliaSim. Il gère également l'historique de production via le module `LogManager`.

*(Pour plus de détails, voir le README du paquet `commande_locale`)*

### 2.2. La logique de commande : `commande`

C'est ici que réside le code de haut niveau (le Réseau de Pétri) développé par les étudiants. Ce paquet simplifie l'interaction en traduisant des instructions simples (ex: `cmd.Stop_PS()`, `robot.DeplacerPiece()`) en publications de messages complexes sur les *topics* ROS appropriés. Il s'appuie sur les données traduites par `commande_locale` pour prendre ses décisions.

*(Pour plus de détails, voir le README du paquet `commande`)*

### 2.3. La gestion matérielle : `aiguillages` et `robots`

Ces paquets écoutent les ordres envoyés par le paquet `commande`.

* **`aiguillages`** : Contrôle l'orientation et le verrouillage des 12 aiguillages de manière centralisée et multithreadée.
* **`robots`** : Gère les 4 bras robotiques et les postes de travail. Chaque bras est un nœud indépendant qui exécute les séquences de prise/dépose de pièces et gère les chronomètres de fabrication, tout en assurant l'animation visuelle (changement de couleur/opacité) dans CoppeliaSim.

*(Pour plus de détails, voir les README des paquets `aiguillages` et `robots`)*

### 2.4. La supervision du trafic : `shuttles`

Le `ShuttleManager` maintient une cartographie en temps réel des navettes. En écoutant l'état des capteurs (via `commande_locale`), il modélise le déplacement des navettes de tronçon en tronçon à l'aide de files d'attente. Les robots interrogent ce paquet (via un service) pour savoir quelle navette se trouve devant eux avant d'interagir avec une pièce.

*(Pour plus de détails, voir le README du paquet `shuttles`)*

### 2.5. Le lien avec la maquette réelle : Paquets Schneider (obsolètes)

Lors de projets précédents, des paquets (nommés `schneider`, `schneider_103`, etc.) avaient été créés pour permettre au code de contrôler la véritable maquette physique (les automates Siemens) au lieu de la simulation. Vous pouvez voir des traces de ce lien avec la maquette physique dans d'autres fichiers (comme `communicationbaxter.cpp` dans `commande`).
Cependant, suite au déménagement du matériel à la MFJA, les configurations réseau et matérielles ont changé, entraînant la perte de la méthode pour les faire fonctionner correctement. Ces fichiers nécessiteraient une refonte majeure pour être réutilisables sur l'installation physique actuelle. Nous les avons supprimés pour nettoyer l'espace du workspace et laisser uniquement les noeuds opérationnels, mais ils pourront faire l'objet d'une migration dans un futur projet.

## 3. Lancement global de la simulation

### 3.1. Le rôle du package `launcher`

Plutôt que de lancer chaque nœud (robots, aiguillages, commande) manuellement dans plusieurs terminaux, le paquet `launcher` automatise ce processus via des scripts Python de type `launch`. Il orchestre le démarrage simultané de tous les éléments nécessaires.

*(Pour plus de détails, voir le README du paquet `launcher`)*

### 3.2. Protocole complet d'exécution

Voici comment lancer la simulation de bout en bout, et l'explication de chaque commande :

**Étape 1 : Préparation de l'environnement**
Ouvrez un terminal dans le répertoire `ros_ws` et exécutez :

```bash
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

* **`source /opt/ros/jazzy/setup.bash`** : Charge les commandes globales de ROS 2 dans le terminal.
* **`colcon build`** : Compile l'ensemble des paquets présents dans le dossier `src`. Cela crée les dossiers build, install et log.
* **`source install/setup.bash`** : Rend vos paquets fraîchement compilés exécutables par le terminal courant.

**Étape 2 : Lancement de l'orchestrateur de base**

```bash
ros2 launch launcher launch_alpha.launch.py
```

* **Ce que fait cette commande** : Elle appelle le fichier `launch_alpha.launch.py` du paquet `launcher`.
* **Qu'est-ce qui run** : Ce fichier lance principalement l'exécutable `simulation` du paquet `commande_locale` (qui démarre CoppeliaSim, charge la scène, et met en place les traducteurs de signaux). Il ouvre également une nouvelle fenêtre terminal pour ce nœud central.

**Étape 3 : Lancement de la logique et des actionneurs (via la boucle de commande locale)**
Une fois CoppeliaSim lancé par l'étape 2, le code de `commande_locale` va lui-même déclencher (de manière automatisée) le lancement du niveau supérieur de la simulation. Ce code va alors appeler `launch_beta.launch.py` (du paquet `launcher`), qui va instancier et ouvrir des terminaux séparés pour tous les packages suivants :
* L'exécutable `run` (Réseau de Pétri du paquet `commande`).
* L'exécutable `run_Aiguillage` (gestion centralisée des aiguillages).
* L'exécutable `ShuttleManager` (supervision des navettes).
* L'exécutable `log_manager` (gestion des logs, voir section 4).
* Les exécutables `Robot1` à `Robot4` (contrôle indépendant de chaque bras).

À ce stade, l'ensemble des paquets interviennent, communiquent entre eux, et la simulation démarre physiquement dans CoppeliaSim en fonction du Réseau de Pétri programmé.

## 4. Gestion des logs et historique

L'historique de production et la trace des événements importants (évacuation, apparition de produits, erreurs, fin de séquence) sont gérés par le **`LogManager`**.
Ce code source (`LogManager.cpp`) se trouve à l'intérieur du paquet **`commande_locale`**. Lorsqu'il est exécuté (automatiquement par le lanceur beta), il crée ou met à jour le fichier texte `log.txt` (situé à la racine du dossier `celluleflexible`), qui sera ensuite utilisé par le script `checker.py` (dans le dossier `forTER`) pour valider le travail des étudiants.

## Dépendances entre packages
Nous avons identifié les relations de dépendance suivantes :
— Package aiguillages : ne dépend d’aucun autre
— Package commande : dépend de shuttles, commande_locale, aiguillages et robots
— Package commande_locale : dépend de aiguillages
— Package launcher : dépend de commande_locale, commande, aiguillages, robots et shuttles
— Package robots : dépend de commande_locale et shuttles
— Package shuttles : dépend de aiguillages

Dans le cas d'une modification globale à faire sur tous les fichiers (comme une migration), il convient de commencer par le fichier avec le moins de dépendances (aiguillages) et de finir par celui avec le plus de dépendances (launcher).
