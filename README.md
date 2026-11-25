# Projet Cellule Flexible MFJA

## 1) Partie informative étudiant

### 1.1) Vidéo de présentation
La vidéo suivante montre un exemple de ce que peut faire la simulation sur CoppeliaSim :

https://user-images.githubusercontent.com/84821679/198697250-7861b35a-a503-4246-86e2-939076bb784c.mp4

### 1.2) Présentation générale du projet
L’objectif de ce projet est d’amener les étudiants à concevoir puis à implémenter un réseau de Pétri afin d’en comprendre concrètement l’utilité et les applications.

Comme support pédagogique, nous utilisons une représentation virtuelle de la cellule flexible existante de la MFJA, en combinant ROS2 et CoppeliaSim.

Le réseau de Pétri ainsi implémenté permettra de piloter les navettes, les aiguillages et les bras robotiques de la simulation afin de reproduire une séquence de production donnée, à l’image du fonctionnement d’une véritable usine.

Ce TER a donc pour ambition de développer et de mettre en œuvre la capacité à produire automatiquement et en parallèle au sein d’un atelier flexible.


### 1.3) Fonctionnement de la simulation
La simulation prend en entrée une séquence de production, décrivant les produits à faire, ainsi que les tâches qui les composent.

A partir de cette séquence, nous allons avoir 8 postes de production qui vont réaliser les tâches à faire. Les produits seront représentés par un empilement de cubes de couleur : un cube pour le type de produit et un cube pour chaque tâche effectuée. Les cubes seront transparents quand la tâche sera en cours et opaque quand elle sera terminée.

A la fin d'une tâche, si la tâche suivante ne se fait pas sur le même poste, il faut déplacer le produit. Nous appelons alors le robot du poste sur lequel on est, soit pour déplacer le produit au poste conjugué (2 postes par robot), soit pour le déplacer à une navette. Les navettes servent à transporter un produit jusqu'à une autre paire de poste.

La navette va naviguer sur un circuit, comportant des capteurs de position, des actionneurs et des aiguillages. Cela permet de diriger les navettes et de les arrêter à un endroit voulu.

L'évacuation du produit se fait lorsque toutes ses tâches sont terminées.

Nous rappelons les règles à respecter : 
* Le type du produit doit être compris entre 1 et 6
* Le nombre de tâches à faire par produit doit être compris entre 1 et 5
* L'évacuation se fait impérativement par le poste 3
* Un poste ne peut faire qu'une seule tâche d'un seul produit en même temps
* Interdiction de circuler sur les voies près des postes si ce n'est pour y amener/récupérer un produit

Le schéma suivant permet de repérer les aiguillages, les capteurs, les postes et les robots:
![Image_Capteurs_et_Actionneurs](celluleflexible/Doc/CelluleSchema.png)


### 1.4) Lancement de la simulation
Nous avons plusieurs moyens de lancer la simulation. Cependant, le moyen le plus simple et celui utilisé par les étudiants consiste à exécuter les lignes suivantes (après avoir ouvert un terminal depuis ce dossier) :
* Pour aller dans le dossier etu :
  ```bash
     cd etu
   ```
* Pour récupérer l'exemple Tuto_Basique :
  ```bash
     cp exemple/Tuto_Basique.main_commande.cpp .
   ```
* Pour compiler les modifications :
  ```bash
     source script0_Compile.sh Tuto_Basique dataP2_1_1_n0
   ```
* Pour lancer l'exécution de la simulation :
  ```bash
     ./script1_Run.sh
   ```
   
   
   
## 2) Partie développement

### 2.1) Installation
Le protocole pour l'installation est à retrouver dans le fichier "tutoriel_installation".

### 2.2) Contenu de chaque dossier
Nous utilisons pour le moment la version **V4.3.0 Edu** de CoppeliaSim et la version **Jazzy** de ROS2.

Dans le dossier celluleflexible, nous retrouvons toute la partie bas niveau de la simulation, ainsi que la communication entre CoppeliaSim et ROS2.
Dans le dossier etu, nous retrouvons tout le matériel mis à disposition pour les étudiants, à savoir les scripts de lancement, des exemples de séquence de production et des exemples de réseau de Pétri.

Le logiciel CoppeliaSim est récupéré directement lors du téléchargement du dossier GitHub, mais il faut installer ROS2, ainsi que quelques extensions (voir "tutoriel_installation").


### 2.2) Pistes d'amélioration
Après la migration de ROS1 noetic à ROS2 jazzy, nous pensons qu'il serait bon de faire les tâches suivantes :
* **Mettre à jour et trier le dossier CoppeliaSim**.
  
  En effet, la simulation tourne aujourd'hui sous la version V4.3.0 mais la dernière version sortie est la V4.10.0. Il faudrait donc faire une mise à jour.
  
  Pour cela, il y a normalement peu de chances que les packages soient impactés, donc tout ce qui concerne ROS2 (ros_ws) devrait rester intact.

  Cependant, certaines fonctions sont maintenant dépréciées (mais sont encore censées marcher) dans Coppelia, comme les fonctions handle. Il faudra peut être donc changer quelques fonctions des scripts lua du dossier sim, en trouvant leurs équivalent dans la nouvelle version de Coppelia.

  Ce qui risque de poser le plus problème est le plugin entre ROS2 et CoppeliaSim. Aujourd'hui, pour ce faire, nous compilons le plugin simROS2interface (sinon message lors de l'ouverture de CoppeliaSim : "Plugin ROS2 not found"). Il faudrait voir si cela est toujours bon pour la nouvelle version, et en même temps regarder s'il n'existe pas une manière plus simple de faire.
  
  Nous pensons que le mieux serait de remplacer le dossier Coppelia par un autre dossier à jour (en rajoutant tout de même les fichiers .ttm des navettes se trouvant dans CoppeliaSim/models/montrac et en n'oubliant pas de compiler le simROS2interface), de lancer la simulation et de voir s'il y a des erreurs ou non. A partir de là, il faudrait regarder les erreurs affichées, les comprendre et les réparer une à une.

  Mettre à jour CoppeliaSim pourrait permettre, en plus d'avoir accès à de nouvelles fonctionnalités, d'avoir moins de crash, car les versions plus récentes supportent mieux les subscriptions multiples et le multi-thread. Le plugin ROS2 est également plus robuste. De plus, la version 4.3.0 est sortie avant l'existence de ROS2 jazzy (27 janvier 2022 pour V4.3.0 de Coppelia et 23 mai 2024 pour ROS2 Jazzy). Il se peut donc que certaines fonctionnalités de ROS2 jazzy ne soient pas compatibles avec cette ancienne version de Coppelia. Coppelia V4.3.0 pourra même devenir complètement obsolète avec les nouvelles mises à jour de ROS2.

  Pour plus de renseignements, consultez : https://manual.coppeliarobotics.com/en/versionInfo.htm.

  Il faudra aussi songer à faire le tri dans le dossier CoppeliaSim. En effet, nous avons décrit dans le ReadMe celluleflexible les dossiers utilisés, mais il faudrait arriver à enlever les fichiers inutiles du dossier, pour différencier ce qui relève d'exemples introduits par CoppeliaSim et ce qui est vraiment utile à son fonctionnement. Il faudrait ensuite faire un vrai ReadMe du dossier, permettant de comprendre l'utilité de chaque fichier restant.

* **Évacuation par n’importe quel poste**

  Dans l’architecture actuelle, l’évacuation des produits se passe de la manière suivante :
    * L’étudiant appelle la fonction robot.Evacuer() dans son réseau de Pétri (dossier etu)
    * Cela exécute la fonction RobotsInterface : :Evacuer() qui se trouve dans le package commande (fichier RobotsInterface.cpp), qui publie un message sur le topic /commande/Simulation/Evacuer.
    * Le seul recepteur de ce message se trouve dans le package robots, dans le fichier Robot.cpp. La réception de ce message va alors faire rentrer tous les robots dans la fonction Robot : :Evacuer. La condition à l’entrée "if(num_robot==2)" va alors permettre de ne faire exécuter la fonction qu’au robot 2 (les autres robots sortent aussitôt de la fonction). La variable "int position=1" va alors permettre de ne sélectionner que le poste 3.
    
  On voit donc que, pour le moment, l’évacuation des produits s’effectue automatiquement par le poste 3.
  Il faudrait donc changer les 3 codes cités ci-dessus. Nous proposons ici une idée pour permettre l’évacuation de tous les postes possibles, en gardant une architecture similaire :
    * L’étudiant appelle maintenant la fonction robot.Evacuer(numéro_poste) dans son réseau de Pétri (dossier etu).
    * Cela exécute la fonction RobotsInterface : :Evacuer(numéro_poste) qui se trouve dans le package
commande (fichier RobotsInterface.cpp), qui se charge de convertir le numéro de poste en un robot + une position. On envoie alors un message sur un des topics suivants, selon le numéro du robot : /commande/Simulation/Robot1/Evacuer, /commande/Simulation/Robot2/Evacuer,
/commande/Simulation/Robot3/Evacuer, /commande/Simulation/Robot4/Evacuer. Le message envoyé comporte la position voulue.
    * Le seul récepteur de ce message se trouve dans le package robots, dans le fichier Robot.cpp. La réception de ce message va alors faire rentrer le robot concerné dans la fonction Robot : :Evacuer(position). Nous aurons alors une condition à l’entrée en fonction de si position=1 ou position=4, ce qui nous permettra finalement d’évacuer sur le bon poste.
    
Pour réaliser cette amélioration, il pourrait être bon de tout d’abord réaliser l’évacuation par un autre poste (possiblement le poste conjugué, soit le poste 4), puis quand cela fonctionne, l’étendre à tous les postes.

En plus de toutes ces modifications, il faudrait mettre à jour le checker, pour dire si l’évacuation s’est faite du bon poste (celluleflexible/forTER/checker.py). Il faudrait aussi mettre à jour la vidéo de présentation (celle du ReadMe général) pour montrer aux étudiants qu’une évacuation de n’importe quel poste est possible. Il faudrait enfin mettre à jour les différents ReadMe et les consignes du TER Atelier Flexible.

* **Rendre libre le choix de quelles navettes on veut en début de simulation**
  
  Toutes les navettes ont une position de départ attribuée en fonction de leur numéro. Pour le moment nous avons la correspondance : Navette 0 = Shuttle Z, Navette 1 = Shuttle A, Navette 2 = Shuttle B, Navette 3 = Shuttle C, Navette 4 = Shuttle D, Navette 5 = Shuttle E, Navette 6 = Shuttle F. Si à la question "Combien voulez-vous de navettes?", nous répondons 3, alors nous verrons apparaitre Shuttle Z, Shuttle A et Shuttle B. Nous voulons maintenant que, au lieu de voir apparaitre Shuttle Z, Shuttle A et Shuttle B, la simulation nous demande lesquelles parmi les 7 navettes nous voulons faire apparaitre.
  
  Les navettes apparaissent après avoir demandé le nombre de navettes. Cela se passe dans le package commande_locale, dans vrepController, dans la fonction vrepController::loadModelInit. Cette fonction ouvre dans CoppeliaSim pour chaque navette un fichier consacré rangé dans CoppeliaSim/models/montrac + Nom de la navette (fichiers .ttm).
  
  Pour réaliser cette tâche, il faudrait rajouter une fonction qui, après avoir demandé le nombre de navettes voulues, montre à l'utilisateur les différents emplacements de navettes disponibles et lui demande d'en choisir le nombre correspondant au nombre de navettes demandé. Nous n'aurions donc plus qu'à changer la fonction vrepController::loadModelInit, pour lui dire d'intégrer les navettes voulues par l'utilisateur et non plus de les prendre seulement dans l'ordre. Les fichiers .ttm ne nécessitent pas d'être modifiés.
  
  Nous ne pensons pas que d'autres modifications (sur le checker par exemple) soient nécessaires, car le checker s'occupe plus spécifiquement de voir si la séquence de tâches a bien été réalisée, mais ne se préoccupe pas de la position des navettes. Il faudrait en revanche modifier le support consignes étudiant (sujet TER).

* **Pouvoir déplacer les points de départ des navettes**

  En suivant ce que nous disions pour le point précédent, la fonction vrepController::loadModelInit (dans commande_locale/vrepController) ne sert qu'à afficher les navettes aux endroits prédéfinis. Nous ne toucherons donc pas à cette fonction dans cette amélioration là.

  En revanche, ce sont les fichiers situés dans CoppeliaSim/models/montrac que nous allons devoir changer. Pour les ouvrir, mettez vous dans le dossier CoppeliaSim puis faites :

  ```bash
     ./coppeliaSim.sh
   ```

  Cela ouvre CoppeliaSim. Ensuite, effectuez un glisser-déposer de votre scène .ttt (comme Simulation4Robots.ttt) dans la fenêtre Coppelia. Effectuez ensuite un glisser-déposer de votre fichier .ttm dans la fenêtre Coppelia. Vous verrez alors apparaitre la navette dans la simulation.

  Après avoir fait cela, cliquez sur le dossier principal de la navette (s'appelle ShuttleA pour la navette A) dans Scene hierarchy (se situe tout à la fin). Cliquez ensuite sur le logo suivant dans la barre en haut :

  ![Logo_CoppeliaSim_Position](celluleflexible/Doc/LogoCoppeliaSimPosition.png)  

  Cela va ouvrir un menu. Allez dans le menu "Position" et changez X-coord, Y-coord et Z-coord à votre guise (mettre entrée à chaque fois permet de voir la position de la navette en temps réel dans la scène). Pour sauvegarder, allez dans File/SaveModelAs et écrasez l'ancienne sauvegarde .ttm. Rien ne sert de sauvegarder la scène .ttt.

  Répétez cette opération pour toutes les navettes, en changeant les coordonnées de chacune des navettes comme vous le souhaitez. En réouvrant la simulation, les navettes seront alors aux positions désirées.
  
* **Faire le lien entre la maquette réelle et la simulation**
  
  Le lien avait déjà été établi lors du projet 2022, sans pouvoir néanmoins être utilisé, à cause du déménagement à la MFJA. Les anciens packages schneider, schneider103 et schneider104 (ROS1) avaient été créés pour cela. Ces fichiers ne servant plus à la simulation, nous ne les avons pas traduits en ROS2 et les avons supprimés. Ils peuvent néanmoins être récupérés sur le GitHub. Ces fichiers s'abonnent à des topics déjà créés comme le topic d'aiguillage droite et gauche, et envoient l'ordre sur des topics propres à eux.
  
  Nous avons également retrouvé des traces de ce lien dans le package commande. En effet, le fichier *communication_baxter.cpp* sert à envoyer des ordres aux robots réels et une partie du fichier *actionneurs.cpp* sert à convertir les ordres pour la simulation réelle. Ces deux fichiers sont traduits en ROS2 mais la partie servant à la maquette réelle n'est pas utilisée.
  
  Enfin, dans le package commande_locale, lors de l'affichage du menu, nous avons encore le choix entre un mode Simu ou Atelier, qui permet soit une connexion à la simulation CoppeliaSim soit une connexion à l'atelier réel. Cette partie du menu est donc obsolète.
  
  Ces remarques ne sont pas une liste exhaustive, car d'autres fichiers doivent contenir également des traces. Pour faire ce lien correctement, il faudrait dans un premier temps enlever tout ce qui ne sert plus dans les codes (appartenant au projet 2022), puis recommencer proprement à intégrer la maquette réelle.

* **Faire fonctionner le fichier display**, présent dans commande_locale. Ce fichier a pour but d'ouvrir une nouvelle fenêtre pour suivre la simulation sous un autre angle.
  Ce fichier a pour but d'ouvrir une nouvelle fenêtre pour suivre la simulation sous un autre angle.
  
  Tout d'abord, l'exécutable de ce noeud est le seul à ne pas être lancé lors de la simulation générale. Néanmoins, en le lançant depuis la machine ROS1 en même temps que la simulation, une fenêtre bleue apparait, mais sans afficher de seconde vue de la simulation. Nous soupçonnons alors que ce fichier a été utilisé par le passé mais qu'il n'est plus pleinement fonctionnel dans l'état actuel du projet. Nous l’avons néanmoins migré vers ROS2 sans avoir eu le temps de le tester complètement.
  
  Nous pouvons seulement dire que ce noeud interagit avec le fichier lua Vision_sensor, situé dans celluleflexible/sim/luafiles. Pour augmenter la rapidité de notre simulation, nous avons été contraints à commenter certaines parties de ce code. En effet, l'ordre envoyé était de publier sur un topic 50 images par secondes pour recréer la vidéo de la simulation sous un autre angle.

  Lors de la migration vers ROS2, cette méthode d’envoi de données s’est révélée beaucoup plus coûteuse en termes de communication. Le changement de middleware (DDS) et la manière dont les messages sont sérialisés rendent l’envoi fréquent d’images volumineuses plus sensible aux problèmes de latence et de performance dans notre architecture actuelle. Pour cette raison, nous avons temporairement désactivé cette fonctionnalité afin de privilégier la stabilité et la rapidité de la simulation.
  
  Il est toutefois important de noter que la transmission de flux vidéo reste tout à fait possible sous ROS2, mais nécessite généralement l’utilisation de mécanismes plus adaptés (compression d’images, transport spécialisé ou réduction de la fréquence d’envoi). La remise en fonctionnement du fichier display nécessitera donc probablement une adaptation du script Vision_sensor.lua ainsi que du mécanisme de publication des images.
  
* **Faire une vidéo scriptée**

  Comme dit précédemment, un point important serait de faire un réseau de Pétri permettant de tester exhaustivement les installations, pour valider définitivement l’ensemble du code (tester les 12 aiguillages, les 6 navettes, tous les capteurs et les 4 robots).

Il faudrait alors filmer la bonne réalisation de ce code, pour remplacer la vidéo de présentation du ReadMe général. Il serait alors intéressant de faire une vidéo scriptée, c’est à dire expliquer ce que le code doit faire, pour que l’utilisateur puisse suivre en temps réel la bonne exécution de la simulation.

Ce réseau de Pétri pourrait être écrit "en dur", pour se faciliter la tâche. Il faudrait alors écrire les conditions initiales dans lesquelles on doit se trouver pour qu’il puisse être testé par la suite comme modèle de référence. Par exemple, comme conditions initiales, il serait bon de préciser les coordonnées des emplacements de navettes, la séquence mise en entrée et les modèles ROS et Coppelia dans lesquels la simulation a été enregistrée.

* **Revoir la création de la documenation (script4)**

Le dossier etu contient 4 scripts différents, avec chacun leur particularité. Le script 4 est censé créer de la documentation liée au code, dans le dossier output/docfromcpp/html. En ouvrant le fichier index.html puis en allant dans Files/File List, nous sommes censés obtenir une liste des codes présents dont nous avons demandé la documentation.

Or, dans le code actuel, malgré la demande de création de documentation pour d'autres codes (via script4), il n'y a toujours que la documentation de Tuto_Basique disponible. Il doit donc y avoir un problème pour aller récupérer le fichier Doxygen créé par le script 4. Le problème est le même en ROS1.



* **Rajouter des sécurités dans le code**

  Par exemple, pour les aiguillages, l'ordre de tourner l'aiguillage 15 n'est pas refusé alors que les aiguillages ne vont que jusqu'à 12. Cette erreur pourra être réglée dans le package Aiguillages. 
  
  Ne pas hésiter à vérifier d'autres sécurités, tel que la demande d'un capteur ou d'un robot non présent.
  
* **Rajouter dans le code des messages informatifs et des messages d'erreurs**, pour passer moins de temps au débogage.


### 2.3) Liens utiles
Les liens qui nous ont été utiles sont les suivants :
* *https://www.youtube.com/watch?v=flT3LIIR5qo* et *https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html* sont des fichiers qui nous ont aidé à installer ROS2 Jazzy
* *https://www.youtube.com/watch?v=FYKHbm4YXkA*, *https://www.youtube.com/watch?v=HJAE5Pk8Nyw* et *https://docs.ros.org/en/jazzy/Tutorials.html* qui nous ont aidé à comprendre comment fonctionnait ROS2
* *https://docs.ros.org/en/rolling/Releases.html*, qui nous a permis de comprendre les différences entre les versions de ROS2. Nous avons choisi ROS2 Jazzy, étant à ce jour la version ayant la EOL date la plus lointaine (End Of Life date).
* *https://docs.ros.org/en/rolling/Releases.html*, qui nous a aidé à installer l'interface entre ROS2 et CoppeliaSim
* *https://docs.ros.org/en/rolling/How-To-Guides/Migrating-from-ROS1.html*, qui nous a permis de commencer à migrer quelques fonctions de ROS1 à ROS2
