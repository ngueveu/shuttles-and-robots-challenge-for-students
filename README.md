# TER atelier flexible ou cellule flexible

Petit guide pour installer le projet sur votre machine s'il n'y existe pas encore : (Ubuntu 16, ROS Kinetic)

- 0) Il est toujours bon de faire un 

          sudo apt-get update

- 1) Installer la librairie modbus avec 

          sudo apt-get install libmodbus-dev
          
- 2) Si c'est la première fois que ros a été installé et lancé sur la machine, exécuter :
    
          source /opt/ros/melodic/setup.bash

Cela permet de mettre le chemin correspondant dans PATH. Pour s'en assurer, vérifier:

           echo $PATH
           
- 3) Cloner la branche master de ce repository :
    (si git n'est pas installé, vous pouvez toujours télécharger le projet en zip et en extraire le code)
    
          git clone https://gitlab.enseeiht.fr/sandra/TERcelluleflexible.git

- 4) si le projet existait déjà mais que vous décidez de le mettre à jour (ce qui écrasera en grande partie la précédente version), alors se placer à la racine du répertoire TERcelluleflexible du git et exécuter:

          git fetch --all
          git reset --hard origin/master

- 5) Dans le dossier TERcelluleflexible il devrait y avoir deux dossier: celluleflexible et etu (si ce dossier n'existe pas, se placer à la racine du dossier TERcelluleflexible et exécuter l'instruction suivante : cp celluleflexible/forTER/etu_init etu)

- 6) Se placer dans le dossier etu (c'est dans ce dossier que s'effectueront toutes les manipulations du TER):
    
          cd etu

- 7) S'assurer que les différents scripts sont bien executables:
    
          chmod +x script0_Compile.sh
          chmod +x script1_Run.sh
          chmod +x script2_GetSimuOutput.txt
          chmod +x script3_Validation.txt
          chmod +x script4_Documentation.txt

A présent vous êtes prêt à commencer le TER, se référer aux fichiers dans le dossier etu pour la suite.


Quelques erreurs lors de l'installation la première fois
s'
assure que xterm est installé: sudo apt install xterm


