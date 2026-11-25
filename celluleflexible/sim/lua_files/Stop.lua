-- DO NOT WRITE CODE OUTSIDE OF THE if-then-end SECTIONS BELOW!!

function stopCallback(msg)
    myintstop=msg.data
end

function goOnCallback(msg)
    myintgoon=msg.data
end

if (sim_call_type==sim.syscb_init) then 
    -- 1. Initialisation des variables
    pluginNotFound = true
    myint=25
    myintbefore=25
    
    -- 2. NOUVELLE MÉTHODE 
    if simROS2 then
        pluginNotFound = false
    end

    if (pluginNotFound) then
        -- sim.displayDialog('Error','ROS 2 plugin was not found.',sim.dlgstyle_ok,false)
    else
        -- Initialisation des handles
        S1=sim.getObjectHandle('ST01')
        S2=sim.getObjectHandle('ST02')
        S3=sim.getObjectHandle('ST03')
        S4=sim.getObjectHandle('ST04')
        S5=sim.getObjectHandle('ST05')
        S6=sim.getObjectHandle('ST06')
        S7=sim.getObjectHandle('ST07')
        S8=sim.getObjectHandle('ST08')
        S9=sim.getObjectHandle('ST09')
        S10=sim.getObjectHandle('ST10')
        S11=sim.getObjectHandle('ST11')
        S12=sim.getObjectHandle('ST12')
        S13=sim.getObjectHandle('ST13')
        S14=sim.getObjectHandle('ST14')
        S15=sim.getObjectHandle('ST15')
        S16=sim.getObjectHandle('ST16')
        S17=sim.getObjectHandle('ST17')
        S18=sim.getObjectHandle('ST18')
        S19=sim.getObjectHandle('ST19')
        S20=sim.getObjectHandle('ST20')
        S21=sim.getObjectHandle('ST21')
        S22=sim.getObjectHandle('ST22')
        S23=sim.getObjectHandle('ST23')
        S24=sim.getObjectHandle('ST24')
        
        -- Creation sécurisée des abonnements
        pcall(simROS2.createSubscription, '/sim_ros_interface/StopController','std_msgs/msg/Int32','stopCallback')
        pcall(simROS2.createSubscription, '/sim_ros_interface/GoController','std_msgs/msg/Int32','goOnCallback')
        
        myintstop=nil
        myintgoon=nil
    end
end 

if (sim_call_type==sim.syscb_cleanup) then 
 
end 

if (sim_call_type==sim.syscb_sensing) then 

end 

if (sim_call_type==sim.syscb_actuation) then
    -- Si pas de plugin, on arrête tout de suite
    if pluginNotFound then return end

    -- Stop Control
    if (myintgoon~=nil) then
        if (sim.boolAnd32(myintgoon,1)==1) then sim.setJointTargetPosition(S1,0) end
        if (sim.boolAnd32(myintgoon,2)==2) then sim.setJointTargetPosition(S2,0) end
        if (sim.boolAnd32(myintgoon,4)==4) then sim.setJointTargetPosition(S3,0) end
        if (sim.boolAnd32(myintgoon,8)==8) then sim.setJointTargetPosition(S4,0) end
        if (sim.boolAnd32(myintgoon,16)==16) then sim.setJointTargetPosition(S5,0) end
        if (sim.boolAnd32(myintgoon,32)==32) then sim.setJointTargetPosition(S6,0) end
        if (sim.boolAnd32(myintgoon,64)==64) then sim.setJointTargetPosition(S7,0) end
        if (sim.boolAnd32(myintgoon,128)==128) then sim.setJointTargetPosition(S8,0) end
        if (sim.boolAnd32(myintgoon,256)==256) then sim.setJointTargetPosition(S9,0) end
        if (sim.boolAnd32(myintgoon,512)==512) then sim.setJointTargetPosition(S10,0) end
        if (sim.boolAnd32(myintgoon,1024)==1024) then sim.setJointTargetPosition(S11,0) end
        if (sim.boolAnd32(myintgoon,2048)==2048) then sim.setJointTargetPosition(S12,0) end
        if (sim.boolAnd32(myintgoon,4096)==4096) then sim.setJointTargetPosition(S13,0) end
        if (sim.boolAnd32(myintgoon,8192)==8192) then sim.setJointTargetPosition(S14,0) end
        if (sim.boolAnd32(myintgoon,16384)==16384) then sim.setJointTargetPosition(S15,0) end
        if (sim.boolAnd32(myintgoon,32768)==32768) then sim.setJointTargetPosition(S16,0) end
        if (sim.boolAnd32(myintgoon,65536)==65536) then sim.setJointTargetPosition(S17,0) end
        if (sim.boolAnd32(myintgoon,131072)==131072) then sim.setJointTargetPosition(S18,0) end
        if (sim.boolAnd32(myintgoon,262144)==262144) then sim.setJointTargetPosition(S19,0) end
        if (sim.boolAnd32(myintgoon,524288)==524288) then sim.setJointTargetPosition(S20,0) end
        if (sim.boolAnd32(myintgoon,1048576)==1048576) then sim.setJointTargetPosition(S21,0) end
        if (sim.boolAnd32(myintgoon,2097152)==2097152) then sim.setJointTargetPosition(S22,0) end
        if (sim.boolAnd32(myintgoon,4194304)==4194304) then sim.setJointTargetPosition(S23,0) end
        if (sim.boolAnd32(myintgoon,8388608)==8388608) then sim.setJointTargetPosition(S24,0) end
        myintgoon=nil
    end

    -- GoOn Control
    if (myintstop~=nil) then
        if (sim.boolAnd32(myintstop,1)==1) then sim.setJointTargetPosition(S1,-0.015) end
        if (sim.boolAnd32(myintstop,2)==2) then sim.setJointTargetPosition(S2,-0.015) end
        if (sim.boolAnd32(myintstop,4)==4) then sim.setJointTargetPosition(S3,-0.015) end
        if (sim.boolAnd32(myintstop,8)==8) then sim.setJointTargetPosition(S4,-0.015) end
        if (sim.boolAnd32(myintstop,16)==16) then sim.setJointTargetPosition(S5,-0.015) end
        if (sim.boolAnd32(myintstop,32)==32) then sim.setJointTargetPosition(S6,-0.015) end
        if (sim.boolAnd32(myintstop,64)==64) then sim.setJointTargetPosition(S7,-0.015) end
        if (sim.boolAnd32(myintstop,128)==128) then sim.setJointTargetPosition(S8,-0.015) end
        if (sim.boolAnd32(myintstop,256)==256) then sim.setJointTargetPosition(S9,-0.015) end
        if (sim.boolAnd32(myintstop,512)==512) then sim.setJointTargetPosition(S10,-0.015) end
        if (sim.boolAnd32(myintstop,1024)==1024) then sim.setJointTargetPosition(S11,-0.015) end
        if (sim.boolAnd32(myintstop,2048)==2048) then sim.setJointTargetPosition(S12,-0.015) end
        if (sim.boolAnd32(myintstop,4096)==4096) then sim.setJointTargetPosition(S13,-0.015) end
        if (sim.boolAnd32(myintstop,8192)==8192) then sim.setJointTargetPosition(S14,-0.015) end
        if (sim.boolAnd32(myintstop,16384)==16384) then sim.setJointTargetPosition(S15,-0.015) end
        if (sim.boolAnd32(myintstop,32768)==32768) then sim.setJointTargetPosition(S16,-0.015) end
        if (sim.boolAnd32(myintstop,65536)==65536) then sim.setJointTargetPosition(S17,-0.015) end
        if (sim.boolAnd32(myintstop,131072)==131072) then sim.setJointTargetPosition(S18,-0.015) end
        if (sim.boolAnd32(myintstop,262144)==262144) then sim.setJointTargetPosition(S19,-0.015) end
        if (sim.boolAnd32(myintstop,524288)==524288) then sim.setJointTargetPosition(S20,-0.015) end
        if (sim.boolAnd32(myintstop,1048576)==1048576) then sim.setJointTargetPosition(S21,-0.015) end
        if (sim.boolAnd32(myintstop,2097152)==2097152) then sim.setJointTargetPosition(S22,-0.015) end
        if (sim.boolAnd32(myintstop,4194304)==4194304) then sim.setJointTargetPosition(S23,-0.015) end
        if (sim.boolAnd32(myintstop,8388608)==8388608) then sim.setJointTargetPosition(S24,-0.015) end
        myintstop=nil
    end
end
