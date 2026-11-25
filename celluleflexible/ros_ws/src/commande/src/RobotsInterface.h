#ifndef ROBOTS
#define ROBOTS

#include <rclcpp/rclcpp.hpp>
#include <iostream>
#include <vector>

#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/byte.hpp>
#include <robots/msg/faire_tache_msg.hpp>
#include <robots/msg/msg_num_robot.hpp>
#include <robots/msg/move_robot.hpp>
#include <robots/msg/evacuer_msg.hpp>
#include <commande_locale/srv/srv_add_product.hpp>
#include <commande_locale/msg/msg_add_product.hpp>
#include <commande_locale/msg/deplacer_piece_msg.hpp>
#include <commande_locale/msg/robot_joints.hpp>
#include <commande_locale/srv/srv_add_product_push_back.hpp>


// Etats robot
#define HAUT		1
#define BAS		-1
#define FERMEE		1
#define OUVERTE		-1
#define EN_POSITION	1

// Robots
#define ROBOT_1 1
#define ROBOT_2 2
#define ROBOT_3 3
#define ROBOT_4 4

// Postes
#define POSTE_1 1
#define POSTE_2 2
#define POSTE_3 3
#define POSTE_4 4
#define POSTE_5 5
#define POSTE_6 6
#define POSTE_7 7
#define POSTE_8 8


class RobotsInterface
{
private:

	rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_robot_position;
    rclcpp::Publisher<commande_locale::msg::RobotJoints>::SharedPtr pub_robot_joints;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_pince_fermer;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_pince_ouvrir;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_pince_descendre;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_descendre;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_monter;
    rclcpp::Publisher<robots::msg::MoveRobot>::SharedPtr pub_controler_robot;
    rclcpp::Publisher<robots::msg::FaireTacheMsg>::SharedPtr pub_faireTache;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_robot_transport1;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_robot_transport2;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_robot_transport3;
    rclcpp::Publisher<robots::msg::MsgNumRobot>::SharedPtr pub_robot_transport4;
    //rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pub_evacuer_piece;
	rclcpp::Publisher<robots::msg::EvacuerMsg>::SharedPtr pub_evacuer_piece;
    rclcpp::Publisher<commande_locale::msg::MsgAddProduct>::SharedPtr pubProductAdd;
    rclcpp::Publisher<commande_locale::msg::DeplacerPieceMsg>::SharedPtr pub_deplacer_piece;

    rclcpp::Subscription<robots::msg::MsgNumRobot>::SharedPtr sub_retourRobot;

    rclcpp::Client<commande_locale::srv::SrvAddProduct>::SharedPtr client;
    rclcpp::Service<commande_locale::srv::SrvAddProductPushBack>::SharedPtr serverPushBack;
	commande_locale::srv::SrvAddProduct srv;
	commande_locale::msg::MsgAddProduct msg0;
	commande_locale::msg::DeplacerPieceMsg deplacer_msg;

	int nbRobot;
	int robotInit[4];
	int robotPosition[4];
	int robotBras[4];
	int bras[4];
	int robotPince[4];
	int pince[4];
	int robotTask[4][2];
	int robotMacroDeplacement[4];

	// std::vector<int> produit_a_ajouter; SUN todelete

	robots::msg::FaireTacheMsg tache_msg;

public:
	RobotsInterface(rclcpp::Node::SharedPtr node, int nombre_robot);
	~RobotsInterface();

	//Contrôles des robots
	void EnvoyerPosition(int numRobot, int numPosition);
	void EnvoyerAngles(int numRobot, int angle1, int angle2, int angle3, int angle4, int angle5, int angle6, int angle7);
	void ControlerRobot(int numRobot, int numPosition, int bras, int pince);
	void FermerPince(int numRobot);
	void OuvrirPince(int numRobot);
	void DescendreBras(int numRobot);
	void MonterBras(int numRobot);

	void DeplacerPiece(int num_robot, int positionA, int positionB);

	//Retour depuis les nodes des robots
	void RetourRobotCallback(const robots::msg::MsgNumRobot::SharedPtr msg);

	int RobotInitialise(int numRobot); //retourne 1 si le robot numRobot est initialisé
	int RobotEnPosition(int numRobot); //retourne 1 si le robot numRobot est en position
	int BrasEnPosition(int numRobot);  //retourne 1 si le bras du robot numRobot est monté, -1 si le bras du robot numRobot est descendu
	int PinceEnPosition(int numRobot); //retourne 1 si la pince du robot numRobot est fermée, -1 si elle est ouverte

	void RobotTransport(int num_robot, bool valeur);
	void FaireTache(int num_poste, int duree);
	int TacheFinie(int num_poste);
	void computeFromNumPoste(int num_poste,int tab[2]);
	int FinDeplacerPiece(int num_robot);
	//void Evacuer();
	void Evacuer(int numero_poste = 3);
	void AjouterProduit(int poste, int produit);
	// int AjoutProduitEnAttente(); SUN to delete

	void ProductAddPushBack(const std::shared_ptr<commande_locale::srv::SrvAddProductPushBack::Request> request, std::shared_ptr<commande_locale::srv::SrvAddProductPushBack::Response> response);
};
#endif
