/* Code CORRIGE et SECURISE pour éviter le Segmentation Fault */

#include <rclcpp/rclcpp.hpp>
#include <unistd.h>
#include <queue>
#include "FileAttente.h"
#include <std_msgs/msg/int32.hpp>
#include "capteurs.h"
#include <std_msgs/msg/byte.hpp>
#include "shuttles/msg/msg_erreur.hpp"

#include <iostream>
#include "shuttles/srv/shuttle_id.hpp"

using namespace std;
using std::placeholders::_1;
using std::placeholders::_2;

#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */

// Variables globales
vector<FileAttente*> liste_file;
int NbNavette=0;
int initPos=0;
bool activate_debug_display = false;

// --- CORRECTION 1 : Service Sécurisé ---
// On n'utilise plus de variable globale risquée ici
void shuttle_at_poste(const std::shared_ptr<shuttles::srv::ShuttleId::Request> req,
                      std::shared_ptr<shuttles::srv::ShuttleId::Response> res)
{
    int local_num_capteur = -1; // Initialisé à -1 pour détecter les erreurs

    switch (req->robot) {
        case 1:
            if (req->position==2){local_num_capteur=21;}
            if (req->position==3){local_num_capteur=22;}
            break;
        case 2:
            if (req->position==2){local_num_capteur=2;}
            if (req->position==3){local_num_capteur=3;}
            break;
        case 3:
            if (req->position==2){local_num_capteur=14;}
            if (req->position==3){local_num_capteur=15;}
            break;
        case 4:
            if (req->position==2){local_num_capteur=9;}
            if (req->position==3){local_num_capteur=10;}
            break;
    }

    // Sécurité : On vérifie que l'ID est valide et non NULL
    if (local_num_capteur > 0 && local_num_capteur < (int)liste_file.size() && liste_file[local_num_capteur] != nullptr)
    {
        if (!liste_file[local_num_capteur]->get_queue().empty())
        {
            res->id_shuttle = liste_file[local_num_capteur]->get_first_navette();
        }
        else
        {
            res->id_shuttle = 66;
        }
    }
    else
    {
        // Si la requête est invalide, on renvoie 66 (vide) sans crasher
        res->id_shuttle = 66;
    }
}


void initPosNavetteCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
    NbNavette=msg->data;
    initPos=1;
}


void ShutdownCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
    (void)msg;
    rclcpp::shutdown();
}


void IfShuttleManagerDisplayCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
    (void)msg;
    activate_debug_display = true;
    cout << "\n\n\n activate_debug_display has been turned" << BOLDGREEN << "ON" << RESET << endl;
}


int main(int argc, char **argv)
{
    cout << "debut Shuttle manager" << endl;
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::Node::make_shared("Shuttle_manager");

    auto sub_shuttlemanagerdisplay = node->create_subscription<std_msgs::msg::Byte>(
        "/commande/ShuttleManagerDisplay", 10, IfShuttleManagerDisplayCallback);

    auto subNbNavette = node->create_subscription<std_msgs::msg::Int32>(
        "/commande_locale/nbNavettes", 100, initPosNavetteCallback);

    auto service = node->create_service<shuttles::srv::ShuttleId>(
        "get_id_shuttle_at_poste", &shuttle_at_poste);
        
    auto sub_shutdown = node->create_subscription<std_msgs::msg::Byte>(
        "/commande_locale/shutdown", 10, ShutdownCallback);
        
    auto pub_erreur_log = node->create_publisher<shuttles::msg::MsgErreur>(
        "/commande/Simulation/Erreur_log", 10);

    Capteurs capteur(node);

    rclcpp::Rate loop_rate(25); 

    queue<int> queue_vide;
    queue<int> queue1;
    queue<int> queue2;
    queue<int> queue3;

    cout << " Tant que 'activate_debug_display' vaut " << BOLDGREEN << "false" ;
    cout << RESET << " rien d'autre ne s'affichera dans cette fenetre" ;
    cout << " (cf main_ShuttleManager.cpp)" << endl;

    while (initPos==0 && rclcpp::ok())
    {
        rclcpp::spin_some(node); 
        loop_rate.sleep();
    }

    // Initialisation des queues (Navettes initiales)
    for (int i=0; i<NbNavette; i++)
    {
        if (i==0) queue3.push(0);
        else if (i<4) queue1.push(i);
        else if (i<6) queue2.push(i);
    }

    // Initialisation liste_file
    liste_file.push_back(0); // Index 0 est NULL/Int
    liste_file.push_back(new FileAttente(1,25,26,queue_vide));//PS1 (1+24=25)
    liste_file.push_back(new FileAttente(0,3,3,queue_vide));//PS2
    liste_file.push_back(new FileAttente(0,5,5,queue3));
    liste_file.push_back(new FileAttente(2,0,6,queue_vide));
    liste_file.push_back(new FileAttente(2,6,0,queue_vide));
    liste_file.push_back(new FileAttente(3,27,20,queue_vide)); 
    liste_file.push_back(new FileAttente(4,8,0,queue2));
    liste_file.push_back(new FileAttente(5,28,29,queue_vide));
    liste_file.push_back(new FileAttente(0,10,10,queue_vide));
    liste_file.push_back(new FileAttente(0,12,12,queue_vide));
    liste_file.push_back(new FileAttente(6,0,13,queue_vide));
    liste_file.push_back(new FileAttente(6,13,0,queue_vide));
    liste_file.push_back(new FileAttente(7,30,31,queue_vide));
    liste_file.push_back(new FileAttente(0,15,15,queue_vide));
    liste_file.push_back(new FileAttente(0,17,17,queue_vide));
    liste_file.push_back(new FileAttente(8,0,18,queue_vide));
    liste_file.push_back(new FileAttente(8,18,0,queue_vide));
    liste_file.push_back(new FileAttente(9,32,8,queue_vide));
    liste_file.push_back(new FileAttente(10,20,0,queue1));
    liste_file.push_back(new FileAttente(11,33,34,queue_vide));
    liste_file.push_back(new FileAttente(0,22,22,queue_vide));
    liste_file.push_back(new FileAttente(0,24,24,queue_vide));
    liste_file.push_back(new FileAttente(12,0,1,queue_vide));
    liste_file.push_back(new FileAttente(12,1,0,queue_vide)); // Index 24

    // Capteurs (Index 25 à 34)
    liste_file.push_back(new FileAttente(0,2,2,queue_vide)); //CP1 (Index 25)
    liste_file.push_back(new FileAttente(0,4,4,queue_vide)); 
    liste_file.push_back(new FileAttente(0,7,7,queue_vide));
    liste_file.push_back(new FileAttente(0,9,9,queue_vide));
    liste_file.push_back(new FileAttente(0,11,11,queue_vide));
    liste_file.push_back(new FileAttente(0,14,14,queue_vide));
    liste_file.push_back(new FileAttente(0,16,16,queue_vide));
    liste_file.push_back(new FileAttente(0,19,19,queue_vide));
    liste_file.push_back(new FileAttente(0,21,21,queue_vide));
    liste_file.push_back(new FileAttente(0,23,23,queue_vide)); // Index 34

    vector<int> mem_capteur;
    vector<int> etat_capteur;
    vector<queue<int>> debug_display;

    int file_attente_suivante;
    int id_aiguillage;

    // Initialisation sûre de mem_capteur
    for (int i=0;i<36;i++) mem_capteur.push_back(0); 
    // Initialisation sûre de etat_capteur
    for (int i=0;i<36;i++) etat_capteur.push_back(0);

    while (rclcpp::ok())
    {
        // Copie des états précédents
        mem_capteur = etat_capteur;
        
        // Reset etat_capteur pour le remplir à nouveau
        etat_capteur.clear();
        etat_capteur.push_back(0); // Index 0

        // Reset debug_display à chaque tour
        debug_display.clear();
        if (activate_debug_display)
        {
            debug_display.push_back(queue_vide); // Index 0
        }

        // Lecture Capteurs
        for (int i=1;i<25;i++) etat_capteur.push_back(capteur.get_PS(i));
        for (int i=1;i<11;i++) etat_capteur.push_back(capteur.get_CP(i));
        // Taille etat_capteur = 1 + 24 + 10 = 35.

        // Logique Principale
        for (int i=1;i<35;i++)
        {
            if(etat_capteur[i]==0 && mem_capteur[i]==1)
            {
                id_aiguillage=liste_file[i]->get_id_aiguillage();
                file_attente_suivante=liste_file[i]->maj(capteur.get_DD(id_aiguillage),capteur.get_DG(id_aiguillage));
                
                // Sécurité : Vérifier que l'index suivant est valide
                if (file_attente_suivante > 0 && file_attente_suivante < (int)liste_file.size())
                {
                    liste_file[file_attente_suivante]->add_navette_in_queue(liste_file[i]->get_first_navette());
                    liste_file[i]->delete_navette_in_queue();
                }
                else if (file_attente_suivante==-1)
                {
                    liste_file[i]->delete_navette_in_queue();
                    shuttles::msg::MsgErreur msgErreur;
                    msgErreur.code=4;
                    msgErreur.n_poste=i;
                    pub_erreur_log->publish(msgErreur);
                }
                file_attente_suivante=-2;
            }

            // Remplissage Debug Display
            if (activate_debug_display)
            {
                debug_display.push_back(liste_file[i]->get_queue());
            }
        }

        // --- CORRECTION 2 : Affichage Debug Sécurisé ---
        if (activate_debug_display)
        {
            // On utilise la taille réelle du vecteur pour éviter le dépassement
            for (size_t i = 1; i < debug_display.size(); i++)
            {
                cout << "La file " << i <<" contient : ";
                
                // On copie la file pour ne pas détruire l'originale
                // et éviter les problèmes de mémoire sur le vecteur principal
                queue<int> file_temp = debug_display[i];
                
                while (!file_temp.empty())
                {
                    cout << ' ' << file_temp.front();
                    file_temp.pop();
                }
                cout << endl;
            }
            cout << endl;
        }

        rclcpp::spin_some(node); 
        loop_rate.sleep();
    }

    return 0;
}
