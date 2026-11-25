#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/byte.hpp"
#include "Aiguillage.h"
#include <memory>

void shutdown_callback(const std_msgs::msg::Byte::SharedPtr msg)
{
    (void)msg;
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Signal d'arrêt reçu. Fermeture du nœud...");
    rclcpp::shutdown();
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node_aiguillage = std::make_shared<Aiguillage>();
    
    // On souscrit au topic de shutdown
    auto sub_shutdown = node_aiguillage->create_subscription<std_msgs::msg::Byte>("/commande_locale/shutdown", 10, shutdown_callback);


    // Mets ça pour forcer 12 threads (un par aiguillage possible + capteurs) :
    rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 15);
    executor.add_node(node_aiguillage);
    
    RCLCPP_INFO(node_aiguillage->get_logger(), "Nœud démarré avec MultiThreadedExecutor.");
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
