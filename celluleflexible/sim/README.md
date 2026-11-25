# Détail dossier sim

## 1. Description générale

Le but de ce  répertoire est de gérer la logique interne des objets de la simulation CoppeliaSim et d'établir le pont de communication avec le worksapce ROS 2.  Afin de rendre plus accessible ces codes pour les modifier, il a été choisi de les mettre dans ce répertoire, que CoppeliaSim charge pour intégrer les codes dans la scène de la simulation ('Simulation4robots.ttt'), plutôt que de d'ouvrir la scène dans le logiciel de simulation, dont l'accès aux codes et la modification sont compliqués (arborescence complexe).
Chaque élément dynamique de la scène (capteurs, actionneurs, navettes, robots) exécute son propre script Lua. Ces scripts s'abonnent à des topics ROS 2 pour recevoir des commandes (ex: bouger un aiguillage, changer une couleur, fermer une pince) et publient des données sur d'autres topics pour informer le système C++ de l'état de l'environnement (ex: présence d'une navette).

## 2. Composition

Ce dossier est composé de 3 éléments principaux à sa racine :

* Un fichier `Simulation4Robots.ttt` qui est la scène CoppeliaSim physique (14.0 MB).
* Un fichier `README.md` servant de documentation.
* Un dossier `lua_files`, où se trouve le code source Lua exécuté par les éléments de la simulation. Il est composé des éléments suivants :
  * `main_script.lua`, qui orchestre la simulation globale.
  * Des scripts liés aux capteurs (`RailSensor.lua`, `StationSensor.lua`, `StopSensor.lua`, `SwitchSensor.lua`, `Vision_sensor.lua`).
  * Des scripts liés aux actionneurs (`aiguillages.lua`, `Stop.lua`).
  * Un sous-dossier `LBR_iiwa_14_R820` pour la logique embarquée sur les bras robotiques.
  * Un sous-dossier `shuttle` pour l'affichage des navettes.



## 3. Description détaillée des fichiers lua_files

### 3.1. Fichier main_script.lua

Ce fichier est le chef d'orchestre de la scène. Lors de son initialisation, il effectue les actions suivantes :

* Vérification de la présence du plugin `simROS2` pour s'assurer que la communication est possible.
* Création des abonnements (Subscriptions) et publications (Publishers) globaux pour le contrôleur (Start, Pause, Stop simulation, LogManager).
* Création de dizaines d'interfaces de communication spécifiques pour les 4 robots de la scène (nœuds `robot1` à `robot4`). Il gère ainsi les services pour obtenir le temps (`GetTime`), récupérer les identifiants d'objets (`GetObjectHandle`), modifier/lire l'état des articulations (`SetJointState`, `GetJointState`) et changer la couleur des produits ou des navettes (`ChangeColor`, `ChangeShuttleColor`).

### 3.2. Fichiers Capteurs (Sensors)

Ces fichiers gèrent la détection physique dans la scène et remontent l'information à ROS 2 :

* **`RailSensor.lua`**, **`StationSensor.lua`**, **`StopSensor.lua`**, **`SwitchSensor.lua`** : Ces scripts lisent l'état de multiples capteurs de proximité (ex: CP01-CP10 pour les rails, PS01-PS24 pour les stops). Pour optimiser l'envoi, l'état de tous les capteurs d'une même famille est encodé dans un seul entier (somme de puissances de 2) puis publié sur un topic dédié (type `std_msgs/msg/Int32`).
* **`Vision_sensor.lua`** : Il récupère l'image sous forme de chaîne binaire depuis un capteur de vision, la convertit, et formate l'en-tête pour la publier sur `/camera/image_raw` au format `sensor_msgs/msg/Image`.

### 3.3. Fichiers Actionneurs (Actuators)

* **`aiguillages.lua`** : Gère les 12 moteurs des aiguillages (A01 à A12). Il s'abonne à trois topics (`SwitchControllerRight`, `SwitchControllerLeft`, `SwitchControllerLock`) pour verrouiller et pivoter les aiguillages selon des angles précis (-120 degrés ou 0). Il publie également l'état réel des aiguillages sur `/sim_ros_interface/aig`.
* **`Stop.lua`** : Contrôle 24 actionneurs physiques (ST01 à ST24) servant à bloquer les navettes. Il écoute les topics `StopController` et `GoController` et utilise des opérations binaires (`boolAnd32`) pour lever ou abaisser de -0.015 mètre les butées correspondantes.

### 3.4. Dossier LBR_iiwa_14_R820

* **`ProduitTransporte_X.lua`** : Attribué à chaque robot. Il écoute un topic booléen (ex: `/commande/Simulation/TransportBrasX`). Si la valeur est `true`, il rend le produit virtuel (représentant la pièce tenue par la pince) visible en passant sa transparence à 1. Sinon, il le rend transparent (0).
* **`RG2_X.lua`** : Gère l'actionnement mécanique des pinces des robots. Il s'abonne à un topic dédié (ex: `/robot/cmdPinceRobotX` de type `std_msgs/msg/Int32`). À la réception d'un message, il applique une force de 20 N au moteur de la pince (`RG2_openCloseJoint`) :
  * Si la donnée vaut **1** : Fermeture de la pince (vitesse de -0.10 m/s).
  * Si la donnée vaut **0** : Ouverture de la pince (vitesse de 0.10 m/s).



### 3.5. Dossier shuttle

* **`shuttleX.lua`** : Ces scripts rendent les navettes autonomes en vitesse et gèrent leur affichage.
* **`Mouvement`** : À l'aide des capteurs, la navette ajuste la vitesse de ses roues (avancer à 20, ralentir à 10, ou s'arrêter à 0 si obstacle détecté).
* **`Affichage`** : Le script écoute le topic `/[nom_de_la_navette]/set_colors` attendant un tableau de 6 entiers. Il met alors à jour la couleur et la transparence des 6 emplacements de produits sur la navette grâce à une table de correspondance de couleurs RGB pré-définie.



## 4. Annexe : Encodage binaire des capteurs

Pour optimiser les communications ROS 2, l'état de groupes de capteurs (Rails, Stops, Stations, etc.) est compressé dans un seul message `std_msgs/msg/Int32` plutôt que de publier un topic par capteur.

La valeur publiée est calculée via une somme de puissances de 2, où chaque bit correspond à l'état booléen (0 ou 1) d'un capteur.

**Exemple de décodage côté nœud C++ :**
Si le nœud ROS 2 reçoit la valeur **5** sur le topic `/sim_ros_interface/RailSensor` :

* 5 correspond en binaire à `...00000101`
* Cela signifie que $2^0$ (valeur 1) et $2^2$ (valeur 4) sont actifs.
* En vous référant à `RailSensor.lua`, le capteur **CP01** correspond à $2^0$ et le capteur **CP03** correspond à $2^2$. Ce sont donc ces deux capteurs qui détectent une présence.
* En C++, vous pouvez tester l'état d'un capteur précis via un masque binaire : `if (msg->data & (1 << index_du_capteur))` où l'index commence à 0.

## 5. Utilisation

* Lancement de la simulation :
1. Lancez le logiciel CoppeliaSim (entrez dans la console ./coppeliaSim.sh depuis le fichier CoppeliaSim).
2. Ouvrez la scène en allant dans `File > Open scene...` et sélectionnez `sim/Simulation4Robots.ttt`.
3. Appuyez sur le bouton "Play" (Start/Resume Simulation). Les scripts Lua tenteront de s'abonner et de publier sur le réseau ROS 2 via le plugin `simROS2`.
