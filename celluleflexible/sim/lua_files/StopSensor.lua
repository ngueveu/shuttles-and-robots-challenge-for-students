-- DO NOT WRITE CODE OUTSIDE OF THE if-then-end SECTIONS BELOW!!

if (sim_call_type==sim.syscb_init) then
    pub = nil
    if simROS2 then
        -- Création du publisher ROS 2
        -- Attention : vérifie si ton architecture attend '/StopSensor' ou '/sim_ros_interface/StopSensor'
        local status, createdPub = pcall(simROS2.createPublisher, '/sim_ros_interface/StopSensor', 'std_msgs/msg/Int32')
        if status and createdPub then
            pub = createdPub
        end

    end
end

if (sim_call_type==sim.syscb_actuation) then
end

if (sim_call_type==sim.syscb_sensing) then
    if not pub then return end

    -- Lecture des 24 capteurs (PS01 à PS24). Remplacement de NULL par nil pour Lua.
    sum = math.pow(2,0)*sim.checkProximitySensor(sim.getObjectHandle('PS01'),sim.handle_all,nil)
    +math.pow(2,1)*sim.checkProximitySensor(sim.getObjectHandle('PS02'),sim.handle_all,nil)
    +math.pow(2,2)*sim.checkProximitySensor(sim.getObjectHandle('PS03'),sim.handle_all,nil)
    +math.pow(2,3)*sim.checkProximitySensor(sim.getObjectHandle('PS04'),sim.handle_all,nil)
    +math.pow(2,4)*sim.checkProximitySensor(sim.getObjectHandle('PS05'),sim.handle_all,nil)
    +math.pow(2,5)*sim.checkProximitySensor(sim.getObjectHandle('PS06'),sim.handle_all,nil)
    +math.pow(2,6)*sim.checkProximitySensor(sim.getObjectHandle('PS07'),sim.handle_all,nil)
    +math.pow(2,7)*sim.checkProximitySensor(sim.getObjectHandle('PS08'),sim.handle_all,nil)
    +math.pow(2,8)*sim.checkProximitySensor(sim.getObjectHandle('PS09'),sim.handle_all,nil)
    +math.pow(2,9)*sim.checkProximitySensor(sim.getObjectHandle('PS10'),sim.handle_all,nil)
    +math.pow(2,10)*sim.checkProximitySensor(sim.getObjectHandle('PS11'),sim.handle_all,nil)
    +math.pow(2,11)*sim.checkProximitySensor(sim.getObjectHandle('PS12'),sim.handle_all,nil)
    +math.pow(2,12)*sim.checkProximitySensor(sim.getObjectHandle('PS13'),sim.handle_all,nil)
    +math.pow(2,13)*sim.checkProximitySensor(sim.getObjectHandle('PS14'),sim.handle_all,nil)
    +math.pow(2,14)*sim.checkProximitySensor(sim.getObjectHandle('PS15'),sim.handle_all,nil)
    +math.pow(2,15)*sim.checkProximitySensor(sim.getObjectHandle('PS16'),sim.handle_all,nil)
    +math.pow(2,16)*sim.checkProximitySensor(sim.getObjectHandle('PS17'),sim.handle_all,nil)
    +math.pow(2,17)*sim.checkProximitySensor(sim.getObjectHandle('PS18'),sim.handle_all,nil)
    +math.pow(2,18)*sim.checkProximitySensor(sim.getObjectHandle('PS19'),sim.handle_all,nil)
    +math.pow(2,19)*sim.checkProximitySensor(sim.getObjectHandle('PS20'),sim.handle_all,nil)
    +math.pow(2,20)*sim.checkProximitySensor(sim.getObjectHandle('PS21'),sim.handle_all,nil)
    +math.pow(2,21)*sim.checkProximitySensor(sim.getObjectHandle('PS22'),sim.handle_all,nil)
    +math.pow(2,22)*sim.checkProximitySensor(sim.getObjectHandle('PS23'),sim.handle_all,nil)
    +math.pow(2,23)*sim.checkProximitySensor(sim.getObjectHandle('PS24'),sim.handle_all,nil)

    -- Publication de l'entier
    simROS2.publish(pub,{data=sum})
end

if (sim_call_type==sim.syscb_cleanup) then
end
