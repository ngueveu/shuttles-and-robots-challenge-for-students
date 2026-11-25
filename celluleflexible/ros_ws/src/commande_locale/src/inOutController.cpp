#include "inOutController.h"
#include <cmath>

inOutController::inOutController(vrepController* vrepSA)
{
    vrepServiceAcces = vrepSA;
}

void inOutController::TypeMode(const commande_locale::msg::MsgChoixMode::SharedPtr msg1)
{
    mode = msg1->mode;
}

void inOutController::SensorCallbackRail(const std_msgs::msg::Int32::SharedPtr msg)
{
    if (mode == 0) //je les avais mis en commentaires au debut quand ca marchait pas

    {
        for(int i=1;i<=10;i++)
            SensorState.cp[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;

        planifRailSensorState->publish(SensorState);
    }
}

void inOutController::SensorCallbackStop(const std_msgs::msg::Int32::SharedPtr msg)
{
    if (mode == 0)

    {
        for(int i=1;i<=24;i++)
            SensorState.ps[i] = (msg->data & (int32_t)pow(2,i-1)) > 0;

        planifRailSensorState->publish(SensorState);
    }
}

void inOutController::SensorCallbackSwitch(const std_msgs::msg::Int32::SharedPtr msg)
{
    if (mode == 0)

    {
        for(int i=1;i<=12;i++)
        {
            SensorState.dd[i] = (msg->data & (int32_t)pow(2,2*i-2)) > 0;
            SensorState.dg[i] = (msg->data & (int32_t)pow(2,2*i-1)) > 0;
        }

        planifRailSensorState->publish(SensorState);
    }
}

void inOutController::StateSwitchCallBack(const commande_locale::msg::MsgSwitchControl::SharedPtr msg)
{
    int SwitchRightControl(0), SwitchLeftControl(0), SwitchLockControl(0);
    std_msgs::msg::Int32 Right, Left, Lock;

    SwitchControl.rd = msg->rd;
    SwitchControl.rd = msg->rg;
    SwitchControl.lock = msg->lock;

    for (int i=1;i<=12;i++)
    {
        if (msg->rd[i])
            SwitchRightControl += pow(2,i-1);

        if (msg->rg[i])
            SwitchLeftControl += pow(2,i-1);

        if (msg->lock[i])
            SwitchLockControl += pow(2,i-1);
    }

    Right.data = SwitchRightControl;
    Left.data  = SwitchLeftControl;
    Lock.data  = SwitchLockControl;

    VREPSwitchControllerRight->publish(Right);
    VREPSwitchControllerLeft->publish(Left);
    VREPSwitchControllerLock->publish(Lock);
}

void inOutController::StateStopCallBack(const commande_locale::msg::MsgStopControl::SharedPtr msg)
{
    int StopControlInt(0), GoControlInt(0);
    std_msgs::msg::Int32 Stop, Go;

    StopControl.stop = msg->stop;
    StopControl.go   = msg->go;

    for (int i=1;i<=24;i++)
    {
        if (msg->stop[i])
            StopControlInt += pow(2,i-1);

        if (msg->go[i])
            GoControlInt += pow(2,i-1);
    }

    Stop.data = StopControlInt;
    Go.data   = GoControlInt;

    VREPStopController->publish(Stop);
    VREPGoController->publish(Go);
}

void inOutController::StatePinCallBack(const commande_locale::msg::MsgPinControl::SharedPtr msg)
{
    int PinOnControlInt(0), PinOffControlInt(0);
    std_msgs::msg::Int32 PinOn, PinOff;

    PinControl.pinon  = msg->pinon;
    PinControl.pinoff = msg->pinoff;

    for (int i=1;i<=8;i++)
    {
        if (msg->pinon[i])
            PinOnControlInt += pow(2,i-1);

        if (msg->pinoff[i])
            PinOffControlInt += pow(2,i-1);
    }

    PinOn.data  = PinOnControlInt;
    PinOff.data = PinOffControlInt;

    for(int i=1;i<=8;i++)
        SensorState.cpi[i] = (PinOn.data & (int32_t)pow(2,i-1)) > 0;

    planifRailSensorState->publish(SensorState);
}


void inOutController::init(rclcpp::Node::SharedPtr n)
{
    node = n;

    // Subscribers
    VREPsubRailSensor = node->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/RailSensor", 100,
        std::bind(&inOutController::SensorCallbackRail, this, std::placeholders::_1));

    VREPsubStopSensor = node->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/StopSensor", 100,
        std::bind(&inOutController::SensorCallbackStop, this, std::placeholders::_1));

    VREPsubSwitchSensor = node->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/aig", 100,
        std::bind(&inOutController::SensorCallbackSwitch, this, std::placeholders::_1));

    planifSubSwitchState = node->create_subscription<commande_locale::msg::MsgSwitchControl>("/commande/Simulation/Actionneurs_aiguillages", 100,
        std::bind(&inOutController::StateSwitchCallBack, this, std::placeholders::_1));

    planifSubStopState = node->create_subscription<commande_locale::msg::MsgStopControl>("/commande/Simulation/Actionneurs_stops", 100,
        std::bind(&inOutController::StateStopCallBack, this, std::placeholders::_1));

    planifSubPinState = node->create_subscription<commande_locale::msg::MsgPinControl>("/commande/Simulation/Actionneurs_pins", 100,
        std::bind(&inOutController::StatePinCallBack, this, std::placeholders::_1));

    choixMode = node->create_subscription<commande_locale::msg::MsgChoixMode>("/commande_locale/ChoixMode", 10,
        std::bind(&inOutController::TypeMode, this, std::placeholders::_1));

    // Publishers
    VREPSwitchControllerRight = node->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerRight", 100);

    VREPSwitchControllerLeft = node->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLeft", 100);

    VREPSwitchControllerLock = node->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/SwitchControllerLock", 100);

    VREPStopController = node->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/StopController", 100);

    VREPGoController = node->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/GoController", 100);

    planifRailSensorState = node->create_publisher<aiguillages::msg::MsgSensorState>("/commande/Simulation/Capteurs", 100);

    // Initialisation capteurs ergots
    for(int i=1;i<9;i++)
        SensorState.cpi[i] = 0;

    //rclcpp::sleep_for(std::chrono::seconds(1));
}
