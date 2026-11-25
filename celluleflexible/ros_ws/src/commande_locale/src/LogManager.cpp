#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>
#include <std_msgs/msg/byte.hpp>
#include <std_msgs/msg/float32.hpp>
#include "commande_locale/msg/msg_add_product.hpp"
#include "commande_locale/msg/msg_erreur.hpp"
#include "commande_locale/msg/tache_finie_msg.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <string>
#include <chrono>

using namespace std;

rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pubSim_getTime;
bool repSim_getTime;
float valueSim_getTime;
time_t tmm = time(0);
char* dt = ctime(&tmm);

std::string genererCheminLog() {
    std::string p = ament_index_cpp::get_package_share_directory("commande_locale");
    size_t pos = p.find("ros_ws");
    if (pos != std::string::npos) {
        return p.substr(0, pos) + "log.txt";
    }
    return "log.txt";
}

std::string filepath = genererCheminLog();

ofstream monFlux(filepath, ios::app);  //On essaye d'ouvrir le fichier

void ProduitEvacCallback(const std_msgs::msg::Int32MultiArray::SharedPtr msg)
{	
	repSim_getTime=false;
	float temps=valueSim_getTime;

	int poste=msg->data[0];   // le poste evacue est maintenant en 1ere position

	int couleur[4];
	monFlux<<"Sortie";
	monFlux<<" :"<<poste;   // on ecrit le poste dans le log
	for (int i=0;i<4;i++)
	{
		couleur[i]=msg->data[i+1];   // les couleurs sont decalees d'une case
		monFlux<<" :" <<couleur[i];
	}
	monFlux<<fixed<<setprecision(2)<<" :"<<temps;
	monFlux<<endl;
}

void NewProductCallback(const commande_locale::msg::MsgAddProduct::SharedPtr msg)
{	
	repSim_getTime=false;
	float temps=valueSim_getTime;

	monFlux<<"NouveauProduit: ";
	monFlux<<msg->num_produit;
	monFlux<<fixed<<setprecision(2)<<" :"<<temps;
	monFlux<<endl;
}

void ErreurCallback(const commande_locale::msg::MsgErreur::SharedPtr msg)
{
	switch(msg->code)
	{
		// Operation sur un poste vide
		case 1:
			monFlux<<"OperationPosteVide: ";
			monFlux<<msg->n_poste;
			monFlux<<endl;

			//a supprimer plus tard
			RCLCPP_INFO(rclcpp::get_logger("LogManager"), "ERREUR poste Vide ide ide");
            RCLCPP_INFO(rclcpp::get_logger("LogManager"), "  sur le poste: %d", msg->n_poste);
			break;

		// Operation sur un produit plein
		case 2:
			monFlux<<"OperationProduitPlein: ";
			monFlux<<msg->n_poste;
			monFlux<<endl;

			//a supprimer plus tard
			RCLCPP_INFO(rclcpp::get_logger("LogManager"), "ERREUR Operation sur un produit plein");
            RCLCPP_INFO(rclcpp::get_logger("LogManager"), "  sur le poste: %d", msg->n_poste);
            break;

		// Manipulation d'un produit en cours de traitement
		case 3:
			monFlux<<"ManipulationEnTraitement: ";
			monFlux<<msg->n_poste;
			monFlux<<endl;

			//a supprimer plus tard
			RCLCPP_INFO(rclcpp::get_logger("LogManager"), "ERREUR Manipulation produit en traitement");
            RCLCPP_INFO(rclcpp::get_logger("LogManager"), "  sur le poste: %d", msg->n_poste);
            break;

		// Manipulation d'un produit en cours de traitement
		case 4:
			monFlux<<"PerteNavette: ";
			monFlux<<msg->n_poste; // Correspond à la file
			monFlux<<endl;

			//a supprimer plus tard
			RCLCPP_INFO(rclcpp::get_logger("LogManager"), "ERREUR Perte navette");
            RCLCPP_INFO(rclcpp::get_logger("LogManager"), "  sur la file: %d", msg->n_poste);
            break;

		// Ecrasement d'un produit (ajout produit ou deplacement)
		case 66:
			monFlux<<"EcrasementProduit:";
			monFlux<<msg->n_poste; // num du robot
			monFlux<<endl;

			//a supprimer plus tard
			RCLCPP_INFO(rclcpp::get_logger("LogManager"), "ERREUR On a ecrase un produit : c'est mal");
            RCLCPP_INFO(rclcpp::get_logger("LogManager"), "  au robot %d", msg->n_poste);
            break;
	}
}

void TachefinieCallback(const commande_locale::msg::TacheFinieMsg::SharedPtr msg)
{
	monFlux<<"TempoT";
	monFlux<<": "<<msg->num_produit;
	monFlux<<": "<<msg->num_poste;
	monFlux<<": "<<msg->duree;
	monFlux<<endl;
}

void getTimeCallback(const std_msgs::msg::Float32::SharedPtr msg)
{
	valueSim_getTime=msg->data;
	repSim_getTime=true;
}

void ShutdownCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	rclcpp::shutdown();
}

void PetriTermineCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	monFlux<<endl<<"Petri terminé" << endl;
}

int main(int argc, char **argv)
{
	ofstream monFlux2(filepath);
	monFlux2.close();

	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("log_manager");

	// Subscribers
    auto subTacheFinie = node->create_subscription<commande_locale::msg::TacheFinieMsg>("/commande/Simulation/TacheFinie", 100, TachefinieCallback);
    auto subProduitEvac = node->create_subscription<std_msgs::msg::Int32MultiArray>("/commande/Simulation/produitEvac", 100, ProduitEvacCallback);
    auto subNewProduit = node->create_subscription<commande_locale::msg::MsgAddProduct>("/commande_locale/AddProduct", 100, NewProductCallback);
    auto subErreur = node->create_subscription<commande_locale::msg::MsgErreur>("/commande/Simulation/Erreur_log", 100, ErreurCallback);
    auto sub_shutdown = node->create_subscription<std_msgs::msg::Byte>("/commande_locale/shutdown", 10, ShutdownCallback);
    auto sub_terminee = node->create_subscription<std_msgs::msg::Byte>("/commande/PetriTermine", 10, PetriTermineCallback);

	// GetTime VREP
	pubSim_getTime = node->create_publisher<std_msgs::msg::Byte>("/sim_ros_interface/services/LogManager/GetTime", 100);
    auto subSim_getTime = node->create_subscription<std_msgs::msg::Float32>("/sim_ros_interface/services/response/LogManager/GetTime", 100, getTimeCallback);
	repSim_getTime=false;

	//rclcpp::sleep_for(std::chrono::seconds(1));

	rclcpp::Rate loop_rate(25); //fréquence de la boucle

	RCLCPP_INFO(rclcpp::get_logger("LogManager"), "LogManager initialise\n");
    monFlux << "Creation fichier log: " << dt << endl;

	while (rclcpp::ok())
	{
		pubSim_getTime->publish(std_msgs::msg::Byte());
		rclcpp::spin_some(node); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}
