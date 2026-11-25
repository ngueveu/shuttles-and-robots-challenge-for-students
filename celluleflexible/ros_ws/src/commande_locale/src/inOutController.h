#ifndef IN_OUT_CONTROLLER_HPP
#define IN_OUT_CONTROLLER_HPP

#include "vrepController.h"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/int32.hpp>

#include "commande_locale/msg/msg_switch_control.hpp"
#include "commande_locale/msg/msg_stop_control.hpp"
#include "commande_locale/msg/msg_pin_control.hpp"
#include "commande_locale/msg/msg_choix_mode.hpp"
#include "aiguillages/msg/msg_sensor_state.hpp"

class inOutController
{
private:
    vrepController* vrepServiceAcces;
    rclcpp::Node::SharedPtr node;

    // Publishers
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr VREPSwitchControllerRight;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr VREPSwitchControllerLeft;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr VREPSwitchControllerLock;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr VREPStopController;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr VREPGoController;
    rclcpp::Publisher<aiguillages::msg::MsgSensorState>::SharedPtr planifRailSensorState;

    // Subscribers
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr VREPsubRailSensor;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr VREPsubStopSensor;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr VREPsubSwitchSensor;
    rclcpp::Subscription<commande_locale::msg::MsgSwitchControl>::SharedPtr planifSubSwitchState;
    rclcpp::Subscription<commande_locale::msg::MsgStopControl>::SharedPtr planifSubStopState;
    rclcpp::Subscription<commande_locale::msg::MsgPinControl>::SharedPtr planifSubPinState;
    rclcpp::Subscription<commande_locale::msg::MsgChoixMode>::SharedPtr choixMode;

    commande_locale::msg::MsgStopControl StopControl;
    commande_locale::msg::MsgPinControl PinControl;
    commande_locale::msg::MsgSwitchControl SwitchControl;
    aiguillages::msg::MsgSensorState SensorState;

    int mode = 0;

public:
    inOutController(vrepController* vrepSA);
    void init(rclcpp::Node::SharedPtr n);
    void TypeMode(const commande_locale::msg::MsgChoixMode::SharedPtr msg);

    // Sensors
    void SensorCallbackRail(const std_msgs::msg::Int32::SharedPtr msg);
    void SensorCallbackStop(const std_msgs::msg::Int32::SharedPtr msg);
    void SensorCallbackSwitch(const std_msgs::msg::Int32::SharedPtr msg);

    // Actuators
    void StateSwitchCallBack(const commande_locale::msg::MsgSwitchControl::SharedPtr msg);
    void StateStopCallBack(const commande_locale::msg::MsgStopControl::SharedPtr msg);
    void StatePinCallBack(const commande_locale::msg::MsgPinControl::SharedPtr msg);
  
};

#endif

