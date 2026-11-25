-- Fonction de sécurité pour trouver un objet
-- Utilisation de function plutot que des boucles if, conseillé pour réaliser des callbacks dans coppeliasim

function getHandleSafe(name)
    local status, handle = pcall(sim.getObjectHandle, name)
    if status then return handle else return -1 end
end

function sysCall_init()
    
    -- Verification du plugin ROS 2
    if not simROS2 then
        print("ERREUR: Plugin ROS 2 introuvable!")
        return
    end

    -- Recherche des moteurs (Handles)
    jointHandles = {}
    for i=1, 12 do
        -- Adaptez les noms selon votre scène (A01 ou Sode_aiguillage_A1...)
        local h = getHandleSafe("A"..string.format("%02d", i))
        if h == -1 then h = getHandleSafe("Sode_aiguillage_A"..i) end
        
        if h ~= -1 then
            jointHandles[i] = h
        else
            print("Moteur "..i.." introuvable.")
        end
    end

    -- CREATION PUBLISHER (TYPE INT32)
    -- ne publiait pas dans le code ROS1, ne change pas les fonctionnalités du systeme, réalisé surtout pour tester notre noeud cpp
    pubSensors = simROS2.createPublisher('/sim_ros_interface/aig', 'std_msgs/msg/Int32')

    -- Subscribers 
    subRight = simROS2.createSubscription('/sim_ros_interface/SwitchControllerRight', 'std_msgs/msg/Int32', 'right_cb')
    subLeft  = simROS2.createSubscription('/sim_ros_interface/SwitchControllerLeft',  'std_msgs/msg/Int32', 'left_cb')
    subLock  = simROS2.createSubscription('/sim_ros_interface/SwitchControllerLock',  'std_msgs/msg/Int32', 'lock_cb')
    -- initialisation des entiers
    cmd_right = 0
    cmd_left = 0
    cmd_lock = 0 
end
-- maj des entiers pour tourner et déverouiller
function right_cb(msg) cmd_right = msg.data end
function left_cb(msg)  cmd_left  = msg.data end
function lock_cb(msg)  cmd_lock  = msg.data end

function sysCall_actuation() -- remplace les boucles if en ROS1, qui gerait les mouvements aiguillages
    -- Gestion des mouvements
    if not simROS2 then return end

    local id_to_move_right = cmd_right
    local id_to_move_left = cmd_left
    
--------------------------------------------
------------ Right Switches ----------------
--------------------------------------------
    if id_to_move_right > 0 and id_to_move_right <= 12 then
        local h = jointHandles[id_to_move_right]
        if h then 
	    -- Vérification que l'ID reçu correspond à l'aiguillage à bouger
	    if cmd_lock == id_to_move_right then
                sim.setJointTargetPosition(h, 0) -- Position Droite (0)
                
            else
                --print('switch ' .. id_to_move_right .. ' is not unlocked !')
            end
        end
        cmd_right = 0 -- Reset de la commande après traitement
    end

-------------------------------------------
------------ Left Switches ----------------
-------------------------------------------

    if id_to_move_left > 0 and id_to_move_left <= 12 then
        local h = jointHandles[id_to_move_left]
        if h then 
	    -- Vérification que l'ID reçu correspond à l'aiguillage à bouger
	    if cmd_lock == id_to_move_left then
                -- Calcul de l'angle gauche
                local angle = -120 * math.pi / 180
                -- Inversion pour les moteurs pairs si nécessaire
                if (id_to_move_left % 2 == 0) then angle = -angle end 
                
                sim.setJointTargetPosition(h, angle) 
            else
                --print('switch ' .. id_to_move_left .. ' is not unlocked !')
            end
        end
        cmd_left = 0 -- Reset de la commande après traitement
    end
end

function sysCall_sensing()
   if not simROS2 or not pubSensors then return end

    -- CALCUL DE L'ENTIER UNIQUE (LOGIQUE ROS 1)
    local val = 0
    
    for i=1, 12 do
        local h = jointHandles[i]
        if h and h ~= -1 then
            local pos = sim.getJointPosition(h)
            
            -- Bit Droite (2*i - 2) -> Correspond à 2^(2*i-2)
            -- Si pos proche de 0 (Droite)
            if math.abs(pos) < 0.1 then
                val = val + 2^(2*i - 2)
            end
            
            -- Bit Gauche (2*i - 1) -> Correspond à 2^(2*i-1)
            -- Si pos > 0.5 (Gauche, ajustez seuil selon besoin)
            if math.abs(pos) > 0.1 then
                val = val + 2^(2*i - 1)
            end
        end
    end
    
    -- Envoi du message Int32
    local data = {}
    data['data'] = val -- Champ standard du message Int32
    simROS2.publish(pubSensors, data)
end

function sysCall_cleanup()
end

