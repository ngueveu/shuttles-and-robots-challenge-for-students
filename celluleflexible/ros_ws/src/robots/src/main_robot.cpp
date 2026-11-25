/**** Projet long N7 2017 - ROS2 version ****/
//Pour lancer ce package individuellement sans launch, faire "ros2 run robots robot 1" (lance un robot)
//Pour lancer ce package individuellement à partir du launch, faire "ros2 launch robots robotsGauche.launch.py" (lance deux robots dans des fenêtres xterm)


#include "Robot.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/byte.hpp>
#include <memory>
#include <string>

using std::string;


// Callback de shutdown
// Si un message est publié sur le topic shutdown, on envoit un signal de fin
void ShutdownCallback(const std_msgs::msg::Byte::SharedPtr /*msg*/)
{
    rclcpp::shutdown();
}

//Quand on fait "ros2 run robots robot 1", argv[0]=robot et argv[1]=1
int main(int argc, char **argv)
{
    // Initialisation du noeud ROS2
    rclcpp::init(argc, argv);
    int num_robot = atoi(argv[1]);

    //Création d'un noeud ROS2 nommé "robots"
    auto robot = std::make_shared<Robot>(num_robot);

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(robot);

    std::thread spin_thread([&executor]() {
        executor.spin();
    });

    // On souscrit au topic de shutdown
    auto sub_shutdown = robot->create_subscription<std_msgs::msg::Byte>("/commande_locale/shutdown", 10, ShutdownCallback);

    // Initialisation des publishers, subscribers et services
    //Appelle la fonction init du fichier Robot.cpp
    robot->init();

    rclcpp::Rate loop_rate(25);
    int compteur = 0;
    while(rclcpp::ok())
    {
        if(compteur++ > 25)
        {
            robot->update();
            compteur = 0;
        }
        loop_rate.sleep();
    }

    spin_thread.join();

    rclcpp::shutdown();
    return 0;
}
