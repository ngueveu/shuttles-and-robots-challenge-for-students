-- DO NOT WRITE CODE OUTSIDE OF THE if-then-end SECTIONS BELOW!! (unless the code is a function definition)

if (sim_call_type==sim.syscb_init) then
    -- Vérification de la présence de ROS 2
    if not simROS2 then
        sim.displayDialog('Error','ROS 2 plugin was not found.&&nSimulation will not run properly',sim.dlgstyle_ok,false,nil,{0.8,0,0,0,0,0},{0.5,0,0,1,1,1})
    else
        -- Récupération du handle du capteur
        visionSensorHandle=sim.getObjectHandle('Vision_sensor')
        
        -- Création du Publisher
        -- Le topic est /camera/image_raw
        --pubImage=simROS2.createPublisher('/camera/image_raw', 'sensor_msgs/msg/Image')
        
        -- Variables pour limiter la fréquence
        --lastImageTime = sim.getSimulationTime()
        --fps_limit = 5.0 -- Envoie seulement 5 images par seconde
    end
end

if (sim_call_type==sim.syscb_actuation) then

end

if (sim_call_type==sim.syscb_sensing) then
    if visionSensorHandle and pubImage then
    	--local currentTime = sim.getSimulationTime()
        
        -- On ne rentre ici que si le délai est passé !
        --if currentTime - lastImageTime >= (1.0 / fps_limit) then
            	--lastImageTime = currentTime -- On met à jour le chrono
            
		-- On récupère l'image sous forme de chaîne binaire (string)
		local data,w,h=sim.getVisionSensorCharImage(visionSensorHandle)
		
		if data then
		    local d={}
		    d['header']={
		        frame_id="vision_sensor_frame",
		        stamp=simROS2.getTime()
		    }
		    d['height']=h
		    d['width']=w
		    d['encoding']='rgb8'
		    d['is_bigendian']=0
		    d['step']=w*3
		    
		    -- On convertit la chaîne "data" en tableau de nombres pour ROS 2
		    d['data']=sim.unpackUInt8Table(data)
		    
		    --simROS2.publish(pubImage,d)
		end
	--end
    end
end

if (sim_call_type==sim.syscb_cleanup) then

end
