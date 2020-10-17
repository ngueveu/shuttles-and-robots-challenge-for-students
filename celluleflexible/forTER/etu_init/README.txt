
Toutes les instructions données supposent que vous vous trouvez à la racine du dossier "etu"

///////////////////////////////////////////////////////////////////

A FAIRE UNE SEULE FOIS LORS DE LA PREMIERE SEANCE DE TER

///////////////////////////////////////////////////////////////////


La toute première fois que vous installez le programme, il se peut que les fichiers "scriptCompileandRun.sh" et "scriptValidation.txt" ne soient pas reconnus par le système comme exécutables. Pour y remédier, il suffit d'appliquer les manips suivantes: 
> chmod +x scriptCompileandRun.txt
> chmod +x scriptValidation.txt


Créer votre fichier principal en remplaçant XXX_XXX par vos initiales
> cp XXX_XXX.main_commande.cpp VOS_INITIALES.main_commande.cpp




/////////////////////////////////////////////////////////////////////////////////////

POUR COMPILER ET EXECUTER VOTRE FICHIER "VOS_INITIALES.main_commande.cpp" (après chaque modification de ce fichier)

/////////////////////////////////////////////////////////////////////////////////////


Pour la suite, pour compiler et exécuter en considérant les données du fichier NOM_FICHIER_DATA.prodconfig.h contenu dans le dossier dataprodconfig, il suffit de faire:

> ./scriptCompileandRun.sh VOS_INITIALES NOM_FICHIER_DATA

Par exemple, si vos initiales sont SUN_SC (et donc le fichier commande associé est SUN_SC.main_commande.cpp), et que le fichier data à considérer est data4_2_2_n0.prodconfig.h, alors l'instruction pour compiler et exécuter sera : 

> ./scriptCompileandRun.sh SUN_SC data4_2_2_n0



Après une exécution, un fichier log nommé "VOS_INITIALES_NOM_FICHIER_DATA.log.txt" doit apparaitre dans le dossier "output". 
Si ce fichier n'existe pas, c'est qu'il y a eu une erreur lors de votre simulation.



/////////////////////////////////////////////////////////////////////////////////////

POUR VERIFIER LA VALIDITE DE L'EXECUTION DE LA SIMULATION PAR RAPPORT A DES DONNEES

/////////////////////////////////////////////////////////////////////////////////////

Le fichier log précédemment généré permet de vérifier la validité de votre simulation par rapport aux données d'entrée.

Pour ce faire, depuis le dossier "etu", vous pouvez exécuter:

> ./scriptValidation.sh VOS_INITIALES NOM_FICHIER_DATA



/////////////////////////////////////////////////////////////////////////////////////

POUR GENERER LA DOCUMENTATION ASSOCIEE A VOTRE FICHIER 

/////////////////////////////////////////////////////////////////////////////////////

Pour générer la documentation associée à votre code, depuis le dossier "etu", exécuter:

> ./scriptDocumentation.sh VOS_INITIALES

La documentation devrait alors être générée dans le dossier output/docfromcpp

