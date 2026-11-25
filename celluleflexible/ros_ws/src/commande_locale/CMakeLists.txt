cmake_minimum_required(VERSION 3.5)
project(commande_locale)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver les packages ROS2 requis
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(shuttles REQUIRED)
find_package(aiguillages REQUIRED)
find_package(OpenCV REQUIRED)

# Messages et services
find_package(rosidl_default_generators REQUIRED)

# Définition des messages et services
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/SrvAddProduct.srv"
  "srv/SrvFinInit.srv"
  "srv/SrvAddProductPushBack.srv"
  "msg/MsgAddProduct.msg"
  "msg/MsgStopControl.msg"
  "msg/MsgColor.msg"
  "msg/MsgSwitchControl.msg"
  "msg/MsgPinControl.msg"
  "msg/MsgErreur.msg"
  "msg/RobotJoints.msg"
  "msg/TacheFinieMsg.msg"
  "msg/DeplacerPieceMsg.msg"
  "msg/Actionneurs.msg"
  "msg/Capteurs.msg"
  "msg/MsgChoixMode.msg"
  DEPENDENCIES std_msgs
)

# Include directories
include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

# Déclaration des exécutables
add_executable(simulation src/commande_locale.cpp src/vrepController.cpp src/inOutController.cpp)
ament_target_dependencies(simulation
  rclcpp
  std_msgs
  sensor_msgs
  image_transport
  cv_bridge
  shuttles
  aiguillages
  OpenCV
)

add_executable(display_node src/display.cpp)
ament_target_dependencies(display_node
  rclcpp
  std_msgs
  sensor_msgs
  image_transport
  cv_bridge
  shuttles
  aiguillages
  OpenCV
)

add_executable(log_manager src/LogManager.cpp)
ament_target_dependencies(log_manager
  rclcpp
  std_msgs
  sensor_msgs
  image_transport
  ament_index_cpp
  cv_bridge
  shuttles
  aiguillages
  OpenCV
)

# Récupérer la cible typesupport C++
rosidl_get_typesupport_target(
  cpp_typesupport_target
  ${PROJECT_NAME}
  "rosidl_typesupport_cpp"
)

# Lier les messages à l'exécutable
target_link_libraries(simulation "${cpp_typesupport_target}")
target_link_libraries(display_node "${cpp_typesupport_target}")
target_link_libraries(log_manager "${cpp_typesupport_target}")


# Installer les exécutables
install(TARGETS
  simulation
  display_node
  log_manager
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_dependencies(rosidl_default_runtime std_msgs)

# ament package
ament_package()

