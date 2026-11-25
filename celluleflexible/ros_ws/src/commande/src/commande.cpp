#include "commande.h"
#include <chrono>

using namespace std;

//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

Commande::Commande(rclcpp::Node::SharedPtr node, std::string executionPath)
{
	// Actionner ergots
	//subPinOn = noeud.subscribe("/Poste_Cmde/SortirErgots", 10, &Commande::SortirErgotCallback, this);
	//subPinOff = noeud.subscribe("/Poste_Cmde/RentrerErgots", 10, &Commande::RentrerErgotCallback, this);

	//Commande aiguillages
	SubDeverouilleAiguillages = node->create_subscription<std_msgs::msg::Int32>("/commande/DeverouilleAiguillage", 100, std::bind(&Commande::DeverouilleAiguillagesCallback, this, std::placeholders::_1));
	SubVerouilleAiguillages =  node->create_subscription<std_msgs::msg::Int32>("/commande/VerouilleAiguillage", 100, std::bind(&Commande::VerouilleAiguillagesCallback, this, std::placeholders::_1));
	SubAiguillagesGauches =  node->create_subscription<std_msgs::msg::Int32>("/commande/AiguillageGauche", 100, std::bind(&Commande::AiguillagesgauchesCallback, this, std::placeholders::_1));
	SubAiguillagesDroits =  node->create_subscription<std_msgs::msg::Int32>("/commande/AiguillageDroite", 100, std::bind(&Commande::AiguillagesdroitsCallback, this, std::placeholders::_1));

	sub_pauseSim = node->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/vrep_controller/PauseSimulation",10,std::bind(&Commande::PauseCallback, this, std::placeholders::_1));
	sub_playSim = node->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/vrep_controller/StartSimulation",10,std::bind(&Commande::PlayCallback, this, std::placeholders::_1));
	play=false;
	clientFinInit = node->create_client<commande_locale::srv::SrvFinInit>("srv_fin_init");
	pub_PetriTermine = node->create_publisher<std_msgs::msg::Byte>("/commande/PetriTermine",10);
	pub_AutoRunSimu = node->create_publisher<std_msgs::msg::Byte>("/commande/AutoRunSimu",10);//SUN
	pub_ShuttleManagerDisplay = node->create_publisher<std_msgs::msg::Byte>("/commande/ShuttleManagerDisplay",10);//SUN

	// Publishers messages actionneurs
	pub_navettes_stops = node->create_publisher<commande_locale::msg::MsgStopControl>("/commande/Simulation/Actionneurs_stops", 100);
	pub_actionneurs_simu_aiguillages = node->create_publisher<commande_locale::msg::MsgSwitchControl>("/commande/Simulation/Actionneurs_aiguillages", 100);
	pub_actionneurs_simu_pins = node->create_publisher<commande_locale::msg::MsgPinControl>("/commande/Simulation/Actionneurs_pins", 100);

	//rclcpp::sleep_for(std::chrono::seconds(1));

	// Initialisation des Actionneurs
	for(int i=1;i<=24;i++) actionneurs_simulation_Stop.stop[i] = 0;
	for(int i=1;i<=24;i++) actionneurs_simulation_Stop.go[i] = 1;

	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.lock[i] = 0;
	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.rd[i] = 0;
	for(int i=1;i<=12;i++) actionneurs_simulation_Aiguillages.rg[i] = 0;

	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.pinon[i] = 0;
	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.pinoff[i] = 1;
}

void Commande::Initialisation()
{
	for(int i=0;i<25;i++) PSx[i]=0;
	for(int i=0;i<13;i++) DxD[i]=0;
	for(int i=0;i<13;i++) DxG[i]=0;
	for(int i=0;i<11;i++) CPx[i]=0;
	for(int i=0;i<9;i++) CPIx[i]=0;

	for(int i=0;i<25;i++) STx[i]=1;
	for(int i=0;i<13;i++) RxD[i]=0;
	for(int i=0;i<13;i++) RxG[i]=0;

	for(int i=0;i<13;i++) Vx[i]=0;
	for(int i=0;i<13;i++) Dx[i]=0;
	for(int i=0;i<9;i++) PIx[i]=0;
	auto request = std::make_shared<commande_locale::srv::SrvFinInit::Request>();
	clientFinInit->async_send_request(request);
}

void Commande::PlayCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	play=true;
}

void Commande::PauseCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	play=false;
}

bool Commande::getPlay()
{
	return play;
}


void Commande::Stop_PS(int point_stop)
{
	actionneurs_simulation_Stop.stop[point_stop]=1;
	actionneurs_simulation_Stop.go[point_stop]=0;
	pub_navettes_stops->publish(actionneurs_simulation_Stop);
}

void Commande::Ouvrir_PS(int point_stop)
{
	actionneurs_simulation_Stop.stop[point_stop]=0;
	actionneurs_simulation_Stop.go[point_stop]=1;
	pub_navettes_stops->publish(actionneurs_simulation_Stop);
}

void Commande::DeverouilleAiguillagesCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	Dx[msg->data]=1;
	Vx[msg->data]=0;
}

void Commande::VerouilleAiguillagesCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	Dx[msg->data]=0;
	Vx[msg->data]=1;
}

void Commande::AiguillagesgauchesCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	RxG[msg->data]=1;
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.rg[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.rd[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.lock[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages->publish(actionneurs_simulation_Aiguillages);
	RxG[msg->data]=0;
}

void Commande::AiguillagesdroitsCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	RxD[msg->data]=1;
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.rd[i] = RxD[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.rg[i] = RxG[i];
	for(int i=0;i<13;i++) actionneurs_simulation_Aiguillages.lock[i] = !Vx[i] && Dx[i];
	pub_actionneurs_simu_aiguillages->publish(actionneurs_simulation_Aiguillages);
	RxD[msg->data]=0;
}

void Commande::SortirErgot(int num_ergot)
{
	PIx[num_ergot]=1;
	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.pinon[i] = PIx[i];
	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.pinoff[i] = !PIx[i];
	pub_actionneurs_simu_pins->publish(actionneurs_simulation_Pin);
}

void Commande::RentrerErgot(int num_ergot)
{
	PIx[num_ergot]=0;
	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.pinon[i] = PIx[i];
	for(int i=1;i<=8;i++) actionneurs_simulation_Pin.pinoff[i] = !PIx[i];
	pub_actionneurs_simu_pins->publish(actionneurs_simulation_Pin);
}

void Commande::FinPetri()
{
	pub_PetriTermine->publish(std_msgs::msg::Byte());
}

void Commande::activateAutoRunSimu() // SUN
{
	pub_AutoRunSimu->publish(std_msgs::msg::Byte());
}

void Commande::activateShuttleManagerDisplay() // SUN
{
	pub_ShuttleManagerDisplay->publish(std_msgs::msg::Byte());
}
