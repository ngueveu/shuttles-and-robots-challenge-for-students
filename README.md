# TER atelier flexible ou cellule flexible

Petit guide pour installer le projet sur votre machine s'il n'y existe pas encore : (Ubuntu 16, ROS Kinetic)

- 0) Il est toujours bon de faire un 

          sudo apt-get update

- 1) Installer la librairie modbus avec 

          sudo apt-get install libmodbus-dev
          
- 2) Cloner la branche master de ce repository :
    (si git n'est pas installé, vous pouvez toujours télécharger le projet en zip et en extraire le code)
    
          git clone https://gitlab.enseeiht.fr/sandra/TERcelluleflexible.git

- 3) si le projet existait déjà mais que vous décidez de le mettre à jour (ce qui écrasera en grande partie la précédente version), alors se placer à la racine du répertoire TERcelluleflexible du git et exécuter:

          git fetch --all
          git reset --hard origin/master

- 4) Dans le dossier TERcelluleflexible il devrait y avoir deux dossier: celluleflexible et etu (si ce dossier n'existe pas, se placer à la racine du dossier TERcelluleflexible et exécuter l'instruction suivante : cp celluleflexible/forTER/etu_init etu)

- 5) Se placer dans le dossier etu (c'est dans ce dossier que s'effectueront toutes les manipulations du TER):
    
          cd etu

- 6) S'assurer que les différents scripts sont bien executables:
    
          chmod +x scriptCompileandRun.sh
          chmod +x scriptValidation.txt
          chmod +x scriptDocumentation.txt

A présent vous êtes prêt à commencer le TER, se repérer aux fichiers dans le dossier etu pour la suite.

