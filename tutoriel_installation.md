## Guide d'installation

Ce tutoriel nécessite une machine virtuelle ou un ordinateur Linux, fonctionnant avec Ubuntu 24.04.

# 1) Installation de tous les fichiers nécessaires à la simulation (ROS2 + extensions)

### 1.1) Installation de git
Pour installer git : 
* Ouvrir un terminal à la racine de votre PC
* Faire :
```bash
sudo apt install git
sudo apt update
```

### 1.2) Récupération du dossier GitHub sur votre espace de travail :
Pour cela, exécutez dans votre espace de travail :
```bash
git clone https://github.com/aip-primeca-occitanie/cellule_flexible_coppelia-2025-2026
```

### 1.3) Installation de ROS2 Jazzy
Pour cela, suivre le tuto suivant : https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html

Les lignes à executer sont les suivantes (le passage de la configuration locale en anglais est obligatoire) :
```bash
locale  # check for UTF-8
sudo apt update && sudo apt install locales
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8
locale  # verify settings
```
Ensuite faire

```bash
sudo apt install software-properties-common
sudo add-apt-repository universe
```
```bash
sudo apt update && sudo apt install curl -y
export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F\" '{print $4}')
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo ${UBUNTU_CODENAME:-${VERSION_CODENAME}})_all.deb"
sudo dpkg -i /tmp/ros2-apt-source.deb
```
Ces lignes servent à installer les dépôts de ROS2.

Pour permettre le colcon build, on installe les outils de développement, puis on source obligatoirement l'environnement ROS 2 de base :
```bash
sudo apt update && sudo apt install ros-dev-tools
source /opt/ros/jazzy/setup.bash
```

Et enfin :
```bash
sudo apt update
sudo apt upgrade
sudo apt install ros-jazzy-desktop
```
ROS2 est maintenant installé.

### 1.4) Installation d'extensions indispensables à la simulation
Exécuter les lignes suivantes :
```bash
sudo apt update 
sudo apt install python3-xmlschema xsltproc xterm doxygen -y
```
Ces lignes servent à installer des extensions de python3, le logiciel permettant d'ouvrir les noeuds dans des fenêtres xterm séparées, ainsi que doxygen permettant de créer la documentation du projet.

### 1.5) Installation annexes
Il peut être bon d'installer en plus sur les PC Vs Code, pour visualiser proprement le code en C++. Cette application est téléchargeable sur le lien suivant : https://code.visualstudio.com/download

# 2) Compilation de l'espace de travail (Workspace) et de CoppeliaSim :

### 2.1) Compilation globale de ros_ws
Avant de compiler l'interface du simulateur, il est nécessaire de compiler l'intégralité des packages ROS 2 de notre projet. Ouvrez un nouveau terminal puis déplacez vous ici : `cellule_flexible_coppelia-2025-2026/celluleflexible/ros_ws`, exécutez alors:
```bash
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```
La dernière ligne permet de charger (sourcer) votre espace de travail local afin que ROS 2 reconnaisse vos packages personnalisés.

### 2.2) Compilation du dossier simROS2interface de CoppeliaSim
Dans le même terminal (pour conserver l'environnement ROS 2 et le workspace sourcés), déplacez-vous dans le dossier de CoppeliaSim :
```bash
cd ../CoppeliaSim
```
Définissez le chemin absolu vers le dossier CoppeliaSim pour l'outil de build :
```bash
export COPPELIASIM_ROOT_DIR=$PWD
```

Enfin, exécutez et attendez la fin de la compilation de l'interface :
```bash
colcon build --packages-select sim_ros2_interface --symlink-install
```
Vous pouvez alors fermer ce terminal.

# 3) Lancement de la simulation :
Le dossier est maintenant prêt pour lancer la simulation. On ouvre un nouveau terminal dans le dossier principal (contenant `etu` et `celluleflexible`).

Avant d'exécuter les scripts, il faut impérativement sourcer ROS 2 ainsi que notre workspace local. Exécutez les lignes suivantes :
```bash
source /opt/ros/jazzy/setup.bash
cd celluleflexible/ros_ws
source install/setup.bash
cd ../../etu
cp exemple/Tuto_Basique.main_commande.cpp .
source script0_Compile.sh Tuto_Basique dataP2_1_1_n0
./script1_Run.sh
```
Ces lignes configurent votre terminal, compilent l'intégralité des packages, et lancent la simulation en suivant le fichier `Tuto_Basique` et la dataprodconfig `dataP2_1_1_n0`.
