if (sim_call_type==sim.syscb_init) then
    -- 1. Vérification du plugin ROS 2 (ROS2Interface au lieu de ROSInterface)
    local moduleName=0
    local moduleVersion=0
    local index=0
    local pluginNotFound=true
    while moduleName do
        moduleName,moduleVersion=sim.getModuleName(index)
        -- Changement ici : on cherche 'ROS2Interface'
        if (moduleName=='ROS2Interface') then
            pluginNotFound=false
        end
        index=index+1
    end

    if (pluginNotFound) then
        -- sim.displayDialog('Error','ROS 2 plugin was not found.&&nSimulation will not run properly',sim.dlgstyle_ok,false,nil,{0.8,0,0,0,0,0},{0.5,0,0,1,1,1})
    else
    
        -- 2. Création du Publisher pour ROS 2
        -- Syntaxe : simROS2.createPublisher(TopicName, MessageType)
        -- Note : En ROS 2, le type complet est souvent requis 'std_msgs/msg/Int32'
        

        pub=simROS2.createPublisher('SwitchSensor','std_msgs/msg/Int32')
    end
end

if (sim_call_type==sim.syscb_actuation) then

end

if (sim_call_type==sim.syscb_sensing) then
    -- La logique de calcul "sum" reste identique
    sum = math.pow(2,0)*sim.checkProximitySensor(sim.getObjectHandle('D01D'),sim.handle_all,NULL)
    +math.pow(2,1)*sim.checkProximitySensor(sim.getObjectHandle('D01G'),sim.handle_all,NULL)
    +math.pow(2,2)*sim.checkProximitySensor(sim.getObjectHandle('D02D'),sim.handle_all,NULL)
    +math.pow(2,3)*sim.checkProximitySensor(sim.getObjectHandle('D02G'),sim.handle_all,NULL)
    +math.pow(2,4)*sim.checkProximitySensor(sim.getObjectHandle('D03D'),sim.handle_all,NULL)
    +math.pow(2,5)*sim.checkProximitySensor(sim.getObjectHandle('D03G'),sim.handle_all,NULL)
    +math.pow(2,6)*sim.checkProximitySensor(sim.getObjectHandle('D04D'),sim.handle_all,NULL)
    +math.pow(2,7)*sim.checkProximitySensor(sim.getObjectHandle('D04G'),sim.handle_all,NULL)
    +math.pow(2,8)*sim.checkProximitySensor(sim.getObjectHandle('D05D'),sim.handle_all,NULL)
    +math.pow(2,9)*sim.checkProximitySensor(sim.getObjectHandle('D05G'),sim.handle_all,NULL)
    +math.pow(2,10)*sim.checkProximitySensor(sim.getObjectHandle('D06D'),sim.handle_all,NULL)
    +math.pow(2,11)*sim.checkProximitySensor(sim.getObjectHandle('D06G'),sim.handle_all,NULL)
    +math.pow(2,12)*sim.checkProximitySensor(sim.getObjectHandle('D07D'),sim.handle_all,NULL)
    +math.pow(2,13)*sim.checkProximitySensor(sim.getObjectHandle('D07G'),sim.handle_all,NULL)
    +math.pow(2,14)*sim.checkProximitySensor(sim.getObjectHandle('D08D'),sim.handle_all,NULL)
    +math.pow(2,15)*sim.checkProximitySensor(sim.getObjectHandle('D08G'),sim.handle_all,NULL)
    +math.pow(2,16)*sim.checkProximitySensor(sim.getObjectHandle('D09D'),sim.handle_all,NULL)
    +math.pow(2,17)*sim.checkProximitySensor(sim.getObjectHandle('D09G'),sim.handle_all,NULL)
    +math.pow(2,18)*sim.checkProximitySensor(sim.getObjectHandle('D10D'),sim.handle_all,NULL)
    +math.pow(2,19)*sim.checkProximitySensor(sim.getObjectHandle('D10G'),sim.handle_all,NULL)
    +math.pow(2,20)*sim.checkProximitySensor(sim.getObjectHandle('D11D'),sim.handle_all,NULL)
    +math.pow(2,21)*sim.checkProximitySensor(sim.getObjectHandle('D11G'),sim.handle_all,NULL)
    +math.pow(2,22)*sim.checkProximitySensor(sim.getObjectHandle('D12D'),sim.handle_all,NULL)
    +math.pow(2,23)*sim.checkProximitySensor(sim.getObjectHandle('D12G'),sim.handle_all,NULL)
    
    -- 3. Publication ROS 2
    if pub then
        simROS2.publish(pub,{data=sum})
    end
end

if (sim_call_type==sim.syscb_cleanup) then

end
