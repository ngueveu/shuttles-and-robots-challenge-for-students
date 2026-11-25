# Tests du package robots :

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
       
## 2) Test OK du Shutdown
EXPLICATIONS : Le but est de tester que la simulation se ferme bien lorsqu'on lui en donne l'ordre.

TEST : Dans le terminal 1, on fait :
```bash
       ros2 launch robots robotsGauche.launch.py
```

Dans le terminal 2, on fait :
```bash
       ros2 topic pub -n temp_pub --once /commande_locale/shutdown std_msgs/msg/Byte "{}"
```

RESULTAT : On doit voir les deux fenêtres des robots se fermer (terminal 1) et voir apparaitre des messages "process has finished cleanly" (terminal 1)


## 3) Test KO de l'initialisation (choix du robot)
EXPLICATIONS : Il y a 4 robots différents. Nous devons donc choisir un numéro de robot compris entre 1 et 4. Si num_robot est 1, 2, 3 ou 4, la simulation doit s'ouvrir correctement (directement dans le terminal). Si num_robot est invalide, la simulation renvoit CHOIX ROBOT INCORRECT.

TEST : Dans le terminal 1, on fait :
```bash
       ros2 run robots robot 8
```

RESULTAT : La console affiche "CHOIX ROBOT INCORRECT".


## 4) Protocole pour finir l'initialisation 
//Pas besoin si lié à Coppelia, besoin si testé individuellement

EXPLICATIONS : Pour terminer l'initialisation, nous avons besoin d'avoir tous les handles du robot. On demande donc à Coppelia de nous les envoyer. Comme Coppelia n'est pas encore présent, on simule sa réponse par une publication sur le bon topic. 

TEST : Dans le terminal 1, on fait :
```bash
       ros2 run robots robot 2
```
Dans le terminal 2, on fait :
```bash
       ros2 topic pub --times 7 /sim_ros_interface/services/response/robot2/GetObjectHandle std_msgs/msg/Int32 "{data: 1}"
```

RESULTAT : Le terminal 1 affiche "Robot 2 OK".

## 5) Test OK de la fonction Evacuer (faire le 4=fin initialisation avant)
EXPLICATIONS : On souscrit dans l'initialisation au topic "/commande/Simulation/Evacuer", de format Byte, qui exécute si activé la fonction Evacuer (dans Robot). On vérifie donc qu'en publiant un message sur ce topic, la fonction Evacuer s'effectue correctement.

TEST : (tout dans le terminal 2)
* 5.1) Pour rentrer dans la fonction (publier un message sur le topic), on fait :

    ```bash
    ros2 topic pub -n temp_pub --once /commande/Simulation/Evacuer std_msgs/msg/Byte "{}"
    ```

* 5.2) Ensuite pour sortir du premier while, on fait :

    ```bash
    ros2 topic pub --times 6 /sim_ros_interface/services/response/robot2/GetColor std_msgs/msg/Int32 "{data: 1}"
    ```

* 5.3) Pour sortir de la fonction Evacuer, on fait :

    ```bash
    ros2 topic pub -n temp_pub --once /sim_ros_interface/services/response/robot2/ChangeColor std_msgs/msg/Byte "{}"
    ```
RESULTAT : (visible dans le terminal 1)
* Pour le 5.1. nous obtenons "Callback Evacuer recu pour robot 2".
* Pour le 5.2. nous obtenons une liste de couleur "couleur[0]=1, couleur[1]=1, couleur[2]=1, couleur[3]=1, couleur[4]=1, couleur[5]=1"

