#ifndef VREP_CONTROLLER
#define VREP_CONTROLLER

#include <rclcpp/rclcpp.hpp>
#include <vector>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/byte.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>

#include "commande_locale/msg/msg_color.hpp"
#include "commande_locale/msg/msg_erreur.hpp"

#define NB_CUBE 6

class vrepController
{
private:
    //** Topic V-Rep **//

    // StartSimulation
    rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pubSim_startSimulation;
    std_msgs::msg::Byte msgSim_startSimulation;
    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr subSim_startSimulation;
    bool repSim_startSimulation;

    // PauseSimulation
    rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pubSim_pauseSimulation;
    std_msgs::msg::Byte msgSim_pauseSimulation;
    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr subSim_pauseSimulation;
    bool repSim_pauseSimulation;

    // LoadModel
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pubSim_loadModel;
    std_msgs::msg::String msgSim_loadModel;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subSim_loadModel;
    bool repSim_loadModel;
    int valueSim_loadModel;
    
    // RemoveModel
    //////////////////////////////////////
    
    // ChangeColor
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pubSim_changeColor;
    std_msgs::msg::Int32MultiArray msgSim_changeColor;
    rclcpp::Subscription<std_msgs::msg::Byte>::SharedPtr subSim_changeColor;
    bool repSim_changeColor;

    // GetColor
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pubSim_getColor;
    std_msgs::msg::String msgSim_getColor;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subSim_getColor;
    bool repSim_getColor;
    int valueSim_getColor;

    //Gestion des navettes
    ////////////////////////////////////////////

    rclcpp::Publisher<commande_locale::msg::MsgErreur>::SharedPtr pub_erreur_log;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubStopTacheRobot1;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubStopTacheRobot2;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubStopTacheRobot3;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubStopTacheRobot4;

    std::string SimulationFileName;
    rclcpp::Rate* loop_rate;

public:
    rclcpp::Node::SharedPtr node;
    
    vrepController();
    void init(rclcpp::Node::SharedPtr n, const std::string& executionPath, const std::string& simulationFileName);
    void close();
    void loadModelInit(int shuttleNumber);
    //void removeModel(int32_t handle);
    //int32_t getObjectHandle(std::string);
    void waitVrep();
    void play();
    void pause();

    void setSimulationFile(std::string);
    ///////////////////////void add
    ///////////////////////

    int computeTableId(int poste);
    void addProduct(int produit, int poste);
    
    void computeNumRobotPosteTache(int poste,int tab[2]);

    // Callbacks pour V-Rep
    void simChangeColorCallback(const std_msgs::msg::Byte::SharedPtr msg);
    void simStartSimulationCallback(const std_msgs::msg::Byte::SharedPtr msg);
    void simPauseSimulationCallback(const std_msgs::msg::Byte::SharedPtr msg);
    void simLoadModelCallback(const std_msgs::msg::Int32::SharedPtr msg);
    void simGetColorCallback(const std_msgs::msg::Int32::SharedPtr msg);
};

#endif

