#include <iostream>
#include "vrepController.h"
#include "inOutController.h"
#include "commande_locale/SrvAddProduct.h"
#include "commande_locale/Msg_AddProduct.h"
#include "commande_locale/Msg_ChoixMode.h"
#include "commande_locale/SrvFinInit.h"
#include "commande_locale/SrvAddProductPushBack.h"
#include <unistd.h>
#include <thread>
#include <ros/ros.h>
#include <std_msgs/Byte.h>


using namespace std;

vrepController VREPController;
commande_locale::Msg_AddProduct msg0;
commande_locale::Msg_ChoixMode msg1;
bool initEnCours(true);
bool initCoppeliaEnCours(true);
bool autorunstop(false);//SUN
bool petrifinished(false);//SUN

void spinner()
{
	ros::Rate loop_rate(25);
	while(ros::ok())
	{
		ros::spinOnce();
		loop_rate.sleep();
	}
}

bool AddProduct(commande_locale::SrvAddProduct::Request &req, commande_locale::SrvAddProduct::Response &res)
{
	VREPController.addProduct(req.choixProduit,req.choixPoste);
	return true;
}

void SpawnShuttlesCallback(const std_msgs::Int32::ConstPtr& msg)
{
    // VREPController.loadModelInit(msg->data); // SUN
	for(int i=0; i<msg->data; i++) // i<nbShuttles
	{
		VREPController.loadModelInit(i);
	}
}

void ShutdownCallback(const std_msgs::Byte::ConstPtr& msg)
{
		ros::shutdown();
}

bool finInit(commande_locale::SrvFinInit::Request &req, commande_locale::SrvFinInit::Response &res)
{
	initEnCours=false;
	return true;
}

void CoppeliaFinInitCallback(const std_msgs::Byte::ConstPtr& msg)
{
	initCoppeliaEnCours=false;
}

void IfPetriTermineCallback(const std_msgs::Byte::ConstPtr& msg)//SUN
{
	petrifinished = true;
	if (autorunstop){
		cout << "\n\n\n The petri net has reached its ending place and autorun is true. \n ===> shutdown will be launched. \n" << endl;
	}
}

void AutoRunSimuCallback(const std_msgs::Byte::ConstPtr& msg)//SUN
{
	autorunstop = true;
	cout << "\n\n\n autorunstop !!!!!!!! \n" << endl;
}

int main(int argc, char **argv)
{
	//Initialisation du noeud ROS
	ros::init(argc, argv, "commande_locale");
	ros::NodeHandle nh;

	ros::Publisher pubProductAdd= nh.advertise<commande_locale::Msg_AddProduct>("/commande_locale/AddProduct",10);
	ros::Publisher pubModeType= nh.advertise<commande_locale::Msg_ChoixMode>("/commande_locale/ChoixMode",10);
	ros::Subscriber sub_spawnShuttles = nh.subscribe("/commande_locale/nbNavettes",10,SpawnShuttlesCallback);

	ros::ServiceServer service = nh.advertiseService("srv_add_product", AddProduct);
	ros::ServiceClient clientAddProduct = nh.serviceClient<commande_locale::SrvAddProductPushBack>("srv_add_product_push_back");
	commande_locale::SrvAddProductPushBack srv;
	ros::Publisher pub_shutdown = nh.advertise<std_msgs::Byte>("/commande_locale/shutdown",10);
	ros::Subscriber sub_shutdown = nh.subscribe("/commande_locale/shutdown",10,ShutdownCallback);

	ros::ServiceServer serviceInit = nh.advertiseService("srv_fin_init", finInit);
	ros::Subscriber sub_coppeliaFinInit = nh.subscribe("/sim_ros_interface/FinInit",10,&CoppeliaFinInitCallback);

	ros::Publisher pub_stopSim = nh.advertise<std_msgs::Byte>("/sim_ros_interface/StopSimulation",100);

	ros::Publisher pub_actuator = nh.advertise<std_msgs::Byte>("/actuator",100);

	ros::Subscriber sub_terminee = nh.subscribe("/commande/PetriTermine",10,&IfPetriTermineCallback);//SUN
	ros::Subscriber sub_autorunsimu = nh.subscribe("/commande/AutoRunSimu",10,&AutoRunSimuCallback);//SUN

	string nombreRobotStr=string(argv[1]);
	if(nombreRobotStr!="2" && nombreRobotStr!="4")
		nombreRobotStr="4";

	string simulationFile="Simulation"+nombreRobotStr+"Robots";

	// VREP CONTROLLER
	VREPController.init(nh,argv[0], simulationFile.c_str());

	// IN & OUT CONTROLLER
	inOutController IOController(&VREPController);
	IOController.init(nh);

	ros::Rate loop_rate(2);
	while(initCoppeliaEnCours && ros::ok())
	{
		cout << "Attente fin demarrage Coppelia ..." << endl;
		ros::spinOnce();
		loop_rate.sleep();
	}

	string cmd="roslaunch launcher launch_beta.launch nbRobot:=" + nombreRobotStr + " &";
	system(cmd.c_str());

	cout << "Pause envoyée" << endl;
	VREPController.pause();

	thread spinnerThread(spinner);
	std_msgs::Byte msg_shutdown;
	///////////////////////

	// On attend l'initialisation du reste du projet
	while(initEnCours && ros::ok())
	{
		cout << "Attente fin de l'initialisation ..." << endl;
		ros::spinOnce();
		loop_rate.sleep();
	}

	//int choix=0;
	string choix;
	int choixProduit=0;
	int choixPoste=0;
	int choixMode=0;

	if (!autorunstop) //SUN
		{
		while(ros::ok())
			{
			cout << endl << endl << endl;
			cout << "Que voulez faire ?" 	<< endl <<
				"	1- Ajouter un produit" << endl <<
				"	2- Pause simu" 	<< endl <<
				"	3- Play simu" 	<< endl<<
				"	4- Fin programme" 	<< endl <<
				"	5- Simu ou Atelier ?"	<< endl;
			cout << "Choix : ";
			cin >> choix;
			if(choix=="hhbbgd")
			{
				cout << endl << "> The white rabbit tells you a long story about three men :";
				cout << endl << ">  Kagemasa Kozuki, Yoshinobu Nakama and Tatsuo Miyasako ..." << endl;
			}
			else if(choix=="hhbbgdgdba")
			{
				cout << endl << "> Niiice ..." << endl;
				pub_actuator.publish(std_msgs::Byte());
			}
			else if(choix.length()>1 || choix[0]<'1' || choix[0]>'9')
			{
				cout << endl << " [Erreur mauvais choix ..]" << endl;
				cin.clear();
				cin.ignore(256,'\n');
			}
			else
			{
				cout << endl;
				int choixInt=atoi(choix.c_str());
				switch(choixInt)
				{
					case 1:
						cout << "Ajout de produit : quel poste ? [1..8]" << endl;
						cin >> choixPoste;
						if(cin.fail() || choixPoste<1 || choixPoste>8)
						{
							cout << endl << " [Erreur mauvais choix ..]" << endl;
							cin.clear();
							cin.ignore(256,'\n');
							break;
						}
						cout << "Quel produit ? [1..6]" << endl;
						cin >> choixProduit;
						if(cin.fail() || choixProduit<1 || choixProduit>6)
						{
							cout << endl << " [Erreur mauvais choix ..]" << endl;
							cin.clear();
							cin.ignore(256,'\n');
							break;
						}
						msg0.num_poste = choixPoste;
						msg0.num_produit = choixProduit*10+4;
						pubProductAdd.publish(msg0); // log
						VREPController.addProduct(choixProduit,choixPoste);
						srv.request.poste=choixPoste;
						srv.request.produit=choixProduit;
						clientAddProduct.call(srv);
						break;

					case 2:
						cout << "Mise en Pause de la simu" << endl;
						VREPController.pause();
						break;

					case 3:
						cout << "Mise en Play de la simu" << endl;
						VREPController.play();
						break;

					case 4:
						cout << "Fin Programme" << endl;
						pub_stopSim.publish(std_msgs::Byte());
						pub_shutdown.publish(msg_shutdown);
						ros::Duration(1).sleep();
						break;
					case 5:
						cout << "Mode : Simu (0) ou Atelier (1)?"<<endl;
						cin >> choixMode;
						if(cin.fail() || choixMode<0 || choixMode>1)
						{
							cout << endl << " [Erreur mauvais choix ..]" << endl;
							cin.clear();
							cin.ignore(256,'\n');
							break;
						}
						msg1.mode = choixMode;
						pubModeType.publish(msg1);
						break;
					default:
						cout << endl << " [Erreur mauvais choix ..]" << endl;
						break;
				}
			}

			ros::spinOnce();
		}
	}
	else{ //SUN
		bool beg=true;
		while(ros::ok())
		{
			if (beg) {
				cout << endl << endl << endl;
				cout << "Lancement automatique de la simulation" 	<< endl;
									VREPController.play();
									beg=false;
			}
			if (petrifinished){
				cout << "Fin Programme" << endl;
				ros::Duration(1).sleep();
				pub_stopSim.publish(std_msgs::Byte());
				pub_shutdown.publish(msg_shutdown);
				ros::Duration(1).sleep();
			}
			ros::spinOnce();
		}

}

	VREPController.close();
	return 0;
}
