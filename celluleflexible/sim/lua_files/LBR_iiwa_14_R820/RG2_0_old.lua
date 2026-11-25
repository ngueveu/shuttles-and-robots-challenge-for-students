if (sim_call_type==sim.syscb_init) then
    
    
    
    local moduleName=0
    local moduleVersion=0
    local index=0
    local pluginNotFound=true

    while moduleName do
        moduleName,moduleVersion=sim.getModuleName(index)
        if (moduleName=='ROSInterface') then
            pluginNotFound=false
        end
        index=index+1
    end

    if (pluginNotFound) then

        -- Display an error message if the plugin was not found:
        sim.displayDialog('Error','ROS plugin was not found.&&nSimulation will not run properly',sim.dlgstyle_ok,false,nil,{0.8,0,0,0,0,0},{0.5,0,0,1,1,1})
    else
        
        -- Retrive the handle of the vision sensor we wish to stream:
        motorHandle=sim.getObjectHandle('RG2_openCloseJoint')
        motorVelocity=0.10 -- m/s
        motorForce=20 -- N

        sim.setJointForce(motorHandle,motorForce)
        sim.setJointTargetVelocity(motorHandle,motorVelocity)

        -- Now enable a topic subscription:
        sub=simROS.subscribe('/robot/cmdPinceRobot1','std_msgs/Int32','cmdPinceRobotCallback',100)
        
    end

end

function cmdPinceRobotCallback(msg)
    v=0
    data=msg.data

    if(data~=nil) then
        if(data==1) then
            v=-motorVelocity
        end
        if (data==0) then
            v=motorVelocity
        end
        
    sim.setJointForce(motorHandle,motorForce)
    sim.setJointTargetVelocity(motorHandle,v)
    end
end

if (sim_call_type==sim.syscb_actuation) then
    
end
