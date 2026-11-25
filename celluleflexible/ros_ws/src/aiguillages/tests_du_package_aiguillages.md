Afin de tester l'intégration indépendamment de CoppeliaSim, vous pouvez simuler son comportement en ligne de commande depuis des terminaux : 

* Test du noeud aiguillage sans Coppelia : 
### Phase 1 : vérifier la connectivité
 
    Pour ce test, on ouvre 2 terminaux à la source du dossier ros_ws.
    **Dans un TERMINAL 1**, faire :
```bash
  source /opt/ros/jazzy/setup.bash
  colcon build --packages-select aiguillages
  source install/setup.bash
  ros2 run aiguillages run_Aiguillage 
```
    **Dans un TERMINAL 2**, faire :
```bash
  source /opt/ros/jazzy/setup.bash
  ros2 topic list
```
    Vous devriez voir la liste des topics actifs apparaitre. Cela permet de voir si tous les topics ont été correctement créés.

Voici le protocole de test corrigé. La modification principale se trouve au **TERMINAL 4**, où nous envoyons désormais la valeur **256**.

Dans votre code, le capteur est décodé via un masque de bits : pour l'aiguillage **5** à **DROITE**, le bit correspondant est le 8ème ($2 \times 5 - 2 = 8$), ce qui donne une valeur décimale de $2^{8} = \mathbf{256}$.

---

### Phase 2 : test de commande

Pour ce test, on ouvre 4 terminaux à la source du dossier `ros_ws`.

**TERMINAL 1 : Lancement du nœud**

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select aiguillages
source install/setup.bash
ros2 run aiguillages run_Aiguillage 

```

**TERMINAL 2 : Écoute du verrouillage**

```bash
source install/setup.bash
ros2 topic echo /sim_ros_interface/SwitchControllerLock

```

**TERMINAL 3 : Envoi de l'ordre de mouvement**
On demande de faire tourner l'aiguillage **5** à droite.

```bash
source install/setup.bash
ros2 topic pub -1 /commande/Simulation/AiguillageDroite std_msgs/msg/Int32 "{data: 5}"

```

> **Note :** Vous verrez `data: 5` apparaître dans le **TERMINAL 2** (pour le déverouillage). Dans le **TERMINAL 1**, le message `DROITE -> Aiguillage 5` s'affiche.

**TERMINAL 4 : Simulation du capteur (Réponse Coppelia)**
On envoie la valeur correspondant au bitmask de l'aiguillage 5 en position droite ($2^8$).

```bash
source install/setup.bash
ros2 topic pub -1 /sim_ros_interface/aig std_msgs/msg/Int32 "{data: 256}"

```

---

### Résultat attendu

Grâce à la valeur **256**, la boucle `while` dans votre code va enfin détecter que l'aiguillage est arrivé à destination.

* **TERMINAL 1 :** Le message `SUCCES : Aiguillage 5 est à DROITE` doit apparaître instantanément.
* **TERMINAL 2 :** Vous devez recevoir un message `data: 5`, confirmant que le nœud a envoyé l'ordre de verrouillage final.







* Test du noeud aiguillage avec CoppeliaSim (script .lua):
  
Refaire le test de commande pas à pas, mais cette fois-ci en lançant Coppelia, au lieu de simuler ses publications (étape du terminal 4 faite par Coppelia)
