
Remplacer XXX_XXX par les 3 premières lettres des noms de famille de chaque étudiant (quatre première lettres si besoin de différencier)



La toute première fois que vous installez le programme, il se peut que les fichiers "scriptCompileandRun.txt" et "scriptValidation.txt" ne soient pas reconnus par le système comme exécutable. Pour y remédier, il suffit d'appliquer les manips suivantes: 
> chmod +x scriptCompileandRun.txt
> chmod +x scriptValidation.txt


Pour la suite, pour compiler et exécuter il suffit de faire:

> ./scriptCompileandRun XXX_XXX


Après une exécution, un fichier XXX_XXX_log.txt devrait apparaitre.

Pour vérifier si le cahier des charges aurait été respecté, vous pouvez alors lancer 

> ./scriptValidation.txt XXX_XXX