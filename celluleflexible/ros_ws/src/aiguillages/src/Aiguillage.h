#ifndef AIGUILLAGE
#define AIGUILLAGE  // securite, empeche le compilateur de lire le fichier 2 fois

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp" 
#include "std_msgs/msg/byte.hpp"
#include "aiguillages/msg/msg_sensor_state.hpp" // en c++ les messages se lisent sous hpp
#include <memory>

class Aiguillage : public rclcpp::Node
{
private: // accessible uniquement dans le fichier.h
    // Le type C++ perd l'underscore : MsgSensorState
    // on declare les abonnements de la classe 
    
    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_shutdown;
    
    rclcpp::Subscription<aiguillages::msg::MsgSensorState>::SharedPtr vrep_sub_switch_sensor; // etat global

    rclcpp::CallbackGroup::SharedPtr callback_group_; // groupe de rappel pour execution simultane des fonctions
    
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_cmd_droite; //ordre de tourner à droite
    
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_cmd_gauche; //ordre de tourner à gauche

    std::unique_ptr<rclcpp::Rate> loop_rate; //vitesse de boucle
    // on declare les publisher, envoie des ordres verrouill,deverouille..
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_dev; 
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_ver;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_gauche;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_droite;

    bool aig_d[13];  // memoire des positions des aiguillages
    bool aig_g[13];

public: // accessible au main
    Aiguillage(); // sera difnis dans le cpp, icio on déclare les fonctions 
    virtual ~Aiguillage();
    // fonctions callback, def de leur nom, type de message...
    void switch_sensor_callback(const aiguillages::msg::MsgSensorState::SharedPtr msg);
    void gauche_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs);
    void droite_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs);
    void shutdown_callback(const std_msgs::msg::Byte::SharedPtr msg);
};

#endif
