-- MAIN SCRIPT ADAPTÉ ROS 2 (JAZZY)

function sysCall_init()
    sim.openModule(sim.handle_all)
    sim.handleGraph(sim.handle_all_except_explicit,0)

    -- Initialisation des variables globales du script original
    debut=1
    actuating=1

    -- Vérification rapide du plugin
    if not simROS2 then
        sim.displayDialog('Error','Le plugin ROS 2 n\'est pas chargé.',sim.dlgstyle_ok,false,nil,{0.8,0,0,0,0,0},{0.5,0,0,1,1,1})
        return
    end

    -------------------------------------------------------------------------
    -- DEFINITION DES PUBLISHERS ET SUBSCRIBERS (ROS 2)
    -------------------------------------------------------------------------

    -- 1. Services Généraux VREP Controller
    subChangeColorVrepController=simROS2.createSubscription('/sim_ros_interface/services/vrep_controller/ChangeColor','std_msgs/msg/Int32MultiArray','ChangeColorVrepControllerCallback')
    pubChangeColorVrepController=simROS2.createPublisher('/sim_ros_interface/services/response/vrep_controller/ChangeColor','std_msgs/msg/Byte')
    
    subGetColorVrepController=simROS2.createSubscription('/sim_ros_interface/services/vrep_controller/GetColor','std_msgs/msg/String','GetColorVrepControllerCallback')
    pubGetColorVrepController=simROS2.createPublisher('/sim_ros_interface/services/response/vrep_controller/GetColor','std_msgs/msg/Int32')
    
    subGetTimeLogManager=simROS2.createSubscription('/sim_ros_interface/services/LogManager/GetTime','std_msgs/msg/Byte','GetTimeLogManagerCallback')
    pubGetTimeLogManager=simROS2.createPublisher('/sim_ros_interface/services/response/LogManager/GetTime','std_msgs/msg/Float32')
    
    subStartSimulation=simROS2.createSubscription('/sim_ros_interface/services/vrep_controller/StartSimulation', 'std_msgs/msg/Byte', 'StartSimulationCallback')
    pubStartSimulation=simROS2.createPublisher('/sim_ros_interface/services/response/vrep_controller/StartSimulation', 'std_msgs/msg/Byte')
    
    subPauseSimulation=simROS2.createSubscription('/sim_ros_interface/services/vrep_controller/PauseSimulation', 'std_msgs/msg/Byte', 'PauseSimulationCallback')
    pubPauseSimulation=simROS2.createPublisher('/sim_ros_interface/services/response/vrep_controller/PauseSimulation', 'std_msgs/msg/Byte')
    
    subLoadModel=simROS2.createSubscription('/sim_ros_interface/services/vrep_controller/LoadModel', 'std_msgs/msg/String', 'LoadModelCallback')
    pubLoadModel=simROS2.createPublisher('/sim_ros_interface/services/response/vrep_controller/LoadModel', 'std_msgs/msg/Int32')
    
    subActuator=simROS2.createSubscription('/actuator','std_msgs/msg/Byte','ActuatorCallback')
    
    subStopSimulation=simROS2.createSubscription('/sim_ros_interface/StopSimulation','std_msgs/msg/Byte','StopSimulationCallback')
    pubFinInit=simROS2.createPublisher('/sim_ros_interface/FinInit','std_msgs/msg/Byte')
    
    -- 2. ROBOT 1
    subGetTimeRobot1=simROS2.createSubscription('/sim_ros_interface/services/robot1/GetTime','std_msgs/msg/Byte','GetTimeRobot1Callback')
    pubGetTimeRobot1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/GetTime','std_msgs/msg/Float32')
    
    subGetTimeRobot1Update=simROS2.createSubscription('/sim_ros_interface/services/robot1Update/GetTime','std_msgs/msg/Byte','GetTimeRobot1UpdateCallback')
    pubGetTimeRobot1Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot1Update/GetTime','std_msgs/msg/Float32')
    
    subHandleRobot1=simROS2.createSubscription('/sim_ros_interface/services/robot1/GetObjectHandle','std_msgs/msg/String','GetObjectHandleRobot1Callback')
    pubHandleRobot1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/GetObjectHandle','std_msgs/msg/Int32')
    
    subSetJointState1=simROS2.createSubscription('/sim_ros_interface/services/robot1/SetJointState','std_msgs/msg/Float32MultiArray','SetJointState1Callback')
    pubSetJointState1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/SetJointState','std_msgs/msg/Byte')
    
    subGetJointState1=simROS2.createSubscription('/sim_ros_interface/services/robot1/GetJointState','std_msgs/msg/Int32','GetJointState1Callback')
    pubGetJointState1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/GetJointState','sensor_msgs/msg/JointState')
    
    subChangeColor1=simROS2.createSubscription('/sim_ros_interface/services/robot1/ChangeColor','std_msgs/msg/Int32MultiArray','ChangeColor1Callback')
    pubChangeColor1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/ChangeColor','std_msgs/msg/Byte')
    
    subChangeShuttleColor1=simROS2.createSubscription('/sim_ros_interface/services/robot1/ChangeShuttleColor','std_msgs/msg/Int32MultiArray','ChangeShuttleColor1Callback')
    pubChangeShuttleColor1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/ChangeShuttleColor','std_msgs/msg/Byte')
    
    subGetColor1=simROS2.createSubscription('/sim_ros_interface/services/robot1/GetColor','std_msgs/msg/String','GetColor1Callback')
    pubGetColor1=simROS2.createPublisher('/sim_ros_interface/services/response/robot1/GetColor', 'std_msgs/msg/Int32')
    
    -- 3. ROBOT 2
    subGetTimeRobot2=simROS2.createSubscription('/sim_ros_interface/services/robot2/GetTime','std_msgs/msg/Byte','GetTimeRobot2Callback')
    pubGetTimeRobot2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/GetTime','std_msgs/msg/Float32')
    
    subGetTimeRobot2Update=simROS2.createSubscription('/sim_ros_interface/services/robot2Update/GetTime','std_msgs/msg/Byte','GetTimeRobot2UpdateCallback')
    pubGetTimeRobot2Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot2Update/GetTime','std_msgs/msg/Float32')
    
    subHandleRobot2=simROS2.createSubscription('/sim_ros_interface/services/robot2/GetObjectHandle','std_msgs/msg/String','GetObjectHandleRobot2Callback')
    pubHandleRobot2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/GetObjectHandle','std_msgs/msg/Int32')
    
    subSetJointState2=simROS2.createSubscription('/sim_ros_interface/services/robot2/SetJointState','std_msgs/msg/Float32MultiArray','SetJointState2Callback')
    pubSetJointState2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/SetJointState','std_msgs/msg/Byte')
    
    subGetJointState2=simROS2.createSubscription('/sim_ros_interface/services/robot2/GetJointState','std_msgs/msg/Int32','GetJointState2Callback')
    pubGetJointState2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/GetJointState','sensor_msgs/msg/JointState')
    
    subChangeColor2=simROS2.createSubscription('/sim_ros_interface/services/robot2/ChangeColor','std_msgs/msg/Int32MultiArray','ChangeColor2Callback')
    pubChangeColor2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/ChangeColor','std_msgs/msg/Byte')
    
    subChangeShuttleColor2=simROS2.createSubscription('/sim_ros_interface/services/robot2/ChangeShuttleColor','std_msgs/msg/Int32MultiArray','ChangeShuttleColor2Callback')
    pubChangeShuttleColor2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/ChangeShuttleColor','std_msgs/msg/Byte')
    
    subGetColor2=simROS2.createSubscription('/sim_ros_interface/services/robot2/GetColor','std_msgs/msg/String','GetColor2Callback')
    pubGetColor2=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/GetColor', 'std_msgs/msg/Int32')
    
    subGetColor2Update=simROS2.createSubscription('/sim_ros_interface/services/robot2/GetColorUpdate','std_msgs/msg/String','GetColor2UpdateCallback')
    pubGetColor2Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot2/GetColorUpdate', 'std_msgs/msg/Int32')
    
    -- 4. ROBOT 3
    subGetTimeRobot3=simROS2.createSubscription('/sim_ros_interface/services/robot3/GetTime','std_msgs/msg/Byte','GetTimeRobot3Callback')
    pubGetTimeRobot3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/GetTime','std_msgs/msg/Float32')
    
    subGetTimeRobot3Update=simROS2.createSubscription('/sim_ros_interface/services/robot3Update/GetTime','std_msgs/msg/Byte','GetTimeRobot3UpdateCallback')
    pubGetTimeRobot3Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot3Update/GetTime','std_msgs/msg/Float32')
    
    subHandleRobot3=simROS2.createSubscription('/sim_ros_interface/services/robot3/GetObjectHandle','std_msgs/msg/String','GetObjectHandleRobot3Callback')
    pubHandleRobot3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/GetObjectHandle','std_msgs/msg/Int32')
    
    subSetJointState3=simROS2.createSubscription('/sim_ros_interface/services/robot3/SetJointState','std_msgs/msg/Float32MultiArray','SetJointState3Callback')
    pubSetJointState3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/SetJointState','std_msgs/msg/Byte')
    
    subGetJointState3=simROS2.createSubscription('/sim_ros_interface/services/robot3/GetJointState','std_msgs/msg/Int32','GetJointState3Callback')
    pubGetJointState3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/GetJointState','sensor_msgs/msg/JointState')
    
    subChangeColor3=simROS2.createSubscription('/sim_ros_interface/services/robot3/ChangeColor','std_msgs/msg/Int32MultiArray','ChangeColor3Callback')
    pubChangeColor3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/ChangeColor','std_msgs/msg/Byte')
    
    subChangeShuttleColor3=simROS2.createSubscription('/sim_ros_interface/services/robot3/ChangeShuttleColor','std_msgs/msg/Int32MultiArray','ChangeShuttleColor3Callback')
    pubChangeShuttleColor3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/ChangeShuttleColor','std_msgs/msg/Byte')
    
    subGetColor3=simROS2.createSubscription('/sim_ros_interface/services/robot3/GetColor','std_msgs/msg/String','GetColor3Callback')
    pubGetColor3=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/GetColor', 'std_msgs/msg/Int32')
    
    subGetColor3Update=simROS2.createSubscription('/sim_ros_interface/services/robot3/GetColorUpdate','std_msgs/msg/String','GetColor3UpdateCallback')
    pubGetColor3Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot3/GetColorUpdate', 'std_msgs/msg/Int32')
    
    -- 5. ROBOT 4
    subGetTimeRobot4=simROS2.createSubscription('/sim_ros_interface/services/robot4/GetTime','std_msgs/msg/Byte','GetTimeRobot4Callback')
    pubGetTimeRobot4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/GetTime','std_msgs/msg/Float32')
    
    subGetTimeRobot4Update=simROS2.createSubscription('/sim_ros_interface/services/robot4Update/GetTime','std_msgs/msg/Byte','GetTimeRobot4UpdateCallback')
    pubGetTimeRobot4Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot4Update/GetTime','std_msgs/msg/Float32')
    
    subHandleRobot4=simROS2.createSubscription('/sim_ros_interface/services/robot4/GetObjectHandle','std_msgs/msg/String','GetObjectHandleRobot4Callback')
    pubHandleRobot4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/GetObjectHandle','std_msgs/msg/Int32')
    
    subSetJointState4=simROS2.createSubscription('/sim_ros_interface/services/robot4/SetJointState','std_msgs/msg/Float32MultiArray','SetJointState4Callback')
    pubSetJointState4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/SetJointState','std_msgs/msg/Byte')
    
    subGetJointState4=simROS2.createSubscription('/sim_ros_interface/services/robot4/GetJointState','std_msgs/msg/Int32','GetJointState4Callback')
    pubGetJointState4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/GetJointState','sensor_msgs/msg/JointState')
    
    subChangeColor4=simROS2.createSubscription('/sim_ros_interface/services/robot4/ChangeColor','std_msgs/msg/Int32MultiArray','ChangeColor4Callback')
    pubChangeColor4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/ChangeColor','std_msgs/msg/Byte')
    
    subChangeShuttleColor4=simROS2.createSubscription('/sim_ros_interface/services/robot4/ChangeShuttleColor','std_msgs/msg/Int32MultiArray','ChangeShuttleColor4Callback')
    pubChangeShuttleColor4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/ChangeShuttleColor','std_msgs/msg/Byte')
    
    subGetColor4=simROS2.createSubscription('/sim_ros_interface/services/robot4/GetColor','std_msgs/msg/String','GetColor4Callback')
    pubGetColor4=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/GetColor', 'std_msgs/msg/Int32')
    
    subGetColor4Update=simROS2.createSubscription('/sim_ros_interface/services/robot4/GetColorUpdate','std_msgs/msg/String','GetColor4UpdateCallback')
    pubGetColor4Update=simROS2.createPublisher('/sim_ros_interface/services/response/robot4/GetColorUpdate', 'std_msgs/msg/Int32')

    -- Configuration initiale des couleurs (transparence)
    sim.setShapeColor(sim.getObjectHandle('5mx5mWoodenFloor'),nil,sim.colorcomponent_transparency,{0})
    sim.setShapeColor(sim.getObjectHandle('240cmHighWall400cm'),nil,sim.colorcomponent_transparency,{0})
end

--------------------------------------------------------------------------
-- CALLBACKS FUNCTIONS
--------------------------------------------------------------------------

function ChangeColorVrepControllerCallback(msg)
    id=msg.data[1]
    local colorSignal0 = 'Table#'..id..'#0_color'
    local colorSignal1 = 'Table#'..id..'#1_color'
    local colorSignal2 = 'Table#'..id..'#2_color'
    local colorSignal3 = 'Table#'..id..'#3_color'
    local colorSignal4 = 'Table#'..id..'#4_color'
    local colorSignal5 = 'Table#'..id..'#5_color'
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeColorVrepController,{data=0})
end

function GetColorVrepControllerCallback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end
    simROS2.publish(pubGetColorVrepController,{data=color})
end

function GetTimeLogManagerCallback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeLogManager,{data=time})
end

function StartSimulationCallback(msg)
    sim.startSimulation()
    simROS2.publish(pubStartSimulation, {data=0})
end

function PauseSimulationCallback(msg)
    sim.pauseSimulation()
    simROS2.publish(pubPauseSimulation,{data=0})
end

function LoadModelCallback(msg)
    local handle=sim.loadModel(msg.data)
    simROS2.publish(pubLoadModel,{data=handle})
end

function ActuatorCallback(msg)
    sim.setShapeColor(sim.getObjectHandle('5mx5mWoodenFloor'),nil,sim.colorcomponent_transparency,{actuating})
    sim.setShapeColor(sim.getObjectHandle('240cmHighWall400cm'),nil,sim.colorcomponent_transparency,{actuating})
    if(actuating==1) then
        actuating=0
    else
        actuating=1
    end
end

function StopSimulationCallback(msg)
    sim.stopSimulation()
end

-- Callbacks Robot 1 --
function SetJointState1Callback(msg)
    -- MultiArray data handling
    local nHandle=msg.layout.dim[1].size
    local nValues=nHandle 
    local handles={}
    for i=1,nHandle,1 do handles[i]=msg.data[i] end

    local values={}
    for i=1,nValues,1 do values[i]=msg.data[nHandle+i] end
        
    for i=1,nHandle,1 do sim.setJointTargetPosition(handles[i],values[i]) end
    simROS2.publish(pubSetJointState1,{data=0})
end

function GetJointState1Callback(msg)
    local pos=sim.getJointPosition(msg.data)
    simROS2.publish(pubGetJointState1,{position={pos}})
end

function GetTimeRobot1Callback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot1,{data=time})
end

function GetTimeRobot1UpdateCallback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot1Update,{data=time})
end

function GetObjectHandleRobot1Callback(msg)
    local objectHandle=sim.getObjectHandle(msg.data)
    simROS2.publish(pubHandleRobot1,{data=objectHandle})
end

function ChangeColor1Callback(msg)
    id=msg.data[1]
    local colorSignal0 = 'Table#'..id..'#0_color'
    local colorSignal1 = 'Table#'..id..'#1_color'
    local colorSignal2 = 'Table#'..id..'#2_color'
    local colorSignal3 = 'Table#'..id..'#3_color'
    local colorSignal4 = 'Table#'..id..'#4_color'
    local colorSignal5 = 'Table#'..id..'#5_color'
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeColor1,{data=0})
end

function ChangeShuttleColor1Callback(msg)
    id=msg.data[1]
    if id==1 then lettre='A'
    elseif id==2 then lettre='B'
    elseif id==3 then lettre='C'
    elseif id==4 then lettre='D'
    elseif id==5 then lettre='E'
    elseif id==6 then lettre='F'
    elseif id==0 then lettre='Z' end
    
    local colorSignal0 = 'Shuttle'..lettre..'#0_color'
    local colorSignal1 = 'Shuttle'..lettre..'#1_color'
    local colorSignal2 = 'Shuttle'..lettre..'#2_color'
    local colorSignal3 = 'Shuttle'..lettre..'#3_color'
    local colorSignal4 = 'Shuttle'..lettre..'#4_color'
    local colorSignal5 = 'Shuttle'..lettre..'#5_color'
    
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeShuttleColor1,{data=0})
end

function GetColor1Callback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end 
    simROS2.publish(pubGetColor1,{data=color})
end

-- Callbacks Robot 2 --
function SetJointState2Callback(msg)
    local nHandle=msg.layout.dim[1].size
    local nValues=nHandle 
    local handles={}
    for i=1,nHandle,1 do handles[i]=msg.data[i] end

    local values={}
    for i=1,nValues,1 do values[i]=msg.data[nHandle+i] end
    for i=1,nHandle,1 do sim.setJointTargetPosition(handles[i],values[i]) end
    simROS2.publish(pubSetJointState2,{data=0})
end

function GetJointState2Callback(msg)
    local pos=sim.getJointPosition(msg.data)
    simROS2.publish(pubGetJointState2,{position={pos}})
end

function GetTimeRobot2Callback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot2,{data=time})
end

function GetTimeRobot2UpdateCallback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot2Update,{data=time})
end

function GetObjectHandleRobot2Callback(msg)
    local objectHandle=sim.getObjectHandle(msg.data)
    simROS2.publish(pubHandleRobot2,{data=objectHandle})
end

function ChangeColor2Callback(msg)
    id=msg.data[1]
    local colorSignal0 = 'Table#'..id..'#0_color'
    local colorSignal1 = 'Table#'..id..'#1_color'
    local colorSignal2 = 'Table#'..id..'#2_color'
    local colorSignal3 = 'Table#'..id..'#3_color'
    local colorSignal4 = 'Table#'..id..'#4_color'
    local colorSignal5 = 'Table#'..id..'#5_color'
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeColor2,{data=0})
end

function ChangeShuttleColor2Callback(msg)
    id=msg.data[1]
    if id==1 then lettre='A'
    elseif id==2 then lettre='B'
    elseif id==3 then lettre='C'
    elseif id==4 then lettre='D'
    elseif id==5 then lettre='E'
    elseif id==6 then lettre='F'
    elseif id==0 then lettre='Z' end
    
    local colorSignal0 = 'Shuttle'..lettre..'#0_color'
    local colorSignal1 = 'Shuttle'..lettre..'#1_color'
    local colorSignal2 = 'Shuttle'..lettre..'#2_color'
    local colorSignal3 = 'Shuttle'..lettre..'#3_color'
    local colorSignal4 = 'Shuttle'..lettre..'#4_color'
    local colorSignal5 = 'Shuttle'..lettre..'#5_color'
    
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeShuttleColor2,{data=0})
end

function GetColor2Callback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end 
    simROS2.publish(pubGetColor2,{data=color})
end

function GetColor2UpdateCallback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end
    simROS2.publish(pubGetColor2Update,{data=color})
end

-- Callbacks Robot 3 --
function SetJointState3Callback(msg)
    local nHandle=msg.layout.dim[1].size
    local nValues=nHandle 
    local handles={}
    for i=1,nHandle,1 do handles[i]=msg.data[i] end

    local values={}
    for i=1,nValues,1 do values[i]=msg.data[nHandle+i] end
    for i=1,nHandle,1 do sim.setJointTargetPosition(handles[i],values[i]) end
    simROS2.publish(pubSetJointState3,{data=0})
end

function GetJointState3Callback(msg)
    local pos=sim.getJointPosition(msg.data)
    simROS2.publish(pubGetJointState3,{position={pos}})
end

function GetTimeRobot3Callback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot3,{data=time})
end

function GetTimeRobot3UpdateCallback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot3Update,{data=time})
end

function GetObjectHandleRobot3Callback(msg)
    local objectHandle=sim.getObjectHandle(msg.data)
    simROS2.publish(pubHandleRobot3,{data=objectHandle})
end

function ChangeColor3Callback(msg)
    id=msg.data[1]
    local colorSignal0 = 'Table#'..id..'#0_color'
    local colorSignal1 = 'Table#'..id..'#1_color'
    local colorSignal2 = 'Table#'..id..'#2_color'
    local colorSignal3 = 'Table#'..id..'#3_color'
    local colorSignal4 = 'Table#'..id..'#4_color'
    local colorSignal5 = 'Table#'..id..'#5_color'
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeColor3,{data=0})
end

function ChangeShuttleColor3Callback(msg)
    id=msg.data[1]
    if id==1 then lettre='A'
    elseif id==2 then lettre='B'
    elseif id==3 then lettre='C'
    elseif id==4 then lettre='D'
    elseif id==5 then lettre='E'
    elseif id==6 then lettre='F'
    elseif id==0 then lettre='Z' end
    
    local colorSignal0 = 'Shuttle'..lettre..'#0_color'
    local colorSignal1 = 'Shuttle'..lettre..'#1_color'
    local colorSignal2 = 'Shuttle'..lettre..'#2_color'
    local colorSignal3 = 'Shuttle'..lettre..'#3_color'
    local colorSignal4 = 'Shuttle'..lettre..'#4_color'
    local colorSignal5 = 'Shuttle'..lettre..'#5_color'
    
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeShuttleColor3,{data=0})
end

function GetColor3Callback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end
    simROS2.publish(pubGetColor3,{data=color})
end

function GetColor3UpdateCallback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end
    simROS2.publish(pubGetColor3Update,{data=color})
end

-- Callbacks Robot 4 --
function SetJointState4Callback(msg)
    local nHandle=msg.layout.dim[1].size
    local nValues=nHandle 
    local handles={}
    for i=1,nHandle,1 do handles[i]=msg.data[i] end

    local values={}
    for i=1,nValues,1 do values[i]=msg.data[nHandle+i] end
    for i=1,nHandle,1 do sim.setJointTargetPosition(handles[i],values[i]) end
    simROS2.publish(pubSetJointState4,{data=0})
end

function GetJointState4Callback(msg)
    local pos=sim.getJointPosition(msg.data)
    simROS2.publish(pubGetJointState4,{position={pos}})
end

function GetTimeRobot4Callback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot4,{data=time})
end

function GetTimeRobot4UpdateCallback(msg)
    local time=sim.getSimulationTime()
    simROS2.publish(pubGetTimeRobot4Update,{data=time})
end

function GetObjectHandleRobot4Callback(msg)
    local objectHandle=sim.getObjectHandle(msg.data)
    simROS2.publish(pubHandleRobot4,{data=objectHandle})
end

function ChangeColor4Callback(msg)
    id=msg.data[1]
    local colorSignal0 = 'Table#'..id..'#0_color'
    local colorSignal1 = 'Table#'..id..'#1_color'
    local colorSignal2 = 'Table#'..id..'#2_color'
    local colorSignal3 = 'Table#'..id..'#3_color'
    local colorSignal4 = 'Table#'..id..'#4_color'
    local colorSignal5 = 'Table#'..id..'#5_color'
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeColor4,{data=0})
end

function ChangeShuttleColor4Callback(msg)
    id=msg.data[1]
    if id==1 then lettre='A'
    elseif id==2 then lettre='B'
    elseif id==3 then lettre='C'
    elseif id==4 then lettre='D'
    elseif id==5 then lettre='E'
    elseif id==6 then lettre='F'
    elseif id==0 then lettre='Z' end
    
    local colorSignal0 = 'Shuttle'..lettre..'#0_color'
    local colorSignal1 = 'Shuttle'..lettre..'#1_color'
    local colorSignal2 = 'Shuttle'..lettre..'#2_color'
    local colorSignal3 = 'Shuttle'..lettre..'#3_color'
    local colorSignal4 = 'Shuttle'..lettre..'#4_color'
    local colorSignal5 = 'Shuttle'..lettre..'#5_color'
    
    local color0=msg.data[2] or 0
    local color1=msg.data[3] or 0
    local color2=msg.data[4] or 0
    local color3=msg.data[5] or 0
    local color4=msg.data[6] or 0
    local color5=msg.data[7] or 0
    
    sim.setInt32Signal(colorSignal0,color0)
    sim.setInt32Signal(colorSignal1,color1)
    sim.setInt32Signal(colorSignal2,color2)
    sim.setInt32Signal(colorSignal3,color3)
    sim.setInt32Signal(colorSignal4,color4)
    sim.setInt32Signal(colorSignal5,color5)
    
    simROS2.publish(pubChangeShuttleColor4,{data=0})
end

function GetColor4Callback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end
    simROS2.publish(pubGetColor4,{data=color})
end

function GetColor4UpdateCallback(msg)
    local nameColorSignal=msg.data
    local color=sim.getInt32Signal(nameColorSignal)
    if color == nil then color = 0 end
    simROS2.publish(pubGetColor4Update,{data=color})
end

function getHandleInside(name)
    return sim.getObjectHandle(name)
end

function sysCall_actuation()
    -- Partie spécifique: signal de fin d'init et gestion threads
    if(debut>0)then
        if(debut==10)then
            simROS2.publish(pubFinInit,{data=0})
            debut=0
        else
            debut=debut+1
        end
    end

    -- Ligne vitale pour les Aiguillages et autres scripts enfants
    sim.handleChildScripts(sim.syscb_actuation)
    
    -- Fonctions standards
    sim.handleMechanism(sim.handle_all_except_explicit)
    sim.handleIkGroup(sim.handle_all_except_explicit)
    sim.handleDynamics(sim.getSimulationTimeStep())
end

function sysCall_sensing()
    sim.handleChildScripts(sim.syscb_sensing)
    sim.handleGraph(sim.handle_all_except_explicit,sim.getSimulationTime()+sim.getSimulationTimeStep())
end

function sysCall_cleanup()
    sim.closeModule(sim.handle_all)
end
