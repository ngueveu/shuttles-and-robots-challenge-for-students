-- DO NOT WRITE CODE OUTSIDE OF THE if-then-end SECTIONS BELOW!!

if (sim_call_type==sim.syscb_init) then
    -- 1. On initialise à nil par sécurité
    pub = nil

    -- 2. Vérification du plugin
    if simROS2 then
        local status, createdPub = pcall(simROS2.createPublisher, '/StationSensor', 'std_msgs/msg/Int32')
        
        if status and createdPub then
            pub = createdPub
            -- sim.displayDialog('Info', 'StationSensor: Publisher créé.', sim.dlgstyle_message, false)
        else
            sim.displayDialog('Error', 'StationSensor: Erreur création publisher!', sim.dlgstyle_ok, false)
        end
    else
        --sim.displayDialog('Error','ROS 2 plugin not found.',sim.dlgstyle_ok,false)
    end
end

if (sim_call_type==sim.syscb_actuation) then
    
end

if (sim_call_type==sim.syscb_sensing) then
    -- SECURITÉ : Si le publisher n'existe pas, on arrête tout de suite.
    if not pub then return end

    -- Ton calcul original
    sum = math.pow(2,0)*sim.checkProximitySensor(sim.getObjectHandle('CPI01'),sim.handle_all,nil)
    +math.pow(2,1)*sim.checkProximitySensor(sim.getObjectHandle('CPI02'),sim.handle_all,nil)
    +math.pow(2,2)*sim.checkProximitySensor(sim.getObjectHandle('CPI03'),sim.handle_all,nil)
    +math.pow(2,3)*sim.checkProximitySensor(sim.getObjectHandle('CPI04'),sim.handle_all,nil)
    +math.pow(2,4)*sim.checkProximitySensor(sim.getObjectHandle('CPI05'),sim.handle_all,nil)
    +math.pow(2,5)*sim.checkProximitySensor(sim.getObjectHandle('CPI06'),sim.handle_all,nil)
    +math.pow(2,6)*sim.checkProximitySensor(sim.getObjectHandle('CPI07'),sim.handle_all,nil)
    +math.pow(2,7)*sim.checkProximitySensor(sim.getObjectHandle('CPI08'),sim.handle_all,nil)

    -- Envoi sécurisé
    simROS2.publish(pub,{data=sum})
end

if (sim_call_type==sim.syscb_cleanup) then

end
