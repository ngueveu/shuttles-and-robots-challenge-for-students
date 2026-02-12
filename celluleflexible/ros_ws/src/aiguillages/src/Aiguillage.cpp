#include "Aiguillage.h"
#include <cmath> // Pour pow si besoin, mais on utilise le bitshift <<

using namespace std;
using std::placeholders::_1;

Aiguillage::Aiguillage() : Node("aiguillage_node") 
{
    loop_rate = std::make_unique<rclcpp::Rate>(25); 
    
    callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    
    auto options = rclcpp::SubscriptionOptions();
    options.callback_group = callback_group_;
    
    // --- SUBSCRIBERS ---
    vrep_sub_switch_sensor = this->create_subscription<aiguillages::msg::MsgSensorState>(
        "sim_ros_interface/SwitchSensor", 10, 
        std::bind(&Aiguillage::switch_sensor_callback, this, _1), options);

    sub_cmd_droite = this->create_subscription<std_msgs::msg::Int32>(
        "/commande/Simulation/AiguillageDroite", 10, 
        std::bind(&Aiguillage::droite_callback, this, _1), options); 

    sub_cmd_gauche = this->create_subscription<std_msgs::msg::Int32>(
        "/commande/Simulation/AiguillageGauche", 10, 
        std::bind(&Aiguillage::gauche_callback, this, _1), options); 
        
    // --- PUBLISHERS (Direct CoppeliaSim) ---
    aig_dev = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLock", 10);
    aig_ver = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLock", 10);

    aig_gauche = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLeft", 10);
    aig_droite = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerRight", 10);

    // Initialisation du tableau à faux par sécurité
    for(int i=0; i<13; i++) { aig_g[i] = false; aig_d[i] = false; }

    rclcpp::sleep_for(std::chrono::seconds(1));
    RCLCPP_INFO(this->get_logger(), "--- NOEUD AIGUILLAGE PRET (Logiciel Corrigé) ---");
}

Aiguillage::~Aiguillage() {}

void Aiguillage::switch_sensor_callback(const aiguillages::msg::MsgSensorState::SharedPtr msg) 
{
    // Copie les états. Attention : msg->dd[0] est l'aiguillage 1.
    for(int i=0; i<12; i++) // On va jusqu'à 12 (index 0 à 11)
    { 
        if (i < msg->dg.size() && i < msg->dd.size()) {
            this->aig_g[i] = msg->dg[i]; 
            this->aig_d[i] = msg->dd[i];
        }
    }
}

void Aiguillage::gauche_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs)
{
    int id = msg_aigs->data; // Ex: 1

    // Sécurité anti-crash
    if (id < 1 || id > 12) {
        RCLCPP_WARN(this->get_logger(), "COMMANDE IGNOREE : ID %d invalide (doit être entre 1 et 12)", id);
        return;
    }

    int index = id - 1;       // Ex: 0
    int bitmask = 1 << index; // Ex: 1 (2^0)

    RCLCPP_INFO(this->get_logger(), "GAUCHE -> Aiguillage %d (Index %d, Bitmask %d)", id, index, bitmask);
    
    // On vérifie l'état actuel à l'index correct
    if(!aig_g[index]) 
    {
        auto msg_cmd = std_msgs::msg::Int32();
        msg_cmd.data = bitmask; // On envoie le masque binaire !

        aig_dev->publish(msg_cmd);     // Déverrouille
        aig_gauche->publish(msg_cmd);  // Tourne

        // Boucle d'attente (avec timeout pour éviter le blocage infini)
        int timeout = 0;
        while(!aig_g[index] && rclcpp::ok() && timeout < 500) 
        {
            if(aig_d[index]) aig_gauche->publish(msg_cmd); // On insiste si besoin
            loop_rate->sleep();
            timeout++;
        }
        
        // Verrouillage final
        auto msg_lock = std_msgs::msg::Int32();
        msg_lock.data = 0; 
        aig_ver->publish(msg_lock);

        if (timeout >= 500) 
            RCLCPP_ERROR(this->get_logger(), "TIMEOUT : L'aiguillage %d n'a pas atteint la gauche !", id);
        else
            RCLCPP_INFO(this->get_logger(), "SUCCES : Aiguillage %d est à GAUCHE", id);
    } else {
        RCLCPP_INFO(this->get_logger(), "Deja a gauche.");
    }
}

void Aiguillage::droite_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs) 
{
    int id = msg_aigs->data;

    if (id < 1 || id > 12) {
        RCLCPP_WARN(this->get_logger(), "COMMANDE IGNOREE : ID %d invalide", id);
        return;
    }

    int index = id - 1;       
    int bitmask = 1 << index; 

    RCLCPP_INFO(this->get_logger(), "DROITE -> Aiguillage %d (Index %d, Bitmask %d)", id, index, bitmask);
    
    if(!aig_d[index])
    {
        auto msg_cmd = std_msgs::msg::Int32();
        msg_cmd.data = bitmask;

        aig_dev->publish(msg_cmd);
        aig_droite->publish(msg_cmd);

        int timeout = 0;
        while(!aig_d[index] && rclcpp::ok() && timeout < 500)
        {
            if(aig_g[index]) aig_droite->publish(msg_cmd);
            loop_rate->sleep();
            timeout++;
        }
        
        auto msg_lock = std_msgs::msg::Int32();
        msg_lock.data = 0; 
        aig_ver->publish(msg_lock);

        if (timeout >= 500) 
            RCLCPP_ERROR(this->get_logger(), "TIMEOUT : L'aiguillage %d n'a pas atteint la droite !", id);
        else
            RCLCPP_INFO(this->get_logger(), "SUCCES : Aiguillage %d est à DROITE", id);
    } else {
        RCLCPP_INFO(this->get_logger(), "Deja a droite.");
    }
}

void Aiguillage::shutdown_callback(const std_msgs::msg::Byte::SharedPtr msg)
{
    (void)msg;
    RCLCPP_INFO(this->get_logger(), "Arret...");
    rclcpp::shutdown();
}
