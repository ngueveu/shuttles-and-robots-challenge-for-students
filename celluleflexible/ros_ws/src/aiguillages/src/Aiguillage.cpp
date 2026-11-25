#include "Aiguillage.h"

using namespace std;
using std::placeholders::_1;

Aiguillage::Aiguillage() : Node("aiguillage_node") 
{
    // On garde la fréquence de 25Hz du code ROS 1
    loop_rate = std::make_unique<rclcpp::Rate>(25); 
    
    // Callback group reentrant pour permettre l'exécution parallèle (remplace le spinOnce manuel)
    callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    auto options = rclcpp::SubscriptionOptions();
    options.callback_group = callback_group_;
    
    // --- SUBSCRIBERS ---
    
    //  Abonnement à Int32 sur le topic SwitchSensor
    vrep_sub_switch_sensor = this->create_subscription<std_msgs::msg::Int32>(
        "/sim_ros_interface/aig", 10, 
        std::bind(&Aiguillage::switch_sensor_callback, this, _1), options);

    sub_cmd_droite = this->create_subscription<std_msgs::msg::Int32>(
        "/commande/Simulation/AiguillageDroite", 10, 
        std::bind(&Aiguillage::droite_callback, this, _1), options); 

    sub_cmd_gauche = this->create_subscription<std_msgs::msg::Int32>(
        "/commande/Simulation/AiguillageGauche", 10, 
        std::bind(&Aiguillage::gauche_callback, this, _1), options); 
        
    // --- PUBLISHERS ---
    aig_dev = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLock", 10);
    aig_ver = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLock", 10);

    aig_gauche = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLeft", 10);
    aig_droite = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerRight", 10);

    // Initialisation
    for(int i=0; i<13; i++) { aig_g[i] = false; aig_d[i] = false; }
    
    RCLCPP_INFO(this->get_logger(), "--- NOEUD AIGUILLAGE PRET---");
}

Aiguillage::~Aiguillage() {}

// --- LOGIQUE ROS 1 : DECODAGE MANUEL AVEC POW ---
void Aiguillage::switch_sensor_callback(const std_msgs::msg::Int32::SharedPtr msg) 
{
    
    for(int i=1; i<13; i++)
    {
        this->aig_d[i] = (msg->data & (1 << (2*i - 2))) > 0;
        this->aig_g[i] = (msg->data & (1 << (2*i - 1))) > 0;
    }
}

void Aiguillage::gauche_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs)
{
    int id = msg_aigs->data;
    RCLCPP_INFO(this->get_logger(), "GAUCHE -> Aiguillage %d", id);

    if(!this->aig_g[id])
    {
        // 1. Déverrouillage et envoi de l'ordre initial
        this->aig_dev->publish(*msg_aigs);
        this->aig_gauche->publish(*msg_aigs);

        // 2. Boucle d'attente (comme en ROS 1)
        while(!this->aig_g[id] && rclcpp::ok())
        {
            // Pas de ros::spinOnce() en ROS2 ! Le MultiThreadedExecutor gère les capteurs en fond.
            
            // Si le capteur indique que l'aiguillage est resté à Droite, on force la republication 
            // (exactement comme dans ton code ROS 1) pour contrer la "Race Condition" du script Lua
            if(this->aig_d[id]) 
            {
                this->aig_dev->publish(*msg_aigs); // On republie le déverrouillage au cas où
                this->aig_gauche->publish(*msg_aigs);
            }
            
            this->loop_rate->sleep();
        }

        // 3. Verrouillage une fois arrivé
        this->aig_ver->publish(*msg_aigs);
        RCLCPP_INFO(this->get_logger(), "SUCCES : Aiguillage %d est à GAUCHE", id);
    }
}

void Aiguillage::droite_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs) 
{
    int id = msg_aigs->data;
    RCLCPP_INFO(this->get_logger(), "DROITE -> Aiguillage %d", id);
    
    if(!this->aig_d[id])
    {
        // 1. Déverrouillage et envoi de l'ordre initial
        this->aig_dev->publish(*msg_aigs);
        this->aig_droite->publish(*msg_aigs);

        // 2. Boucle d'attente
        while(!this->aig_d[id] && rclcpp::ok())
        {
            // Si le capteur indique que l'aiguillage est toujours à Gauche, 
            // on insiste en renvoyant les DEUX commandes (déverrouillage + mouvement)
            if(this->aig_g[id]) 
            {
                this->aig_dev->publish(*msg_aigs);   
                this->aig_droite->publish(*msg_aigs);
            }
            
            this->loop_rate->sleep();
        }
        
        // 3. Verrouillage une fois arrivé à Droite
        this->aig_ver->publish(*msg_aigs);
        RCLCPP_INFO(this->get_logger(), "SUCCES : Aiguillage %d est à DROITE", id);
    }
}

void Aiguillage::shutdown_callback(const std_msgs::msg::Byte::SharedPtr msg)
{
    (void)msg;
    rclcpp::shutdown();
}
