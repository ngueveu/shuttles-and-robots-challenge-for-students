#include "Aiguillage.h"

using namespace std;
using std::placeholders::_1;

Aiguillage::Aiguillage() : Node("aiguillage_node") // codage de la fonction aiguillage
{
    loop_rate = std::make_unique<rclcpp::Rate>(25); // definiton de la vitesse de boucle
    
    // realisation des 'subscriptions de la classe
    // stockage du subscription (ex:au switchsensor), et a reception dun message sur ce topic, appelle du callback associe

    vrep_sub_switch_sensor = this->create_subscription<aiguillages::msg::MsgSensorState>(
        "sim_ros_interface/SwitchSensor", 10, 
        std::bind(&Aiguillage::switch_sensor_callback, this, _1)); // lecture des capteurs

    sub_cmd_droite = this->create_subscription<std_msgs::msg::Int32>(
        "/commande/Simulation/AiguillageDroite", 10, 
        std::bind(&Aiguillage::droite_callback, this, _1)); // commande de l'aiguillage a droite

    sub_cmd_gauche = this->create_subscription<std_msgs::msg::Int32>(
        "/commande/Simulation/AiguillageGauche", 10, 
        std::bind(&Aiguillage::gauche_callback, this, _1)); // commande de l'aiguillage a gauche
    // realisation des 'publications' de la classe
    aig_dev = this->create_publisher<std_msgs::msg::Int32>("/commande/DeverouilleAiguillage", 10);
    aig_ver = this->create_publisher<std_msgs::msg::Int32>("/commande/VerouilleAiguillage", 10);
    aig_gauche = this->create_publisher<std_msgs::msg::Int32>("/commande/AiguillageGauche", 10);
    aig_droite = this->create_publisher<std_msgs::msg::Int32>("/commande/AiguillageDroite", 10);

    rclcpp::sleep_for(std::chrono::seconds(1));
}

Aiguillage::~Aiguillage() {}

void Aiguillage::switch_sensor_callback(const aiguillages::msg::MsgSensorState::SharedPtr msg) // mise a jour des capteurs
{
    for(int i=0; i<13; i++)
    { // copie de cahque valeur renvoye par coppelia vers la memoire interne du projet
        this->aig_g[i] = msg->dg[i]; 
        this->aig_d[i] = msg->dd[i];
    }
}

void Aiguillage::gauche_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs)
{
    RCLCPP_INFO(this->get_logger(), "Mouvement GAUCHE demandé pour aiguillage n°%d", msg_aigs->data);
    
    if(!aig_g[msg_aigs->data]) // si deja a gauche on n'entre pas dans la boucle
    {
        aig_dev->publish(*msg_aigs); //envoie de l'ordre de deverouillage
        aig_gauche->publish(*msg_aigs); // ordre de mvt

        while(!aig_g[msg_aigs->data]) //le code reste bloque tant que le capteur gauche ne passe pas a true
        {
            rclcpp::spin_some(this->get_node_base_interface()); // pour eviter de rester bloquer dans la boucle while, on demande a ros de traiter les messages en attente en meme 
                                                                //temps (comme les capteurs par exemple)
            if(aig_d[msg_aigs->data]) aig_gauche->publish(*msg_aigs);
            loop_rate->sleep();
        }
        aig_ver->publish(*msg_aigs);
    }
}

void Aiguillage::droite_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs) // meme chose que gauche mais pour droite
{
    RCLCPP_INFO(this->get_logger(), "Mouvement DROITE demandé pour aiguillage n°%d", msg_aigs->data);
    
    if(!aig_d[msg_aigs->data])
    {
        aig_dev->publish(*msg_aigs);
        aig_droite->publish(*msg_aigs);

        while(!aig_d[msg_aigs->data])
        {
            rclcpp::spin_some(this->get_node_base_interface());
            if(aig_g[msg_aigs->data]) aig_droite->publish(*msg_aigs);
            loop_rate->sleep();
        }
        aig_ver->publish(*msg_aigs);
    }
}
