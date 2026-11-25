function sysCall_init()
    -- do some initialization here
    this=sim.getObjectHandle(sim.handle_self)
    myName = sim.getObjectName(this)
    handle=sim.getObjectHandle(myName)
    
    color={80/255,80/255,80/255}
    sim.setShapeColor(handle, nil, 0, color)
    sim.setShapeColor(handle, nil, sim.colorcomponent_transparency, {0})

    sub=simROS2.createSubscription('/commande/Simulation/TransportBras2','std_msgs/msg/Bool','transportBrasCallback',100)
end

function transportBrasCallback(msg)
if msg.data == false then
    sim.setShapeColor(handle, nil, sim.colorcomponent_transparency, {0})
else
    sim.setShapeColor(handle, nil, sim.colorcomponent_transparency, {1})
end
end

function sysCall_actuation()
    -- put your actuation code here
end

function sysCall_sensing()
    -- put your sensing code here
end

function sysCall_cleanup()
    -- do some clean-up here
end

-- See the user manual or the available code snippets for additional callback functions and details

