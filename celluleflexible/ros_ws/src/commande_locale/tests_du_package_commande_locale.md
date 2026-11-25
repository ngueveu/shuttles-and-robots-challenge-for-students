# Tests du package commande_locale :

## 1) Prérequis

On doit avoir lancé dans un terminal (1) dans ros_ws:
```bash
       source /opt/ros/jazzy/setup.bash
       colcon build
       source install/setup.bash
       ros2 run commande_locale simulation 2
```
On doit avoir un autre terminal (2) dans ros_ws prêt :
```bash
       source /opt/ros/jazzy/setup.bash
       source install/setup.bash
```
       
## 2) Sortir de "Attente fin demarrage Coppelia ..." (Si pas de réponse de Coppelia)

EXPLICATION : La première boucle while bloquante rencontrée par le programme est celle qui dit si Coppelia a démarré correctement. N'ayant pas la possibilité d'avoir une réponse directe de Coppelia, nous la simulons.
TEST : Dans le terminal 2, on envoit
```bash
ros2 topic pub -n temp_pub --once /sim_ros_interface/FinInit std_msgs/msg/Byte "{}"
```
RESULTAT : L'affichage du message "Attente fin demarrage Coppelia ..." s'arrête et on affiche "Pause envoyée"



## 3) Sortir de la pause (imposée juste après la fonction précédente)

EXPLICATION : La ligne de commande qui suit la fin du démarrage de Coppelia introduit une pause dans le programme. Nous cherchons alors à en sortir pour faire exécuter la suite du programme.
TEST : Dans le terminal 2, on envoit
```bash
ros2 topic pub -n temp_pub --once /sim_ros_interface/services/response/vrep_controller/PauseSimulation std_msgs/msg/Byte "{}"
```
RESULTAT : Fin de la pause, début apparition message "Attente fin de l'initialisation ..."


## 4) Sortir de "Attente fin de l'initialisation ..."

EXPLICATION : Avant d'accéder au menu de choix, il nous reste encore une boucle while à passer : celle qui met fin à l'initialisation. Encore une fois, comme Coppelia ne répons pas, nous simulons une réponse sur le service associé.
TEST : Dans le terminal 2, on envoit
```bash
ros2 topic pub -n temp_pub --once srv_fin_init commande_locale/srv/SrvFinInit"{}"
```
RESULTAT : L'affichage du message "Attente fin de l'initialisation ..." s'arrête et le menu est affiché (Que voulez-vous faire?)


