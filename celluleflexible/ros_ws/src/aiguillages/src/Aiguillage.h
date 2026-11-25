#ifndef AIGUILLAGE
#define AIGUILLAGE

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp" 
#include "std_msgs/msg/byte.hpp"
#include <memory>
#include <cmath> // Nécessaire pour pow()

class Aiguillage : public rclcpp::Node
{
private:
    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_shutdown;
    

    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr vrep_sub_switch_sensor;

    rclcpp::CallbackGroup::SharedPtr callback_group_; 
    
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_cmd_droite;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_cmd_gauche;

    std::unique_ptr<rclcpp::Rate> loop_rate;

    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_dev; 
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_ver;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_gauche;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr aig_droite;

    bool aig_d[13];
    bool aig_g[13];

public:
    Aiguillage(); 
    virtual ~Aiguillage();


    void switch_sensor_callback(const std_msgs::msg::Int32::SharedPtr msg);
    
    void gauche_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs);
    void droite_callback(const std_msgs::msg::Int32::SharedPtr msg_aigs);
    void shutdown_callback(const std_msgs::msg::Byte::SharedPtr msg);
};

#endif
