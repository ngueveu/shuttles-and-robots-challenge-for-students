#ifndef COMMANDE_SIMU
#define COMMANDE_SIMU

#include <rclcpp/rclcpp.hpp>
#include "actionneurs.h"
#include "capteurs.h"
#include "RobotsInterface.h"
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/int32.hpp>
#include <commande_locale/msg/msg_stop_control.hpp>
#include <commande_locale/msg/msg_add_product.hpp>
#include <commande_locale/srv/srv_fin_init.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

class Commande
{
	private:

	//Aiguillages
	rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr SubDeverouilleAiguillages;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr SubVerouilleAiguillages;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr SubAiguillagesGauches;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr SubAiguillagesDroits;

	// Ergots
	//ros::Subscriber subPinOn;
	//ros::Subscriber subPinOff;

	rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_nouveau_produit;
    rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pub_PetriTermine;
    rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pub_AutoRunSimu; // SUN
    rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pub_ShuttleManagerDisplay; // SUN

	rclcpp::Client<commande_locale::srv::SrvFinInit>::SharedPtr clientFinInit;
    commande_locale::srv::SrvFinInit::Request::SharedPtr srvFinInit;

	// Actionneurs
	rclcpp::Publisher<commande_locale::msg::MsgStopControl>::SharedPtr pub_navettes_stops;
    rclcpp::Publisher<commande_locale::msg::MsgSwitchControl>::SharedPtr pub_actionneurs_simu_aiguillages;
    rclcpp::Publisher<commande_locale::msg::MsgPinControl>::SharedPtr pub_actionneurs_simu_pins;

    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_playSim;
    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr sub_pauseSim;

	bool play;
	int arrivee_produit;
	int poste;
	int produit;

	/* Capteurs */
	bool PSx[25],DxD[13],DxG[13],CPx[11],CPIx[9];

	/* Actionneurs */
	bool STx[25],RxD[13],RxG[13],Vx[13],Dx[13],PIx[9];

	//messages pour actionneurs
	commande_locale::msg::MsgStopControl actionneurs_simulation_Stop;
    commande_locale::msg::MsgSwitchControl actionneurs_simulation_Aiguillages;
    commande_locale::msg::MsgPinControl actionneurs_simulation_Pin;

public:

	Commande(rclcpp::Node::SharedPtr node, std::string executionPath);

	void Initialisation();

	void PlayCallback(const std_msgs::msg::Byte::SharedPtr msg);
	void PauseCallback(const std_msgs::msg::Byte::SharedPtr msg);
	bool getPlay();

  	void NouveauProduitCallback(const commande_locale::msg::MsgAddProduct::SharedPtr msg);
	void Stop_PS(int point_stop);
	void Ouvrir_PS(int point_stop);

	void DeverouilleAiguillagesCallback(const std_msgs::msg::Int32::SharedPtr msg);
	void VerouilleAiguillagesCallback(const std_msgs::msg::Int32::SharedPtr msg);
	void AiguillagesgauchesCallback(const std_msgs::msg::Int32::SharedPtr msg);
	void AiguillagesdroitsCallback(const std_msgs::msg::Int32::SharedPtr msg);

	void SortirErgot(int num_ergot);
	void RentrerErgot(int num_ergot);
	int get_code_arrivee();
	int get_arrivee_nouveau_produit();
	void renitialiser_arrivee_nouveau_produit();

	void FinPetri();
	void activateAutoRunSimu();//SUN
	void activateShuttleManagerDisplay();//SUN
};
#endif
