#include "AigsInterface.h"

#include <chrono>
#include <thread>

using namespace std;

//Couleur des messages des aiguillages
#define RESET   "\033[0m"
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */

AigsInterface::AigsInterface(rclcpp::Node::SharedPtr node)
{
	msg.data=1;
	pub_aig_Droite=node->create_publisher<std_msgs::msg::Int32>("/commande/Simulation/AiguillageDroite",100);
	pub_aig_Gauche=node->create_publisher<std_msgs::msg::Int32>("/commande/Simulation/AiguillageGauche",100);

	//rclcpp::sleep_for(std::chrono::seconds(1));
}

AigsInterface::~AigsInterface()
{
}

void AigsInterface::Droite(int num_Aig)
{
	msg_aigs.data=num_Aig;
	pub_aig_Droite->publish(msg_aigs);
}

void AigsInterface::Gauche(int num_Aig)
{
	msg_aigs.data=num_Aig;
	pub_aig_Gauche->publish(msg_aigs);
}
