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

    // Utiliser un exécuteur multi-threadé (ex: 4 threads pour vos 4 cœurs VM)
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node_aiguillage);
    
    RCLCPP_INFO(node_aiguillage->get_logger(), "Nœud démarré avec MultiThreadedExecutor.");
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
