#include "vrepController.h"
#include <unistd.h>
#include <filesystem>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

vrepController::vrepController(){
    repSim_startSimulation = false;
    repSim_pauseSimulation = false;
    repSim_loadModel = false;
    repSim_changeColor = false;
    repSim_getColor = false;
}

// ------------------- Simulation control -------------------
void vrepController::pause()
{
    pubSim_pauseSimulation->publish(msgSim_pauseSimulation);
    while(!repSim_pauseSimulation && rclcpp::ok()){
        rclcpp::spin_some(node);
        loop_rate->sleep();
    }
    repSim_pauseSimulation = false;
}

void vrepController::play()
{
    pubSim_startSimulation->publish(msgSim_startSimulation);
    while(!repSim_startSimulation && rclcpp::ok()){
        rclcpp::spin_some(node);
        loop_rate->sleep();
    }
    repSim_startSimulation = false;
}

// ------------------- Load models -------------------
void vrepController::loadModelInit(int shuttleNumber)
{
    char shuttleChar;
    if(shuttleNumber < 0 || shuttleNumber > 6) 
        printf(" ATTENTION, LE NUMERO DU SHUTTLE DOIT ETRE COMPRIS ENTRE 0 ET 6 \n");
    else {
        if(shuttleNumber == 0) shuttleChar = (char)(90); // SI 0 -> ShuttleZ
		else shuttleChar = char(shuttleNumber+64);
        string shuttleName = "models/montrac/shuttle" + string(1, shuttleChar) + ".ttm";

        msgSim_loadModel.data = shuttleName;
        pubSim_loadModel->publish(msgSim_loadModel);
        while(!repSim_loadModel && rclcpp::ok()){
            rclcpp::spin_some(node);
            loop_rate->sleep();
        }
        repSim_loadModel = false;
    }
}

// ------------------- Close simulation -------------------
void vrepController::close()
{
    system("pkill coppeliaSim");
}

// ------------------- Initialization -------------------
void vrepController::init(rclcpp::Node::SharedPtr n, const string& executionPath, const string& simulationFileName)
{
    node = n;
    loop_rate = new rclcpp::Rate(25);

    // Récupération dynamique du chemin du workspace
    fs::path exePath(executionPath);
    fs::path wsRoot = exePath.parent_path().parent_path().parent_path().parent_path().parent_path().parent_path(); // remonte jusqu'au workspace root
    fs::path vrepPath = wsRoot / "CoppeliaSim";

    if(!fs::exists(vrepPath)){
        cerr << "ERREUR : CoppeliaSim non trouvé dans " << vrepPath << endl;
        return;
    }

    string cmd = "cd " + vrepPath.string() + " && ./coppeliaSim.sh -s0 -q ../sim/" + simulationFileName + ".ttt &";
    system(cmd.c_str());

    // ---- Publishers / Subscribers ----
    pubSim_startSimulation = node->create_publisher<std_msgs::msg::Byte>("/sim_ros_interface/services/vrep_controller/StartSimulation", 100);
    subSim_startSimulation = node->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/response/vrep_controller/StartSimulation", 100, std::bind(&vrepController::simStartSimulationCallback, this, std::placeholders::_1));

    pubSim_pauseSimulation = node->create_publisher<std_msgs::msg::Byte>("/sim_ros_interface/services/vrep_controller/PauseSimulation", 100);
    subSim_pauseSimulation = node->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/response/vrep_controller/PauseSimulation", 100, std::bind(&vrepController::simPauseSimulationCallback, this, std::placeholders::_1));

    pubSim_loadModel = node->create_publisher<std_msgs::msg::String>("/sim_ros_interface/services/vrep_controller/LoadModel", 100);
    subSim_loadModel = node->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/services/response/vrep_controller/LoadModel", 100, std::bind(&vrepController::simLoadModelCallback, this, std::placeholders::_1));

    pubSim_changeColor = node->create_publisher<std_msgs::msg::Int32MultiArray>("/sim_ros_interface/services/vrep_controller/ChangeColor", 100);
    subSim_changeColor = node->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/response/vrep_controller/ChangeColor", 100, std::bind(&vrepController::simChangeColorCallback, this, std::placeholders::_1));

    pubSim_getColor = node->create_publisher<std_msgs::msg::String>("/sim_ros_interface/services/vrep_controller/GetColor", 100);
    subSim_getColor = node->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/services/response/vrep_controller/GetColor", 100, std::bind(&vrepController::simGetColorCallback, this, std::placeholders::_1));

    pubStopTacheRobot1 = node->create_publisher<std_msgs::msg::Int32>("/commande/Simulation/Robot1/StopTache", 100);
    pubStopTacheRobot2 = node->create_publisher<std_msgs::msg::Int32>("/commande/Simulation/Robot2/StopTache", 100);
    pubStopTacheRobot3 = node->create_publisher<std_msgs::msg::Int32>("/commande/Simulation/Robot3/StopTache", 100);
    pubStopTacheRobot4 = node->create_publisher<std_msgs::msg::Int32>("/commande/Simulation/Robot4/StopTache", 100);

    pub_erreur_log = node->create_publisher<commande_locale::msg::MsgErreur>("/commande/Simulation/Erreur_log", 100);

    //rclcpp::sleep_for(1s);
}

// ------------------- Table / Robot helpers -------------------


void vrepController::setSimulationFile(std::string fileName)
{
    this->SimulationFileName = fileName;
}

int vrepController::computeTableId(int poste) {
    switch(poste) {
        case 1: return 0;
        case 2: return 1;
        case 3: return 3;
        case 4: return 4;
        case 5: return 6;
        case 6: return 7;
        case 7: return 9;
        case 8: return 10;
        default: return -1;
    }
}

void vrepController::computeNumRobotPosteTache(int poste, int tab[2]) {
    switch(poste){
        case 1: tab[0]=1; tab[1]=4; break;
        case 2: tab[0]=1; tab[1]=1; break;
        case 3: tab[0]=2; tab[1]=1; break;
        case 4: tab[0]=2; tab[1]=4; break;
        case 5: tab[0]=3; tab[1]=4; break;
        case 6: tab[0]=3; tab[1]=1; break;
        case 7: tab[0]=4; tab[1]=1; break;
        case 8: tab[0]=4; tab[1]=4; break;
    }
}

// ------------------- Add product -------------------
void vrepController::addProduct(int produit, int poste) {
    msgSim_getColor.data = "Table#" + to_string(computeTableId(poste)) + "#0_color";
    pubSim_getColor->publish(msgSim_getColor);

    while(!repSim_getColor && rclcpp::ok()){
        rclcpp::spin_some(node);
        loop_rate->sleep();
    }
    repSim_getColor=false;
    int couleurLue = valueSim_getColor;

    if(couleurLue != 0){
        RCLCPP_ERROR(node->get_logger(), "ERREUR : On ecrase un produit !!");
        commande_locale::msg::MsgErreur msgErreur;
        msgErreur.code = 66;
        msgErreur.n_poste = poste;
        pub_erreur_log->publish(msgErreur);

        int tab[2];
        computeNumRobotPosteTache(poste, tab);
        std_msgs::msg::Int32 msg;
        msg.data = tab[1];
        switch(tab[0]){
            case 1: pubStopTacheRobot1->publish(msg); break;
            case 2: pubStopTacheRobot2->publish(msg); break;
            case 3: pubStopTacheRobot3->publish(msg); break;
            case 4: pubStopTacheRobot4->publish(msg); break;
        }
    }

    msgSim_changeColor.data.clear();
    msgSim_changeColor.data.push_back(computeTableId(poste));
    int couleur = produit * 10 + 4;
    msgSim_changeColor.data.push_back(couleur);
    for(int i=0; i<NB_CUBE-1; i++) msgSim_changeColor.data.push_back(0);

    pubSim_changeColor->publish(msgSim_changeColor);
    while(!repSim_changeColor && rclcpp::ok()){
        rclcpp::spin_some(node);
        loop_rate->sleep();
    }
    repSim_changeColor = false;
}

// ------------------- Callbacks -------------------
void vrepController::simChangeColorCallback(const std_msgs::msg::Byte::SharedPtr msg){ repSim_changeColor=true; }
void vrepController::simStartSimulationCallback(const std_msgs::msg::Byte::SharedPtr msg){ repSim_startSimulation=true; }
void vrepController::simPauseSimulationCallback(const std_msgs::msg::Byte::SharedPtr msg){ repSim_pauseSimulation=true; }
void vrepController::simLoadModelCallback(const std_msgs::msg::Int32::SharedPtr msg){ valueSim_loadModel=msg->data; repSim_loadModel=true; }
void vrepController::simGetColorCallback(const std_msgs::msg::Int32::SharedPtr msg){ valueSim_getColor=msg->data; repSim_getColor=true; }

