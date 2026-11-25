# Package Aiguillages

## 1. Description générale

Le but de ce package est de gérer les aiguillages de la simulation (les faire tourner à gauche, à droite, les verrouiller et les déverrouiller). Ce paquet contrôle l'ensemble des 12 aiguillages (A01 à A12) de manière centralisée et asynchrone.

## 2. Composition

Ce dossier est composé de plusieurs éléments essentiels :

* Un fichier `package.xml` définissant les métadonnées et dépendances du paquet (rclcpp, std_msgs, opencv, etc.).
* Un fichier `CMakeLists.txt` configuré pour ROS 2 (Jazzy) et C++17, gérant la compilation et la génération des interfaces.
* Un dossier `msg`, permettant de définir les structures de messages pour la communication interne, incluant `Capteurs.msg`, `Actionneurs.msg`, `ExchangeSh.msg` et `MsgSensorState.msg`.
* Un dossier `src`, contenant le code source C++ :
  * `main_aiguillage.cpp`, qui initialise et lance le nœud de gestion globale.
  * `Aiguillage.cpp` et `Aiguillage.h`, qui définissent la logique de contrôle et d'asservissement des aiguillages.



## 3. Description détaillée des fichiers src

### 3.1. Fichier main_aiguillage.cpp

La fonction `main` constitue le point d'entrée du package. Elle exécute séquentiellement :

* L'initialisation de ROS 2.
* La création de l'objet de type `Aiguillage`.
* Le déploiement d'un `MultiThreadedExecutor` configuré avec 15 threads (permettant de dédier un thread par aiguillage avec de la marge), remplaçant ainsi le `spinOnce` de ROS 1 pour traiter les requêtes en parallèle.
* L'écoute d'un signal `shutdown_callback` qui permet de stopper proprement l'exécuteur et le nœud si un message d'arrêt est reçu sur le topic dédié.

### 3.2. Fichiers Aiguillage.cpp et Aiguillage.h

#### 3.2.1. Détails des fonctions utilisées

Ces fichiers contiennent la logique métier. Voici le détail de chaque fonction :

* Le constructeur `Aiguillage` : Il initialise la fréquence de boucle d'attente à 25Hz, configure un groupe de callbacks de type `Reentrant` pour le parallélisme, instancie les abonnements (subscribers) et publications (publishers), et met à zéro les tableaux d'états booléens des capteurs (`aig_g` et `aig_d`).
* La fonction `switch_sensor_callback` : Exécutée à la réception de l'état des capteurs, elle décode une variable entière en utilisant des masques binaires (opérations bit-à-bit : 2 pour le premier aiguillage, 4 pour le 2eme, 16 pour le 3e...) pour mettre à jour en direct les états gauche/droite des 12 aiguillages.
* La fonction `gauche_callback` : Publie l'ordre de déverrouillage et la commande pour pivoter à gauche. Elle contient une boucle d'attente qui scrute les capteurs jusqu'à confirmation du mouvement effectif, puis envoie l'ordre de verrouillage final.
* La fonction `droite_callback` : Même principe que la fonction `gauche_callback`, mais dédiée à l'orientation et à la validation de la position droite de l'aiguillage.
* La fonction `shutdown_callback` : Intercepte les messages du topic d'arrêt global pour déclencher un `rclcpp::shutdown()`.

#### 3.2.2. Sécurités mises en place

Pour pallier d'éventuels soucis d'intégration avec le simulateur, des mécanismes de sécurité ont été implémentés :

* **Anti "Race Condition" de Lua** : Au sein des boucles d'attente (`gauche_callback` et `droite_callback`), si le capteur indique que l'aiguillage reste anormalement bloqué dans la position opposée (ex: forcé à droite alors qu'on demande la gauche), le système réitère volontairement l'envoi de la commande de déverrouillage et de mouvement pour forcer la priorité face au script du simulateur.
* **Architecture multithread reentrante** : L'utilisation de `MultiThreadedExecutor` et de souscriptions `Reentrant` évite qu'un aiguillage bloqué dans sa boucle d'attente ne paralyse le contrôle ou le rafraîchissement sensoriel des autres aiguillages de la ligne de production.



## 4. Utilisation

Pour compiler et utiliser le code, il faut préalablement avoir exécuté les commandes suivantes à la racine de votre workspace ROS 2 (ros_ws) :

```bash
  source /opt/ros/jazzy/setup.bash
  colcon build --packages-select aiguillages
  source install/setup.bash
```

Pour exécuter le noeud pilotant le contrôle des aiguillages, nous faisons :

```bash
  ros2 run aiguillages run_Aiguillage
```

L'exécutable généré s'appelle `run_Aiguillage`. Par cette commande, nous verrons alors apparaitre le message "Nœud démarré avec MultiThreadedExecutor.". Il faudra ensuite publier sur les topics de direction pour faire tourner un aiguillage à droite ou à gauche.


## 5. Protocole de Test
Il a fallu, pour la migration du code de ROS1 à ROS2, tester unitairement ce package. Pour cela, un protocole de test a été établi, à retrouver dans le fichier "tests_du_package_aiguillages".
Ainsi, dans ce protocole, nous envoyons les messages sur les topics associés à la place de CoppeliaSim. Ce protocole n'est donc valable que **si CoppeliaSim n'est pas opérationnel**. 




