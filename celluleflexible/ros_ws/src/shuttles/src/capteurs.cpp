#include "capteurs.h"

using std::placeholders::_1;

Capteurs::Capteurs(std::shared_ptr<rclcpp::Node> noeud)
{
	// Abonnement ROS 2
	sub_capteurs_simu = noeud->create_subscription<aiguillages::msg::MsgSensorState>("/commande/Simulation/Capteurs", 10, std::bind(&Capteurs::Callback_capteurs_simulation, this, _1));

	for(int i=0;i<25;i++) PSx[i]=0;
	for(int i=0;i<13;i++) DxD[i]=0;
	for(int i=0;i<13;i++) DxG[i]=0;
	for(int i=0;i<11;i++) CPx[i]=0;
	for(int i=0;i<9;i++) CPIx[i]=0;
}

Capteurs::~Capteurs()
{
}

void Capteurs::Callback_capteurs_simulation(const aiguillages::msg::MsgSensorState::SharedPtr msg)
{
	for(int i=1;i<25;i++) PSx[i]=msg->ps[i];
	for(int i=1;i<13;i++) DxD[i]=msg->dd[i];
	for(int i=1;i<13;i++) DxG[i]=msg->dg[i];
	for(int i=1;i<11;i++) CPx[i]=msg->cp[i];
	for(int i=1;i<9;i++) CPIx[i]=msg->cpi[i];
}

bool Capteurs::get_PS(int num_PS)
{
	return PSx[num_PS];
}

bool Capteurs::get_DD(int num_DD)
{
	return DxD[num_DD];
}

bool Capteurs::get_DG(int num_DG)
{
	return DxG[num_DG];
}

bool Capteurs::get_CP(int num_CP)
{
	return CPx[num_CP];
}

bool Capteurs::get_CPI(int num_CPI)
{
	return CPIx[num_CPI];
}
