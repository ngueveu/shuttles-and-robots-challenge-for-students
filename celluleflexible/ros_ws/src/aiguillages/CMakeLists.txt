cmake_minimum_required(VERSION 3.8)
project(aiguillages)

# On passe au C++ moderne exigé par ROS 2,on laisse le choix au futurs dév de monter en version (C++ V17) sans avoir a changer le cmakelist, mais on met par defaut v14

# Pour Jazzy, on passe en C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

#1. Reherche des librairies
find_package(ament_cmake REQUIRED) #amenct est le remplacant de catkin, c'est lui qui 				donne les outils pour construire un paquet ROS2

find_package(rclcpp REQUIRED) # remplace roscpp,biblio C++
find_package(std_msgs REQUIRED) 
find_package(rosidl_default_generators REQUIRED)#remplace message_generation
find_package(OpenCV REQUIRED) # On garde OpenCV pour le traitement d'image, remplace la ligne inclue_directories...

# 2. On regroupe tous les messages d'un coup, cette commande remplace à la fois add_message et generate_message
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Capteurs.msg"
  "msg/Actionneurs.msg"
  "msg/ExchangeSh.msg"
  "msg/MsgSensorState.msg"
  DEPENDENCIES std_msgs
)

# 3. L'exécutable (on garde ton nom run_Aiguillage)
add_executable(run_Aiguillage 
  src/main_aiguillage.cpp 
  src/Aiguillage.cpp
)

# 4. Inclusions et Dépendances
target_include_directories(run_Aiguillage PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  ${OpenCV_INCLUDE_DIRS}
)

# On lie les dépendances ROS 2
ament_target_dependencies(run_Aiguillage
  rclcpp
  std_msgs
)

# On lie OpenCV et les messages internes du paquet
target_link_libraries(run_Aiguillage ${OpenCV_LIBRARIES})

rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(run_Aiguillage "${cpp_typesupport_target}")

# 5. L'étape "Installation" (Crucial pour que ros2 run fonctionne)
install(TARGETS run_Aiguillage
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
