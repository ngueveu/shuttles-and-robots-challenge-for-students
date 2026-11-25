# Tests du package shuttle :

## 1) Prérequis
On doit avoir lancé dans un terminal (1) dans ros_ws:
```bash
       source /opt/ros/jazzy/setup.bash
       colcon build
       source install/setup.bash
```

On doit avoir un autre terminal (2) dans ros_ws prêt :
```bash
       source /opt/ros/jazzy/setup.bash
       source install/setup.bash
```

## 2) Test OK de l'initialisation du Shuttle Manager

EXPLICATIONS : Le but est de tester que le gestionnaire de navettes démarre correctement, sort de sa boucle d'attente à la réception du nombre total de navettes, et place bien ces dernières dans leurs files d'attente initiales (ce que l'on vérifie en activant l'affichage de débogage).

TEST : Dans le terminal 1, on fait :

```bash
       ros2 run shuttles shuttle_manager

```

Dans le terminal 2, on fait d'abord pour débloquer l'initialisation :

```bash
       ros2 topic pub --once /commande_locale/nbNavettes std_msgs/msg/Int32 "{data: 6}"

```

Puis toujours dans le terminal 2, on fait pour afficher le contenu des files d'attente en temps réel dans le terminal 1 :

```bash
       ros2 topic pub --once /commande/ShuttleManagerDisplay std_msgs/msg/Byte "{data: 1}"

```
RESULTAT : le terminal 1 affiche la file attente 

## 3) Test OK du service de détection de navette

EXPLICATIONS : Le but est de tester la communication client/serveur entre un robot et le gestionnaire de navettes. On simule la requête d'un robot qui interroge le nœud central pour savoir quelle navette se trouve devant son poste de travail. D'après notre initialisation de l'étape 1, la navette "0" se trouve dans la file 3 (qui correspond au robot 2 en position 3).

TEST : Dans le terminal 2 (pendant que le `shuttle_manager` tourne toujours dans le terminal 1), on fait :

```bash
       ros2 service call /get_id_shuttle_at_poste shuttles/srv/ShuttleId "{robot: 2, position: 3}"

```

RESULTAT : Le terminal 2 devrait alors vous afficher une réponse confirmant la détection, avec la ligne `id_shuttle: 0`

Voici le protocole de test pour un cas d'erreur (KO), rédigé selon le même formalisme.


