import sys
import re
import ast

# Last modification: 20250416 - Sandra Ulrich Ngueveu

# !! Ce programme doit être lancé avec python 3: python3 checker.py
# !!! D'abord lire le fichier .config pour comprenre la synthaxe utilisé pour définir la production de chaque produit !!!

# Etapes dans ce programme:
# 1 - LIRE LE CONFIG
# 2 - LIRE LE LOG
# 3 - COMPARER LOG ET CONFIG
# 4 - AFFICHER INFOS FINALS (si tout s'est bien passé)

# On considère que:
# - Il y a max MAX_PROD_TYPE=6 produits
# - Il y a MAX_STATIONS=8 taches/stations possibles, correspondant aux 8 postes
# - Chaque produit peut subir MAX_TACHES=5 tâches maximum (variables MAX_TACHES à définir plus bas)

# Produits numéroté de 1 à 6
# Synthaxe des 6 produits dans le log: 14 / 24 / 34 / 44 / 54 / 64
# --> ex: P = 24, le '2' indique le n° du produit, et le '4' indique que c'est un produit (de type produit)

# Taches (ou actions) numéroté de 1 à 8:
#       cube plein (tâche fini) : 13 / 23 / 33 / 43 / 53 / 63 / 73 / 83
#       --> ex: D1 = 23, le '2' indique le poste où a été fait la tache, et le '3' indique que c'est un cube plein donc que la tâche est bien fini sur V-rep
#
#       cube transparent (tâche en cours) : 12 / 22 / 32 / 42 / 52 / 62 / 72 / 82
#       --> ex: D1 = 22, le 1er '2' indique le poste où est fait la tache, et le 2nd '2' indique que c'est un cube semi-transparent donc que la tâche est en cours sur V-rep

# A améliorer:
#   - dans erreur de durée d'une tâche, dire sur quelle version du produit s'est produit l'erreur
#   - le programme doit pouvoir fonctionner avec n'importe quelle valeur de MAX_TACHES -> modifier la déclarations des matrices


def checkNoStepRepeatInProd(config, MAX_PROD_TYPE):
    repeat = 0
    for P in range(1,MAX_PROD_TYPE+1):
        for i in range(1,len(config[P])):
            for j in range(i+1,len(config[P])):
                if (config[P][i]['station'] == config[P][j]['station']) and (config[P][i]['duration'] == config[P][j]['duration']):
                    repeat = 1
                    print("ERREUR dans instance !!! La paire 'poste, duree' se répète dans la gamme de fabrication du produit {}", P)
    return repeat



if (len(sys.argv) <= 2):
    sys.exit("Vous avec oublié de spécifier en argument le nom du fichier ProdConfig et le nom du fichier log")


# LA SEULE VARIABLE DANS checker.py QUE L'UTILISATEUR PEUT CHANGER S'IL MODIFIE LE NOMBRE MAX DE TACHES SUR UN PRODUIT DANS V-REP
MAX_TACHES = 5  # le nombre max de tâches par produit -> car dans CoppeliaSim on a un nombre limité de cubes, le 1er cube définit le produit et les cubes restants (au nombre de MAX_TACHES) définissent les tâches effectuées sur les produits

MAX_PROD_TYPE = 6

MAX_STATIONS = 8 # 8 postes de travail

test = 1  # Cette variable indique s'il y a une erreur dans le log par rapport au cahier des charges définit dans le fichier .config: 1 - PAS D'ERREUR  /  0 - ERREUR
erreur_config = 0  # Dit si il y a une erreur de déclaration dans le fichier .config par l'étudiant: 1 - ERREUR / 0 - PAS ERREUR

# --------------- 1 - LIRE LE CONFIG ---------------

def checkProductConfigKeys(ProductConfig):
    if set(ProductConfig.keys()) != set(["Prod_qte", "Prod_type", "Prod_seqdeposte", "Prod_dureeparposte"]):
        print(ProductConfig.keys())
        print("The set of dictionary keys [",ProductConfig.keys(), "] differs from the set([Prod_qte, Prod_type, Prod_seqdeposte, Prod_dureeparposte])")
        return -1
    return 0


def checkProductConfigSizes(ProductConfig, MAX_TACHES, MAX_STATIONS):
    #check that the sizes are coherent
    if len(ProductConfig["Prod_type"]) <= 0:
        print("size of Prod_type is ",len(ProductConfig["Prod_seqdeposte"]) , " less than 0, therefore invalid")
        return -1   
    if len(ProductConfig["Prod_type"]) > MAX_STATIONS:
        print("size of Prod_type is ",len(ProductConfig["Prod_seqdeposte"]) , " greater than MAX_TACHES=", MAX_TACHES, ", therefore invalid")
        return -2       
    if len(ProductConfig["Prod_type"]) != len(ProductConfig["Prod_qte"]):
        print("size of Prod_type (", len(ProductConfig["Prod_type"]), ") is different from size of Prod_qte", len(ProductConfig["Prod_qte"]), ")")
        return -3
    if len(ProductConfig["Prod_type"]) != len(ProductConfig["Prod_seqdeposte"]):
        print("size of Prod_type (", len(ProductConfig["Prod_type"]), ") is different from size of Prod_qte", len(ProductConfig["Prod_seqdeposte"]), ")")
        return -4        
    if len(ProductConfig["Prod_type"]) != len(ProductConfig["Prod_dureeparposte"]):
        print("size of Prod_type (", len(ProductConfig["Prod_type"]), ") is different from size of Prod_qte", len(ProductConfig["Prod_dureeparposte"]), ")")
        return -5
    for i in range(len(ProductConfig["Prod_type"])):
        if len(ProductConfig["Prod_seqdeposte"]) <= 0:
            print("size of Prod_seqdeposte[",i,"] is ",len(ProductConfig["Prod_seqdeposte"]) , " less than 0, therefore invalid")
            return -6
        if len(ProductConfig["Prod_seqdeposte"]) > MAX_TACHES:
            print("size of Prod_seqdeposte[",i,"] is ",len(ProductConfig["Prod_seqdeposte"]) , " greater than MAX_TACHES=", MAX_TACHES, ", therefore invalid")
            return -7
        if len(ProductConfig["Prod_seqdeposte"]) != len(ProductConfig["Prod_dureeparposte"]):
            print("size of Prod_seqdeposte[",i,"] is ",len(ProductConfig["Prod_seqdeposte"]) , " greater than MAX_TACHES=", MAX_TACHES, ", therefore invalid")
            return -8

    return 0      

def checkProductConfigValues(ProductConfig, MAX_STATIONS, MAX_PROD_TYPE):
    #check that the sizes are coherent
    for i in range(len(ProductConfig["Prod_type"])):
        if ProductConfig["Prod_type"][i] not in range(1,MAX_PROD_TYPE+1):
            print("size of Prod_type[",i,"] is ", ProductConfig["Prod_type"][i], "not in range(1,MAX_PROD_TYPE)=range(1,",MAX_PROD_TYPE , ", therefore invalid")
            return -1
        if ProductConfig["Prod_qte"][i] <= 0:
            print("size of Prod_qte[",i,"] is ", ProductConfig["Prod_qte"][i], "is <= 0, therefore invalid")
            return -2
        for j in range(len(ProductConfig["Prod_seqdeposte"][i])):
            if ProductConfig["Prod_seqdeposte"][i][j] not in range(1,MAX_STATIONS+1):
                print("size of Prod_seqdeposte[",i,"][",j,"] is ", ProductConfig["Prod_seqdeposte"][i][j], "not in range(1,MAX_STATIONS)=range(1,",MAX_STATIONS , ", therefore invalid")
                return -3
            if ProductConfig["Prod_dureeparposte"][i][j] <=0:
                print("size of Prod_dureeparposte[",i,"][",j,"] is ", ProductConfig["Prod_dureeparposte"][i][j], "<=0, therefore invalid")
                return -4 
    return 0
         

def splitallin(cpp_allin):
    splitted=cpp_allin.split(';');
    return splitted

def parse_cpp_vector(cpp_code):
    # Expression régulière pour extraire les éléments du vecteur
    vector_pattern = r'^\s*const\s+vector<[^>]+>\s+(\w+)\s*\{(.*?)\}'
    match = re.search(vector_pattern, cpp_code, re.DOTALL)
    if match:
        namevector = match.group(1)
        namevector.strip(' ')
        elements_str = match.group(2)
        # Supprimer les espaces et split par les virgules
        elements = [int(elem.strip()) for elem in elements_str.split(',')]
        return elements, namevector
    else:
        raise ValueError("Aucun vecteur trouvé dans le code C++ fourni.")


def parse_cpp_vector_of_vectors(cpp_code):
    # Expression régulière pour extraire les éléments du vecteur de vecteurs
    vector_pattern = r'^\s*const\s+vector<vector<[^>]+>>\s+(\w+)\s*\{\s*\{(.*?)\}\s*\}'
    match = re.search(vector_pattern, cpp_code, re.DOTALL)
    if match:
        namevector = match.group(1)
        namevector.strip(' ')
        elements_str = match.group(2)
        elements_str = elements_str.replace(' ', '')
        elements_str.strip()
        elements_str = "".join(elements_str.splitlines())
        elements_str = "{{"+elements_str+"}}"
        elements_str = elements_str.replace("POSTE_", '')
        # Remplacer les doubles accolades par des crochets pour faciliter l'évaluation
        elements_str = elements_str.replace('{', '[').replace('}', ']')
        # Utiliser ast.literal_eval pour évaluer la structure de manière sécurisée
        elements = ast.literal_eval(elements_str)
        return elements, namevector
    else:
        raise ValueError("Aucun vecteur de vecteurs trouvé dans le code C++ fourni.")


# Exemple d'utilisation
cpp_allin="""
const vector<int> Prod_type{ 2,
                             6,
                             4 };

const vector<vector<int>> Prod_seqdeposte{  { POSTE_1, POSTE_4 },
                                            { POSTE_7, POSTE_6, POSTE_5 },
                                            { POSTE_1, POSTE_2 } };

const vector<vector<int>> Prod_dureeparposte{  { 4, 5 },
                                               { 3, 6, 3 },
                                               { 7, 3 } };

const vector<int> Prod_qte{ 2,
                            1,
                            1 };
"""


# --------------- 1 - LIRE LE CONFIG ---------------

# Déclarations des matrices nécessaires à "enregistrer" les data
tache = []  # contient les destinations que chaque produit
dure = []  # contient les durées de chaque tache
nb_produit = []  # contient le nombre de fois qu'un même produit doit être créer

verif_config = []  # sert juste a créer la matrice temps - Cette matrice nous sert seulement à créer la matrice temps, qui elle nous servira à savoir quelle tâche pour quelle produit est incorrecte s'il y a une erreur de durée de tâche
temps = []  # contient la durée des taches

production = []  # Catalogue l'enchainement de tâches des 6 produits désirée

config = [ [] for i in range(MAX_PROD_TYPE+1)]
nbtoprod = [ 0 for i in range(MAX_PROD_TYPE+1)]
wip = 0
wipvector = []
wiptime = 0
wipmax = 0

try:
    with open(sys.argv[1], 'r') as file:
        contenu = file.read()

    ProductConfig = dict()
    cpp_allin=contenu.split("// ProductConfig")
    splitted=splitallin(cpp_allin[1])
    for i in range(len(splitted)):
        count = splitted[i].count('{')
        if count == 1:
            parsed_vector, namevector = parse_cpp_vector(splitted[i])
            #print(i, namevector,"=", parsed_vector)
            ProductConfig[namevector]=parsed_vector
        if count >= 2:
            parsed_vector, namevector = parse_cpp_vector_of_vectors(splitted[i])
            #print(i, namevector,"=", parsed_vector)
            ProductConfig[namevector]=parsed_vector
        #if count <=0:
            #print("empty line ? ", splitted[i])
    print(ProductConfig)

    #createalllines (Prod_qte, Prod_type, Prod_seqdeposte, Prod_dureeparposte)
    erreur_config = checkProductConfigKeys(ProductConfig)
    if erreur_config == 0:
        erreur_config = checkProductConfigSizes(ProductConfig, MAX_TACHES, MAX_STATIONS)
        if erreur_config ==0:
            erreur_config = checkProductConfigValues(ProductConfig, MAX_STATIONS, MAX_PROD_TYPE)



    # Déclarations des matrices nécessaires à "enregistrer" les data
    tache = []  # contient les destinations que chaque produit
    dure = []  # contient les durées de chaque tache
    nb_produit = []  # contient le nombre de fois qu'un même produit doit être créer

    verif_config = []  # sert juste a créer la matrice temps - Cette matrice nous sert seulement à créer la matrice temps, qui elle nous servira à savoir quelle tâche pour quelle produit est incorrecte s'il y a une erreur de durée de tâche
    temps = []  # contient la durée des taches

    production = []  # Catalogue l'enchainement de tâches des 6 produits désirée

    config = [ [] for i in range(MAX_PROD_TYPE+1)]
    nbtoprod = [ 0 for i in range(MAX_PROD_TYPE+1)]
    wip = 0
    wipvector = []
    wiptime = 0
    wipmax = 0

    # Initilisation des matrices ci-dessous (sauf verif_config) qui seront tous des tableaux de 6 lignes, chaque ligne par produit -> 1ère ligne pour produit 1, 2ème ligne pour produit 2, etc
    for i in range(MAX_PROD_TYPE):
        nb_produit.append(0)  # Matrice 6x1
        temps.append(
            [0, 0, 0, 0, 0, 0, 0, 0])  # Initialisation de la matrice temps 6x8 ( car 6 produits et 8 postes )
        tache.append([
                        0])  # initialisation nombre de ligne, dans la boucle for suivante on terminera l'initialisation de cette matrice qui dépend de MAX_TACHES
        dure.append([
                        0])  # initialisation nombre de ligne, dans la boucle for suivante on terminera l'initialisation de cette matrice qui dépend de MAX_TACHES
        production.append([
                            0])  # initialisation nombre de ligne, dans la boucle for suivante-suivante on terminera l'initialisation de cette matrice qui dépend de MAX_TACHES

    for i in range(MAX_TACHES - 1):  # Ajout des colonnes en fonction de MAX_TACHES
        tache = [line + [0] for line in tache]  # Matrice 6x3, chaque colonne correspondant à une tache du produit
        dure = [line + [0] for line in dure]  # Matrice 6x3, chaque colonne pour la durée de la tâche correspondante

    for i in range(MAX_TACHES):  # Ajout des colonnes en fonction de MAX_TACHES
        production = [line + [0] for line in
                    production]  # Matrice 6x4, 1ère colonne contient le n° produit, et les autres colonnes définient comme la matrice tache

    print(' ')  # Sert juste à aérer l'affichage

    #print("what now", erreur_config)

    if erreur_config == 0:  # Si il n'y a pas d'erreur par l'utilisateur dans le .config, on ajoute la matrice des destinations dans la matrice tache dans la ligne correspondant au produit correspondant, et on définit la matrice production pour la ligne du produit correspondant
        #print("start fill data structures")
        ## TYPE PRODUIT
        for i in range(len(ProductConfig["Prod_type"])):
            produit_num = int(ProductConfig["Prod_type"][i])
            #print("produit, ", produit_num)
            
             ## DESTINATIONS (ou taches) ET production ET DUREE
            production[produit_num - 1][0] = produit_num
            for j in range(len(ProductConfig["Prod_seqdeposte"][i])):
                production[produit_num - 1][j + 1] = ProductConfig["Prod_seqdeposte"][i][j]
                tache[produit_num - 1][j] = ProductConfig["Prod_seqdeposte"][i][j]
                dure[produit_num - 1][j] = ProductConfig["Prod_dureeparposte"][i][j]

            # NOMBRE PRODUIT
            nb_produit[produit_num - 1] = ProductConfig["Prod_qte"][i]
            nbtoprod[produit_num] = ProductConfig["Prod_qte"][i]

            # REMPLIR TABLEAU verif_config
            config[produit_num].append(dict(station="enter", duration=0.0))
            for j in range(len(ProductConfig["Prod_seqdeposte"][i])):
                verif_config.append([produit_num, ProductConfig["Prod_seqdeposte"][i][j], ProductConfig["Prod_dureeparposte"][i][j]])
                config[produit_num].append(dict())
                config[produit_num][-1]['station'] = ProductConfig["Prod_seqdeposte"][i][j]
                config[produit_num][-1]['duration'] = ProductConfig["Prod_dureeparposte"][i][j]
            config[produit_num].append(dict(station="exit", duration=0.0))

   # FIN de lire ligne par ligne le fichier .config

    print("DATA\nconfig=",config, "\nnbtoprod=",nbtoprod)
    test = test + checkNoStepRepeatInProd(config, MAX_PROD_TYPE)

    # REMPLIR TABLEAU temps
    if erreur_config == 0:
        taille = len(verif_config)
        for i in range(taille):
            temps[verif_config[i][0] - 1][verif_config[i][1] - 1] = verif_config[i][2]

    # RECAP: Les matrices qui nous serviront pour plus tard et définit dans cette partie: temps / nb_produit / production

    # --------------- 2 - LIRE LE LOG ---------------

    # Différents syntaxe dans le log:
    #   - Sortie: 14: 13: 23: 33: 15.2 --> format P: D1: D2: D3: time
    #       ! Si moins de MAX_TACHES tâches alors met des 0 à D2 et/ou D3. Et si evacuation vide met Sortie: 0: 0: 0: 0: 15.2

    # Déclarations des matrices pour la lecture du log
    verif_config_log = []  # Matrice Nx3; sert juste a créer la matrice temps_log (comme avant la matrice verif_config qui servait à créer la matrice temps)
    temps_log = []  # Matrice 6xMAX_TACHES; contient la durée des tâches dans la simulation
    nb_produit_log = []  # Matrice 6x1; compte le nombre de produit apparu, pour ensuite vérifier si ces navettes apparus (si elles sont attendu d'après le cahier des charges) sont bien toutes sorties
    nb_produit_new = []  # Matrice 6x1; va compter le nombre de produit apparu, pour ensuite vérifier si le nombre de produit sortis correspond bien au nombre de produit apparu, et aussi pour afficher dans message d'erreur NouveauProduit quelle version a une erreur
    produit_final = []  # Ce tableau aura la forme produit_final = [P, D1, D2, D3]

    produit_entree = []  # Matrice nbre_apparition x 2; Affichera à la suite de la lecture du log les produits qui apparaisent sur les postes produit_entree[i] = [P, time]; donc ce tableau est dans l'ordre chronologique d'apparition des produits
    produit_duree = []  # Matrice 6x1; Contient la durée totale de présence des produits dans la simulation. A la fin on divisera cette somme par le nombre de produit pour avoir une moyenne de temps de présence de chaque produit

    D = []  # Matrice MAX_TACHES x 2; contient la valeur de durée de chaque tâche dans 1ère colonne et le type de la tâche ('3' si bon, '2' si pas bon) dans la 2ème colonne. 1ère ligne = 1ère tache, etc
    D_list = []  # Matrice MAX_TACHES x 1; qui contient type list de chaque tâche

    nbprodcreate = [0 for i in range(MAX_PROD_TYPE+1)]
    nbprodexit = [0 for i in range(MAX_PROD_TYPE+1)]
    totprodexit = [0.0 for i in range(MAX_PROD_TYPE+1)]
    totprodcreate = [0.0 for i in range(MAX_PROD_TYPE+1)]
    avgprodmakespan = [0.0 for i in range(MAX_PROD_TYPE+1)]
    avgmakespan = 0.0
    totduration = 0.0

    for i in range(MAX_TACHES):
        D.append([0, 0])

    for i in range(
            6):  # Pour sommer les durée on les initialise à 0, 6 ligne pour chaque produit -> plus tard on fera produit_duree[i] = produit_duree[i] + (temps - produit_entree[k][i])
        produit_duree.append(0.0)

    for i in range(
            6):  # initialisation des matrices nb_produit_log (compte sorties de chaque produit) et nb_produit_new (compte apparition de chaque produit)
        nb_produit_log.append(0)
        nb_produit_new.append(0)

    for i in range(6):
        temps_log.append([0, 0, 0, 0, 0, 0, 0,
                        0])  # Initialisation de la matrice temps_log 6x8 ( car 6 produits et 8 postes (ou taches) )

    nextstep = [ [] for i in range(MAX_PROD_TYPE+1)]

    # Ouvrir fichier log, enregistres ses lignes, puis referme fichier
    Log_file = open(sys.argv[2], "r")
    # Log_file = open("log.txt","r")
    Log = Log_file.readlines()
    Log_file.close()

    print("\nLOG")

    if erreur_config == 0:  # On lit le contenu du fichier log SEULEMENT SI il n'y a eu aucune erreur de décaration dans le fichier .config
        for line in Log:
            info = line.replace(" ", "")  # on enleve tous les espaces de la ligne
            info = info.split(":")  # on decoupe on fonction de ":"
            ID = info[
                0]  # le premier terme est l'identifiant ID -> on identifie ligne par ligne le premier mot, s'il correspond à une certaine synthaxe alors on sait comment traiter la ligne

            if ID == "OperationPosteVide":
                poste = int(info[1].strip('\n'))
                print('ERREUR: opération sur poste vide numero {} '.format(
                    poste))  # Si le robot fait une tâche sur un poste vide, alors ce message d'erreur apparait, et "test = 0" signifie que y a déjà un erreur
                test = 0

            if ID == "OperationProduitPlein":
                poste = int(info[1].strip('\n'))
                print("ERREUR: opération sur un produit qui a déjà les 3 tâches maximale sur le poste {} ".format(poste))
                test = 0

            if ID == "EcrasementProduit":
                poste = int(info[1].strip('\n'))
                print('ERREUR: il y a un produit écrasé au poste {} '.format(poste))
                test = 0

            if ID == "PerteNavette":
                troncon = int(info[1].strip('\n'))
                print("ERREUR: perte navette au tronçon {} car l'aiguillage a tourné trop tard".format(troncon))
                test = 0

            if ID == "TempoT":  # identification de la ligne de Log
                P_list = list(info[
                                1])  # Numéro du produit, exemple: 43, donc c'est la tâche 4 et le 3 indique que c'est bien une tâche fini
                P = int(P_list[0].strip('\n'))
                # P_type = int(P_list[1]) # inutile ici car on vérifie déjà dans NouveauProduit et Sortie si le num du produit fini bien par 4
                D_tempo = int(info[2])  # Ici ce ne sera pas 23 ou 22, mais ce sera le numéro du poste donc de 1 à 8
                T = float(info[3])  # durée de la tâche
                verif_config_log.append([P, D_tempo, T])

                i = 0
                if i < len(nextstep[P]):
                    k = nextstep[P][i]
                while i < len(nextstep[P]) and (config[P][k]['station'] != D_tempo or config[P][k]['duration'] != T):
                    i = i + 1
                    if i < len(nextstep[P]):
                        k = nextstep[P][i]
                if i < len(nextstep[P]):
                    nextstep[P][i] = k + 1
                    if k == 1:
                        wip = wip + 1
                        wipvector.append(dict(wip=wip, time=wiptime))
                        if wip > wipmax:
                            wipmax = wip
                else:
                    print("ERROR !!!! wrong step in production of product type ", P)
                    test = 0

            if ID == "NouveauProduit":
                P_list = list(
                    info[1])  # Le produit par exmeple 24, donc c'est le produit 2 et le 4 indique que c'est bien un produit
                P = int(P_list[0])  # Avec l'exemple de la ligne au-dessus, ici ce serait '2'
                P_type = int(P_list[1])  # Et ici ce serait '4'
                time = float(info[2].strip(
                    '\n'))  # Le temps où le produit est apparu (nous servira à faire la moyenne de production de chaque produit
                nb_produit_new[P - 1] = nb_produit_new[
                                            P - 1] + 1  # On incrémente le nombre de produit apparu pour le produit correspondant
                produit_entree.append([P,
                                    time])  # A chaque fois qu'un produit apparait sur un post on l'ajoute dans le tableau en indiquant le temps d'apparition. Ce tableau servira ensuite a calculer le temps de présence du produit jusqu'ç ce qu'il sorte
                if P_type != 4 and nb_produit[
                    P - 1] != 0:  # Si le produit n'est pas de type '4' (donc pas de type produit) alors erreur --> en condition on a "AND nb_produit" car on veut afficher les messages d'erreur utile seulement ! donc si le produit qui n'est pas de type '4' n'est pas définit dans le fichier .config, alors on n'affiche pas ce messages d'erreur car inutile pour nous
                    test = 0
                    print(
                        "ERREUR à t={}s: l'apparition de l'instance {} du produit {} n'est pas définit comme produit".format(
                            time, nb_produit_new[P - 1], P))
                nbprodcreate[P] = nbprodcreate[P] + 1
                totprodcreate[P] = totprodcreate[P] + time
                wiptime = time
                i=0
                while (i < len(nextstep[P])) and (nextstep[P][i] != 0):
                    i = i+1
                if i < len(nextstep[P]):
                    nextstep[P][i] = 1
                else:
                    nextstep[P].append(1)


            if ID == "Sortie":  # identification de la ligne de Log
                P_list = list(info[1])
                taille_info = len(info)
                for i in range(taille_info - 3):
                    # D_list.append(info[2+i]) #SUN
                    if info[2 + i] != '0':  # SUN
                        D_list.append(info[2 + i])
                time = float(info[taille_info - 1].strip('\n'))
                P = int(P_list[0])
                if P != 0:
                    P_type = int(P_list[1])
                taille_D = len(D_list)
                for i in range(taille_D):
                    D[i][0] = int(D_list[i][0])
                    if D[i][0] != 0:
                        D[i][1] = int(D_list[i][1])
                if P != 0:  # Donc si la sortie n'est pas vide, qu'il y a bien un produit qui est evacuer
                    produit_final = [P]
                    for i in range(MAX_TACHES):
                        produit_final.append(D[i][0])
                    # produit_final = [P, D1, D2, D3] # on ecrasera cette ligne du tableau apres le test
                    nb_produit_log[P - 1] = nb_produit_log[P - 1] + 1
                    # durée de vie du produit
                    k = 0  # variable qui compte à quelle ligne dans produit_entree se trouve le plus ancien produit apparu
                    while (
                    1):  # On considère que c'est impossible d'avoir dans le log plus de "Sortie" que de "NouveauProduit". Il n'est donc pas nécessaire d'afficher une erreur sur ça car quoique fasse l'étudiant il y aura forcément plus ou le même nombre de NouveauProduit que de Sortie
                        if produit_entree[k][
                            0] == P:  # lorsqu'on a trouver la ligne où se trouve le plus ancien produit apparu
                            produit_duree[P - 1] = produit_duree[P - 1] + (
                                        time - produit_entree[k][1])  # Somme du temps de présence du produit P
                            produit_entree[k][
                                0] = 0.0  # Comme le produit est sortie on initialise sa ligne dans produit_entree à 0 comme ça elle ne pourra plus servir à calculer produit_duree
                            break
                        else:
                            k = k + 1

                    # Verif sortie
                    nbprodexit[P] = nbprodexit[P] + 1
                    totprodexit[P] = totprodexit[P] + time
                    totduration = max(totduration, time)
                    i = 0
                    if i < len(nextstep[P]):
                        k = nextstep[P][i]
                    while i < len(nextstep[P]) and config[P][k]['station'] != "exit":
                        i = i + 1
                        if i < len(nextstep[P]):
                            k = nextstep[P][i]
                    if i < len(nextstep[P]):
                        nextstep[P][i] = 0 #reinitialisation
                        wip = wip - 1
                        wipvector.append(dict(wip=wip, time=time))
                    else:
                        print("ERROR !!!! wrong step in exit of product of type ", P)
                        test = 0

                    # Affichage des erreurs
                    if nb_produit[P - 1] != 0 and nb_produit_log[P - 1] <= nb_produit[
                        P - 1]:  # cette condition sert à afficher les erreurs seulement sur les produits désiré. Si des produit sont créer mais non désiré il y a un autre message d'erreur qui se génère (en bas du code), donc pas besoin de faire plusieurs messages d'erreur sur des produits non désiré, juste 2 suffit (pour dire que produit non désiré a été crée, et que produit non désiré a été sortie)
                        """if produit_final != production[P - 1]:
                            test = 0
                            print(
                                "ERREUR: enchainement tâches de l'instance {} du produit {} doit être {} et pas {}".format(
                                    nb_produit_log[P - 1], P, production[P - 1][1:], produit_final[1:]))
                        """
                        if P_type != 4:
                            test = 0
                            print(
                                "ERREUR: la base de l'instance {} du produit {} n'est pas définie comme un produit".format(
                                    nb_produit_log[P - 1], P))
                        # for i in range(taille_D-1): #SUN
                        for i in range(taille_D):  # SUN
                            if D[i][1] == 2:
                                print(
                                    "ERREUR: la tâche {} de l'instance {} du produit {} n'est pas terminée lors de la sortie du produit".format(
                                        i + 1, nb_produit_log[P - 1], P))
                            if D[i][1] != 2 and D[i][1] != 3:
                                print(
                                    "ERREUR: la tâche {} de l'instance {} du produit {} n'est pas définie comme une tâche".format(
                                        i + 1, nb_produit_log[P - 1], P))

                    del produit_final  # pour réinitiliser cette variable
                    del D_list
                    del D
                    D = []
                    D_list = []
                    for i in range(MAX_TACHES):
                        D.append([0, 0])

                elif nb_produit[P - 1] == 0:  # Donc si P = 0 et qu'aucun produit est sorti
                    print(
                        'ERREUR: il y a une évacuation vide au poste 3')  # Car ce n'est qu'au poste 3 qu'on peut évacuer les produits finis
                    test = 0

        # REMPLIR TABLEAU temps_log
        taille = len(verif_config_log)
        for i in range(taille):
            temps_log[verif_config_log[i][0] - 1][verif_config_log[i][1] - 1] = verif_config_log[i][2]

        for i in range(6):
            if nb_produit_log[i] != 0:
                produit_duree[i] = produit_duree[i] / nb_produit_log[
                    i]  # On calcule le temps de présence moyen pour chaque produit (comme il peut y avoir plusieurs fois le même produit)

    #print("nextstep=", nextstep)

    # RECAP: Les matrices utile qu'on vient de créer: temps_log / produit_final / nb_produit_log / taches_time / produit_entree / produit_duree


    # --------------- 3 - COMPARAISON ENTRE LOG ET CONFIG ---------------


    # 4 trucs a verifier ici:
    #   - temps et temps_log via verif_temps: vérifie que le temps de chaque actions est bon ET que chaque actions a été réaliser (actions = taches)
    #   - production et produit_final: vérifie la bon ordre des tâches de chaque produit --> SE FAIT DANS PARTI "LECTURE LOG" ENFAITE!
    #   - nb_produit et nb_produit_log: vérifie que le bon nombre de chaque produit et bien sortis (le nombre attendu par rapport au fichier config)
    #   - nb_produit_log et nb_produit_new: vérifier si il y a autant de produit qui sont apparu que de produit qui sont sortis, pour éviter que l'étudiant fasse appraitre des produit en trop

    if erreur_config == 0:  # Les messages d'erreurs liée au log s'affiche si il n'y a eu aucune erreur de décalaration dans le .config

        for i in range(1,MAX_PROD_TYPE+1):
            avgprodmakespan[i] = (totprodexit[i] - totprodcreate[i]) / max(1,nbprodexit[i])
        avgmakespan = (sum(totprodexit) - sum(totprodcreate)) / max(1,sum(nbprodexit))

    #    print("avgmakespanprod=", avgprodmakespan, "\nglobalavgmakespan=", avgmakespan)
    #    print("wipmax=", wipmax, "\ntotduration=", totduration)

        # Initialisation de verif_temps
        verif_temps = []
        for i in range(6):
            verif_temps.append([0, 0, 0, 0, 0, 0, 0,
                                0])  # Pour définir la taille de verif_temp, qui a la meme forme que temps et temps_log mais ne contient pas de temps mais du booléen (0 ou 1) -> '1' les temps coincide, 0 les temps ne sont pas les même (entre config et log)
        for i in range(6):
            for j in range(8):
                if temps[i][j] == 0:
                    verif_temps[i][j] = 1
                else:
                    verif_temps[i][
                        j] = 0  # si on met pas ce else alors tout ce met à 1 à partir du premier 0 détecter (sait pas expliquer pourquoi, bizarre)

        # Comparer temps et temps_log, en complétant verif_temps à 1 si temps respecter, 2 sinon
        # on fait comme ça pour par la suite connaitre exactement sur quelle tache la durée n'a pas été respecté
        for i in range(6):
            for j in range(8):
                if temps[i][j] == temps_log[i][j]:
                    verif_temps[i][j] = 1
                else:
                    verif_temps[i][j] = 2

        # Si au moins une cellule de verif_temps est à 0, ça signifie qu'un temps n'a pas été respecté et donc test = 0 (erreur)
        for i in range(6):
            for j in range(8):
                if verif_temps[i][j] == 2 and nb_produit[i] != 0 and temps[i][j] != 0:
                    test = 0
                    print('ERREUR: durée tâche {} du produit {} doit être égale à {}s, or ici durée est {}s'.format(j + 1,
                                                                                                                    i + 1,
                                                                                                                    temps[
                                                                                                                        i][
                                                                                                                        j],
                                                                                                                    temps_log[
                                                                                                                        i][
                                                                                                                        j]))

        # Compare nb_produit et nb_produit_log pour voir si le bon nombre de produit est bien sorti
        # Si bon nombre de produit sorti attendu, on compare nb_produit_log et nb_produit_new pour voir s'il y a eu autant de produit apparu que de produit sorti
        for i in range(1,MAX_PROD_TYPE+1):
            if nbprodcreate[i] != nbtoprod[i]:
                if nbtoprod[i] == 0:
                    test = 0
                    print("ERREUR: le produit {} est apparu {} fois or il n'est pas censé apparaître".format(i,
                                                                                                            nbprodcreate[
                                                                                                                i]))
                else:
                    test = 0
                    print(
                        "ERREUR: le produit {} doit apparaître {} fois or vous l'avez fait apparaître {} fois".format(i,
                                                                                                                    nbtoprod[
                                                                                                                        i],
                                                                                                                    nbprodcreate[
                                                                                                                        i]))
            if nbprodexit[i] != nbtoprod[i] and nbtoprod[i] != 0:
                test = 0
                print('ERREUR: le produit {} est sortie {} fois or il doit être sortie {} fois'.format(i,
                                                                                                    nbprodexit[i],
                                                                                                    nbtoprod[i]))

    # --------------- 4 - AFFICHER INFOS FINALS ---------------



    # S'affiche seulement s'il n'y a aucune erreur de log donc si test = 1 ET si il n'y a eu aucune erreur de déclaration dans .config donc si erreur_config = 0

    if erreur_config == 0:
        if test == 1:
            for i in range(6):
                if nb_produit[i] != 0:
                    print(' ')
                    print("Le produit {} a été assemblé {} fois avec l'empilement de tâches: {}".format(i + 1,
                                                                                                        nb_produit_log[i],
                                                                                                        production[i][1:]))
                    print("Le temps moyen d'assemblage de ce produit est de {0:.2f}s".format(produit_duree[i]))

        print(' ')  # sert juste à aérer l'affichage

        print("METRICS\navgmakespanprod=", avgprodmakespan, "\nglobalavgmakespan=", avgmakespan,"s")
        print("wipmax=", wipmax, "\ntotduration=", totduration,"s")

    print("\nTHE END")

    # print(' ')  # sert juste à aérer l'affichage


    # ---- PRINT VARIABLES ------

    # print(produit); print(" ")
    # print(tache); print(" ")
    # print(dure); print(" ")
    # print(production); print(" ")
    # print(nb_produit); print(" ")
    # print(verif_config); print(" ")
    # print(temps); print(" ")

    # print(verif_config_log); print(" ")
    # print(produit_final); print(" ")
    # print(temps_log); print(" ")
    # print(nb_produit_log); print(" ")
    # print(produit_entree); print(" ")
    # print(produit_duree); print(" ")

    # print(verif_temps); print(" ")
    # print(test); print(" ")

    # print(P_list); print(" ")
    # print(P); print(" ")
    # print(P_type); print(" ")
    # print(D1_list); print(" ")
    # print(D1); print(" ")
    # print(D1_type); print(" ")
    # print(D2_list); print(" ")
    # print(D2); print(" ")
    # print(D2_type); print(" ")
    # print(D3_list); print(" ")
    # print(D3); print(" ")
    # print(D3_type); print(" ")



except ValueError as e:
    print(e)





