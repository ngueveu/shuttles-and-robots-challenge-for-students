Les fichiers .msg permette a notre code C++ (nos noeuds) de communiquer avec d’autres “robots” : les fichiers ‘Actionneurs’ et ‘Capteurs’ sont au format “PLC”, et servirait donc à communiquer avec les plcs shneider de la maquette réelle, tandique les fichiers “Echenge” et MsgSensorstate” servent à communiquer avec Coppelia

MsgSensorState.msg
C'est le "rapport d'état" de la cellule. Il contient :
std_msgs/Header header : Horodatage pour synchroniser la simulation.
int32 id : L'identifiant du capteur ou de la navette.
bool[13] dg et bool[13] dd : Les capteurs de butée "Gauche" et "Droite" pour les 12 aiguillages de la ligne Montrac (plus un de réserve).
Fonction : CoppeliaSim publie ce message pour dire à ton code : "L'aiguillage n°1 est actuellement à droite".

C'est la partie la plus importante pour ton projet avec CoppeliaSim.

    Le mot "Handle" : En programmation de bas niveau et dans les simulateurs, un "handle" (poignée) est un identifiant unique numérique.

    Dans CoppeliaSim (V-REP) : Chaque objet que tu crées (un cube, un capteur, un joint, une navette) reçoit automatiquement un numéro unique interne. C'est son "numéro de sécurité sociale".

        Exemple : Le capteur "Sensor_A" peut avoir le handle 204. La navette "Shuttle_1" peut avoir le handle 156.
        
        
        
LIEN AVEC CMAKKEELISSGTE


