/*
## Projet Long ENSEEIHT - GEA Department - 2020
 
## Mis à jour par SUN 2023

## Fichier de configuration de la simulation
##
## Ce fichier permet de configurer les produits qui doivent être fabriqués par la cellule
## Il doit respecté une syntaxe précise, il doit donc être manipulé avec précaution et ces commentaires sont à lire avec attention !
##
## Le checker va lire ce fichier et interpréter les lignes après le mot clé S-t-a-r-t, afin de valider ou pas le travail de l'étudiant.
##
## Les lignes permettent la configuration des produits :
## Chaque produit se voit attribuée un nombre entre 1 et 6.
## La ligne produit respectent la syntaxe suivante:
##
## Produit : d1 d2 d3 : t1 t2 t3 : c
##
## Les 'd' correspondent aux destinations successives du produit cad les taches à effectuer ( dans l'ordre biensûr) , on peut en spécifier entre 1 et 8. ex si on met 1 2, cela indique que le produit doit être traite au poste 1 puis au poste 2 avant de sortir de la cellule.
## Les 't' correspondent aux durées des taches en secondes. Il doit y en avoir autant que de 'd'. ex 1 : 1 indique que la tache 1 dure 1 seconde.
## Le 'c' correspond à la commande de produit, c'est à dire le nombre de Produit que doit fournir l'étudiant. Si 'c' vaut 3 pour le produit 1, alors il faudra sortir de la simulation 3 produits de type 1.
##
## ATTENTION aux limitation suivantes :
## MAX 6 PRODUITS AVEC 8 DESTINATIONS CHACUN ! ! ! ! !
## SI il y a une erreur lors de la déclaration de ces variables sur ce fichier, le checker l'identifiera et vous dira quoi changer
## Le nombre max de tâche par produit doit être défini dans checker.py par la variable MAX_TACHES !! Ça correspond au nombre de cubes qu'il y a par navette et poste dans V-Rep
##
## exemples de ligne produit :
## 1 : 1 2 3 : 11.5 12 13 : 1
## 5 : 4  2 1 : 5 3 10 : 2
## 4 : 2 1 : 15 14 : 1
## 6 : 4 2 1 : 15  14 12 : 3
##
## --> pas besoin de respecter l'ordre des numéros de produit ligne par ligne, vous pouvez également laisser des lignes vides ou laisser des espaces entre les destinations et les durées, il n'y aura pas de problème de lecture pour ça
##
##
## REMARQUE :
## Chaque produit a une couleur différente (pareil pour les tâches):
## 1 : rouge, 2 : bleu, 3 : vert, 4 : orange, 5 : rose, 6 : violet
*/

// Start

// 4 : 4 3 4 1 2 : 9 5 5 6 6 : 3

// End

// ProductConfig


const vector<int> Prod_type{ 4 };

const vector<vector<int>> Prod_seqdeposte{  { POSTE_4, POSTE_3, POSTE_4, POSTE_1, POSTE_2 } };

const vector<vector<int>> Prod_dureeparposte{  { 9, 5, 5, 6, 6 } };

const vector<int> Prod_qte{ 3 };


