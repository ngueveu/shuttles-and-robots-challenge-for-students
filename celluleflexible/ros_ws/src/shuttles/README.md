# Package Shuttles

## 1. Description générale

Le but de ce package est de gérer la logique, le suivi et le contrôle des navettes (shuttles) tout au long de la simulation.
Il fonctionne via un nœud centralisant (le `ShuttleManager`) qui maintient une cartographie en temps réel des navettes sur le circuit. Pour ce faire, il utilise un système de files d'attente dynamiques se basant sur les retours des capteurs de présence et la position des aiguillages. Il agit également comme un annuaire permettant aux autres nœuds (notamment les robots) de savoir quelle navette se trouve devant eux.

## 2. Composition

Ce dossier est composé de 4 éléments principaux :

* Un fichier `package.xml` définissant les métadonnées et les dépendances du paquet (`rclcpp`, `std_msgs`, `aiguillages`, etc.).
* Un fichier `CMakeLists.txt` configuré pour ROS 2, gérant la compilation du code C++ et la génération des interfaces personnalisées (messages et services).
* Des dossiers `msg` et `srv`, permettant de définir les structures de communication personnalisées :
  * `MsgErreur.msg` 
  * `MsgShuttleChange.msg` 
  * `MsgShuttleCreate.msg` 
  * `ShuttleId.srv` (pour interroger l'ID d'une navette)
* Un dossier `src`, où se trouve le code source C++ et composé des fichiers suivants :
  * `main_ShuttleManager.cpp`, qui constitue le cœur du programme et la boucle principale.
  * `FileAttente.cpp` et `FileAttente.h`, qui définissent la logique des queues pour modéliser le déplacement des navettes de tronçon en tronçon.
  * `capteurs.cpp` et `capteurs.h`, qui gèrent la récupération et le stockage des états des capteurs de la simulation.



## 3. Description détaillée des fichiers src

### 3.1. Fichier main_ShuttleManager.cpp

La fonction main est le code principal du package. Elle exécute, dans l'ordre :

* L'initialisation de ROS 2.
* La création du nœud `Shuttle_manager` et de ses abonnements aux topics de commande (`nbNavettes`, `shutdown`, `ShuttleManagerDisplay`).
* La création du service `get_id_shuttle_at_poste` permettant aux robots de demander l'ID de la navette présente à une position donnée.
* L'initialisation d'un ensemble de 35 objets `FileAttente` modélisant les différents segments et zones d'attente du circuit de transit.
* Une boucle principale (tournant à 25 Hz) qui met à jour l'état des capteurs, détecte les fronts descendants, et fait transiter les navettes d'une file d'attente à la suivante selon l'état des aiguillages.
* L'affichage conditionnel d'un mode de débogage listant le contenu des files en temps réel.

### 3.2. Fichiers FileAttente.cpp et FileAttente.h

Ces fichiers encapsulent la gestion du trafic des navettes :

* Le constructeur *FileAttente*, qui initialise la file avec son aiguillage associé et ses successeurs (droite et gauche).
* La fonction *maj*, qui détermine vers quel tronçon (successeur) la navette doit être envoyée en fonction de la position actuelle de l'aiguillage (bute droite ou gauche).
* La fonction *get_first_navette*, qui retourne l'ID de la navette en tête de file.
* Les fonctions *add_navette_in_queue* et *delete_navette_in_queue*, qui ajoutent ou retirent une navette de la structure de données `queue`.
* Les fonctions *get_id_aiguillage* et *get_queue*, qui retournent respectivement l'identifiant de l'aiguillage et la file complète.

### 3.3. Fichiers capteurs.cpp et capteurs.h

Ces fichiers s'occupent de l'interface avec la simulation matérielle :

* Le constructeur *Capteurs*, qui initialise les tableaux d'états et s'abonne au topic `/commande/Simulation/Capteurs`.
* La fonction *Callback_capteurs_simulation*, qui met à jour les tableaux internes (PS, DD, DG, CP, CPI) à la réception d'un message.
* Des accesseurs (getters) tels que *get_PS*, *get_DD*, *get_DG*, *get_CP*, *get_CPI* pour lire individuellement l'état d'un capteur spécifique.

### 3.4. Sécurités mises en place

Pendant l'exécution, le code sécurise la simulation à travers plusieurs mécanismes et messages d'erreur :

* **Service Sécurisé** : Dans la fonction `shuttle_at_poste`, si un robot demande l'ID d'une navette à un emplacement invalide ou inexistant, le système renvoie la valeur `66` (ID vide) au lieu de crasher, évitant ainsi un Segmentation Fault.
* **Erreur d'Aiguillage** : "J'ai perdu une navette [...] car un aiguillage n'était pas en butée...". Se déclenche dans `FileAttente::maj` si une navette passe un aiguillage qui n'a pas fini de tourner (ni totalement à droite, ni totalement à gauche). Renvoie l'ID `-1`.
* **Signalement ROS 2 d'erreur** : Si le gestionnaire détecte une perte de navette (renvoi de `-1`), il publie un `MsgErreur` avec le `code=4` sur le topic `/commande/Simulation/Erreur_log`.
* **Sécurité mémoire** : Protection de l'affichage de débogage (`activate_debug_display`) en utilisant des copies de files d'attente pour l'itération, prévenant les fuites ou les accès concurrents non désirés sur les vecteurs principaux.

## 4. Utilisation

Il faut préalablement avoir fait les commandes suivantes dans le workspace ROS (`ros_ws`) :

```bash
  source /opt/ros/jazzy/setup.bash
  colcon build
  source install/setup.bash
```

* Pour lancer le noeud :
Exécuter la ligne suivante permet de lancer le nœud gestionnaire des navettes :
```bash
  ros2 run shuttles shuttle_manager
```
"shuttle_manager" est l'exécutable du package. Il lance main_ShuttleManager, capteurs et FileAttente.

* Pour activer les logs de débogage en temps réel dans le terminal, il faut publier sur le topic dédié :
```bash
  ros2 topic pub /commande/ShuttleManagerDisplay std_msgs/msg/Byte "data: 1" -1
```



## 5. Protocole de Test
Il a fallu, pour la migration du code de ROS1 à ROS 2, tester unitairement ce package. Un protocole de test a été établi pour s'assurer du bon fonctionnement des structures de données (queues) et de la stabilité du nœud (correction des Segmentation Faults).

Ainsi, dans ce protocole, nous envoyons les messages sur les topics associés à la place de CoppeliaSim (notamment l'état des capteurs et l'initialisation du nombre de navettes). Ce protocole n'est donc pleinement testable manuellement que si CoppeliaSim n'est pas opérationnel (n'envoie pas les messages simultanément sur les topics et ne pollue pas les tests).


