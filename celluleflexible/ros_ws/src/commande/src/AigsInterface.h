#ifndef AIGSINTERFACE
#define AIGSINTERFACE

#include <rclcpp/rclcpp.hpp>
#include <iostream>
//Messages
#include <std_msgs/msg/int32.hpp>


class AigsInterface
{
private:
	rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_aig_Droite;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_aig_Gauche;

	std_msgs::msg::Int32 msg;
	std_msgs::msg::Int32 msg_aigs;

public:
	AigsInterface(rclcpp::Node::SharedPtr node);
	~AigsInterface();

	//Contrôles des aiguillages
	void Droite(int num_Aig);
	void Gauche(int num_Aig);
};
#endif
