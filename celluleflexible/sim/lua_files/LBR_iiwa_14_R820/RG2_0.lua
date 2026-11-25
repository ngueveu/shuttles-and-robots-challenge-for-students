function cmdPinceRobotCallback(msg)
    -- On vérifie que les moteurs sont bien initialisés
    if not motorHandle then return end

    local v = 0
    local data = msg.data

    -- Logique : 1 = Fermer, 0 = Ouvrir
    if (data == 1) then
        v = -motorVelocity -- Fermeture (vitesse négative)
    elseif (data == 0) then
        v = motorVelocity  -- Ouverture (vitesse positive)
    end
    
    -- Application de la commande
    sim.setJointForce(motorHandle, motorForce)
    sim.setJointTargetVelocity(motorHandle, v)
end

if (sim_call_type==sim.syscb_init) then
    -- 1. Initialisation des paramètres de la pince
    -- Vérifie bien que ce nom 'RG2_openCloseJoint' est exact dans ta scène pour CE robot
    local status, h = pcall(sim.getObjectHandle, 'RG2_openCloseJoint')
    
    if status then
        motorHandle = h
        motorVelocity = 0.10 -- m/s
        motorForce = 20 -- N
        
        -- État initial (Ouvert par défaut)
        sim.setJointForce(motorHandle, motorForce)
        sim.setJointTargetVelocity(motorHandle, motorVelocity)
    else
        print(" : Problème avec 'RG2_openCloseJoint'")
        print("Erreur: Message renvoyé par le système (h) : " .. h)
    end

    -- 2. Configuration ROS 2
    if simROS2 then
        -- Création du Subscriber
        -- Topic : /robot/cmdPinceRobot1 (Attention : Change le numéro 1 pour les autres robots !)
        -- Type  : std_msgs/msg/Int32
        simROS2.createSubscription('/robot/cmdPinceRobot1', 'std_msgs/msg/Int32', 'cmdPinceRobotCallback')
        --print("Pince RG2 : Abonné au topic /robot/cmdPinceRobot1")
    else
        print("Pince RG2 : Plugin ROS 2 non détecté (mais pas de popup d'erreur !)")
    end
end

if (sim_call_type==sim.syscb_actuation) then
    
end

if (sim_call_type==sim.syscb_sensing) then

end

if (sim_call_type==sim.syscb_cleanup) then

end
