function sysCall_init()
    -- Initialisation des handles
    self = sim.getObjectHandle(sim.handle_self)
    myName = sim.getObjectName(self)
    lettre = myName:sub(8,8)
    
    MotorFrontLeftHandle = sim.getObjectHandle('FrontLeft'..lettre)
    MotorFrontRightHandle = sim.getObjectHandle('FrontRight'..lettre)
    InductiveSensor = sim.getObjectHandle('InductiveSensor'..lettre)
    UltrasonicSensor = sim.getObjectHandle('UltrasonicSensor'..lettre)
    FrontSensor = sim.getObjectHandle('Front_sensor'..lettre)

    -- Handles des produits
    produitHandles = {}
    for i=1,6 do
        produitHandles[i] = sim.callScriptFunction('getHandleInside', sim.scripttype_mainscript, 'Produit'..i..'_'..lettre)
    end

    -- Couleurs (Table complète)
    colors = {
        -- Couleur de base
        base = {200/255, 200/255, 200/255},       -- 0

        -- Spectre Rouge (1)
        [10] = {255/255, 153/255, 153/255},       -- red_0
        [11] = {255/255, 102/255, 102/255},       -- red_1
        [12] = {255/255,   0/255,   0/255},       -- red_2
        [13] = {255/255,   0/255,   0/255},       -- red_3
        [14] = {230/255,   0/255,   0/255},       -- red_4

        -- Spectre Bleu (2)
        [20] = {102/255, 178/255, 255/255},       -- blue_0
        [21] = { 51/255, 153/255, 255/255},       -- blue_1
        [22] = {  0/255, 102/255, 255/255},       -- blue_2
        [23] = {  0/255, 102/255, 255/255},       -- blue_3
        [24] = {  0/255,   0/255, 255/255},       -- blue_4

        -- Spectre Vert (3)
        [30] = {153/255, 255/255, 153/255},       -- green_0
        [31] = {102/255, 255/255, 102/255},       -- green_1
        [32] = {  0/255, 204/255,   0/255},       -- green_2
        [33] = {  0/255, 204/255,   0/255},       -- green_3
        [34] = {  0/255, 150/255,   0/255},       -- green_4

        -- Spectre Orange (4)
        [40] = {255/255, 217/255, 172/255},       -- orange_0
        [41] = {255/255, 178/255, 102/255},       -- orange_1
        [42] = {255/255, 128/255,   0/255},       -- orange_2
        [43] = {255/255, 128/255,   0/255},       -- orange_3
        [44] = {255/255, 100/255,   0/255},       -- orange_4

        -- Spectre Rose (5)
        [50] = {255/255, 153/255, 204/255},       -- pink_0
        [51] = {255/255, 102/255, 178/255},       -- pink_1
        [52] = {255/255,   0/255, 127/255},       -- pink_2
        [53] = {255/255,   0/255, 127/255},       -- pink_3
        [54] = {204/255,   0/255, 102/255},       -- pink_4

        -- Spectre Jaune (6)
        [60] = {190/255, 180/255,  80/255},       -- yellow_0
        [61] = {210/255, 200/255,  60/255},       -- yellow_1
        [62] = {250/255, 240/255,   0/255},       -- yellow_2
        [63] = {250/255, 240/255,   0/255},       -- yellow_3
        [64] = {255/255, 255/255,   0/255},       -- yellow_4

        -- Spectre Blanc (7)
        [72] = {240/255, 240/255, 240/255},       -- white_2
        [73] = {240/255, 240/255, 240/255},       -- white_3
        [74] = {255/255, 255/255, 255/255},       -- white_4

        -- Spectre Noir (8)
        [82] = { 10/255,  10/255,  10/255},       -- black_2
        [83] = { 10/255,  10/255,  10/255},       -- black_3
        [84] = {  0/255,   0/255,   0/255}        -- black_4
    }
    
end


function sysCall_sensing()
    -- Mise à jour des couleurs des produits
    for i=1,6 do
        -- Les index des signaux vont de 0 à 5 (ex: ShuttleD#0_color, ShuttleD#1_color...)
        local signalName = 'Shuttle' .. lettre .. '#' .. (i-1) .. '_color'
        local colorCode = sim.getInt32Signal(signalName)
        
        -- Si le signal est vide (nil), ça veut dire qu'il n'y a pas de pièce (couleur 0)
        if colorCode == nil then 
            colorCode = 0 
        end
        
        updateObjectColor(produitHandles[i], colorCode)
    end
end

function updateObjectColor(object, code)
    local color = colors[code] or colors.base
    local trans = 1.0

    -- Logique de transparence fidèle au script original
    if code == 0 then
        trans = 0.0 -- 0 = Totalement transparent (invisible) par défaut
    elseif code == 72 then
        trans = 0.7 -- Exception pour le blanc 2
    elseif code % 10 == 2 then
        trans = 0.5 -- 0.5 = Semi-transparent pour les codes finissant par 2 (ex: 12, 22...)
    else
        trans = 1.0 -- 1.0 = Opaque pour les autres couleurs (10, 11, 20, etc.)
    end

    sim.setShapeColor(object, nil, sim.colorcomponent_ambient_diffuse, color)
    sim.setShapeColor(object, nil, sim.colorcomponent_transparency, {trans})
end

function sysCall_actuation()
    local slow = sim.checkProximitySensor(InductiveSensor, sim.handle_all)
    local stop = sim.checkProximitySensor(UltrasonicSensor, sim.handle_all)
    local front = sim.checkProximitySensor(FrontSensor, sim.handle_all)

    local leftVel, rightVel = 0, 0

    if (front == 1 or stop == 1) then
        leftVel, rightVel = 0, 0
    elseif (slow == 1) then
        leftVel, rightVel = -10, 10
    else
        leftVel, rightVel = -20, 20
    end

    sim.setJointTargetVelocity(MotorFrontLeftHandle, leftVel)
    sim.setJointTargetVelocity(MotorFrontRightHandle, rightVel)
end

function sysCall_cleanup()

end
