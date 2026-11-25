### 1. Description générale

Le but de ce répertoire est de fournir des fichiers de commande C++ de référence pour la cellule flexible. Ces exemples servent de point de départ, de fichiers de test pour vérifier le bon fonctionnement de l'environnement de simulation, ou de modèles de réinitialisation pour le développement d'un réseaux de Petri. Toutes les commandes d'exécution associées à ces exemples doivent être lancées depuis la racine du dossier "etu".

## 2. Composition

Comme l'illustre l'arborescence du dossier, ce répertoire est composé des éléments suivants :

* Un fichier `README.md` servant de documentation pour ce dossier spécifique.
* Trois scripts de commande C++ :
  * `TeamEMPTY.main_commande.cpp`
  * `TeamSALLAG.main_commande.cpp`
  * `Tuto_Basique.main_commande.cpp`



## 3. Description détaillée des fichiers `.cpp`

Voici la fonction de chacun des fichiers de commande fournis en exemple :

* **`TeamEMPTY.main_commande.cpp`** : Ce fichier est une coquille vide. Il a pour vocation de servir de base vierge et peut être utilisé pour réinitialiser le code étudiant en cas de besoin ou d'erreur majeure dans votre propre fichier de commande.
* **`TeamSALLAG.main_commande.cpp`** : Il s'agit d'une implémentation "en dur" des développeurs précédents, conçue spécifiquement pour l'instance de données `data4_3_2_n0.prodconfig.h`. Il ne fonctionne que pour ce jeu de données précis et permet uniquement de valider le niveau 1 du projet.
* **`Tuto_Basique.main_commande.cpp`** : Ce code correspond au tutoriel du sujet d'atelier flexible. Il met en place un réseau de Petri très basique et permet de vérifier que la simulation CoppeliaSim fonctionne correctement de bout en bout (compilation, exécution et génération des résultats).

## 4. Utilisation (Exemple avec Tuto_Basique)

Pour utiliser un des fichiers d'exemple, vous devez d'abord le copier à la racine de votre espace de travail (le dossier "etu"), puis le compiler et l'exécuter avec les scripts fournis.

Voici la marche à suivre complète pour tester `Tuto_Basique.main_commande.cpp` avec son fichier de configuration associé `dataP2_1_1_n0` :

1. **Copier le fichier** dans le répertoire parent :
`cp exemple/Tuto_Basique.main_commande.cpp .`
2. **Compiler la simulation** :


`./script0_Compile.sh Tuto_Basique dataP2_1_1_n0` 


3. **Lancer la simulation** :
`./script1_Run.sh`
4. **Récupérer les résultats** (après avoir quitté proprement via le terminal) :
`./script2_GetSimuOutput.sh Tuto_Basique dataP2_1_1_n0`
5. **Vérifier la validité** des résultats par rapport aux données de production :
`./script3_Validation.sh Tuto_Basique dataP2_1_1_n0`
6. **Nettoyer l'espace de travail** (optionnel) :
`rm Tuto_Basique.main_commande.cpp`

