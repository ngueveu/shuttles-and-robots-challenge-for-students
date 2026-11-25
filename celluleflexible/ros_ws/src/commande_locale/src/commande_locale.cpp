#include <iostream>
#include <string>
#include <chrono>
#include <thread>


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/byte.hpp"
#include "std_msgs/msg/int32.hpp"

#include "commande_locale/msg/msg_add_product.hpp"
#include "commande_locale/msg/msg_choix_mode.hpp"
#include "commande_locale/srv/srv_add_product.hpp"
#include "commande_locale/srv/srv_fin_init.hpp"

#include "vrepController.h"
#include "inOutController.h"

using namespace std::chrono_literals;

// --- Variables globales ---
vrepController VREPController;
commande_locale::msg::MsgAddProduct msg0;
commande_locale::msg::MsgChoixMode msg1;

bool initEnCours = true;
bool initCoppeliaEnCours = true;
bool autorunstop = false;
bool petrifinished = false;

// --- Callbacks ---
bool AddProduct(const std::shared_ptr<commande_locale::srv::SrvAddProduct::Request> req, std::shared_ptr<commande_locale::srv::SrvAddProduct::Response> res)
{
    VREPController.addProduct(req->choixproduit, req->choixposte);
    res->success = true;
    return true;
}

void SpawnShuttlesCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
    for(int i = 0; i < msg->data; ++i)
        VREPController.loadModelInit(i);
}

void ShutdownCallback(const std_msgs::msg::Byte::SharedPtr)
{
    rclcpp::shutdown();
}

void FinInitCallback(const std::shared_ptr<commande_locale::srv::SrvFinInit::Request>, std::shared_ptr<commande_locale::srv::SrvFinInit::Response>)
{
    initEnCours = false;
}

void CoppeliaFinInitCallback(const std_msgs::msg::Byte::SharedPtr)
{
    initCoppeliaEnCours = false;
}

void IfPetriTermineCallback(const std_msgs::msg::Byte::SharedPtr)
{
    petrifinished = true;
    if(autorunstop)
        std::cout << "\nPetri net finished and autorun=true. Shutdown will be launched.\n";
}

void AutoRunSimuCallback(const std_msgs::msg::Byte::SharedPtr)
{
    autorunstop = true;
    std::cout << "\nautorunstop !!!!!!!! \n";
}

// --- Main ---
int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("commande_locale");
    
    rclcpp::NodeOptions options;
    options.use_global_arguments(false); 
    auto vrep_node = std::make_shared<rclcpp::Node>("vrep_node", options);

    // auto vrep_node = std::make_shared<rclcpp::Node>("vrep_node"); VERSION ELVENN


    // --- Publishers ---
    auto pubProductAdd = node->create_publisher<commande_locale::msg::MsgAddProduct>("/commande_locale/AddProduct", 10);
    auto pubModeType   = node->create_publisher<commande_locale::msg::MsgChoixMode>("/commande_locale/ChoixMode", 10);
    auto pub_stopSim   = node->create_publisher<std_msgs::msg::Byte>("/sim_ros_interface/StopSimulation", 100);
    auto pub_actuator  = node->create_publisher<std_msgs::msg::Byte>("/actuator", 100);
    auto pub_shutdown  = node->create_publisher<std_msgs::msg::Byte>("/commande_locale/shutdown", 10);

    // --- Subscribers ---
    auto sub_spawnShuttles   = node->create_subscription<std_msgs::msg::Int32>("/commande_locale/nbNavettes", 10, SpawnShuttlesCallback);
    auto sub_shutdown        = node->create_subscription<std_msgs::msg::Byte>("/commande_locale/shutdown", 10, ShutdownCallback);
    auto sub_coppeliaFinInit = node->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/FinInit", 10, CoppeliaFinInitCallback);
    auto sub_terminee        = node->create_subscription<std_msgs::msg::Byte>("/commande/PetriTermine", 10, IfPetriTermineCallback);
    auto sub_autorunsimu     = node->create_subscription<std_msgs::msg::Byte>("/commande/AutoRunSimu", 10, AutoRunSimuCallback);

    // --- Services ---
    auto serviceAddProduct = node->create_service<commande_locale::srv::SrvAddProduct>("srv_add_product", AddProduct);
    auto serviceFinInit    = node->create_service<commande_locale::srv::SrvFinInit>("srv_fin_init", FinInitCallback);

    // --- VREP Controller ---
    std::string nombreRobotStr = (argc > 1) ? argv[1] : "4";
    if(nombreRobotStr != "2" && nombreRobotStr != "4") nombreRobotStr = "4";
    std::string simulationFile = "Simulation" + nombreRobotStr + "Robots";
    VREPController.init(vrep_node, argv[0], simulationFile);

    // --- IN & OUT Controller ---
    inOutController IOController(&VREPController);
    IOController.init(node);

    // --- Attente du lancement de CoppeliaSim ---
    rclcpp::Rate rate(2);
    while(initCoppeliaEnCours && rclcpp::ok()){
        std::cout << "Attente fin demarrage Coppelia ..." << std::endl;
        rclcpp::spin_some(node);
        rate.sleep();
    }

    std::string cmd="ros2 launch launcher launch_beta.launch.py nbRobot:=" + nombreRobotStr + " &";
	system(cmd.c_str());

    std::cout << "Pause envoyée" << std::endl;
    VREPController.pause();

    while(initEnCours && rclcpp::ok()){
        std::cout << "Attente fin de l'initialisation ..." << std::endl;
        rclcpp::spin_some(node);
        rate.sleep();
    }


// --- Boucle principale utilisateur ou autorun ---
    if(!autorunstop){
        
        // --- NOUVEAUTÉ : ON LANCE LE MENU DANS UN THREAD SÉPARÉ ---
        std::thread menu_thread([&]() {
            while(rclcpp::ok()){
                std::string choix;
                std::cout << "\nQue voulez faire ?\n1- Ajouter un produit\n2- Pause simu\n3- Play simu\n4- Fin programme\n5- Simu ou Atelier ?\nChoix : ";
                std::cin >> choix;

                if(choix.length() != 1 || choix[0]<'1' || choix[0]>'9'){
                    std::cout << "[Erreur mauvais choix]" << std::endl;
                    std::cin.clear(); std::cin.ignore(256,'\n');
                    continue;
                }

                int choixInt = std::stoi(choix);
                switch(choixInt){
                    case 1:
                    {
                        int choixPoste, choixProduit;
                        std::cout << "Quel poste ? [1..8] "; std::cin >> choixPoste;
                        std::cout << "Quel produit ? [1..6] "; std::cin >> choixProduit;
                        if(choixPoste<1 || choixPoste>8 || choixProduit<1 || choixProduit>6){
                            std::cout << "[Erreur mauvais choix]" << std::endl;
                            std::cin.clear(); std::cin.ignore(256,'\n');
                            break;
                        }
                        msg0.num_poste = choixPoste;
                        msg0.num_produit = choixProduit*10+4;
                        pubProductAdd->publish(msg0);
                        VREPController.addProduct(choixProduit, choixPoste);
                        break;
                    }
                    case 2: VREPController.pause(); break;
                    case 3: VREPController.play(); break;
                    case 4:
                        pub_stopSim->publish(std_msgs::msg::Byte());
                        pub_shutdown->publish(std_msgs::msg::Byte());
                        //rclcpp::sleep_for(1s);
                        rclcpp::shutdown();
                        break;
                    case 5:
                    {
                        int choixMode;
                        std::cout << "Mode : Simu (0) ou Atelier (1)? "; std::cin >> choixMode;
                        msg1.mode = choixMode;
                        pubModeType->publish(msg1);
                        break;
                    }
                    default: std::cout << "[Erreur mauvais choix]" << std::endl; break;
                }
            }
        });
        menu_thread.detach(); // On détache le menu pour qu'il vive sa vie en arrière-plan
        // ----------------------------------------------------------

        // --- LA BOUCLE PRINCIPALE (Capteurs) N'EST PLUS BLOQUÉE ! ---
        rclcpp::Rate rate_spin(25); // On force une lecture à 25 Hz
        while(rclcpp::ok()) {
            rclcpp::spin_some(node); // On avale tous les capteurs en continu
            rate_spin.sleep();
        }
    } else {
        bool beg = true;
        while(rclcpp::ok()){
            if(beg){
                std::cout << "Lancement automatique de la simulation" << std::endl;
                VREPController.play();
                beg = false;
            }
            if(petrifinished){
                std::cout << "Fin Programme" << std::endl;
                pub_stopSim->publish(std_msgs::msg::Byte());
                pub_shutdown->publish(std_msgs::msg::Byte());
                //rclcpp::sleep_for(1s);
                rclcpp::shutdown();
                break;
            }
            rclcpp::spin_some(node);
        }
    }

    VREPController.close();
    return 0;
}
