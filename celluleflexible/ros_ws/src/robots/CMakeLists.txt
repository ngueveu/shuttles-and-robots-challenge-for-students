cmake_minimum_required(VERSION 3.5)
project(robots)

## On trouve les packages dont on a besoin
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(commande_locale REQUIRED)
find_package(shuttles REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Définition des messages et services
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/MoveRobot.msg"
  "msg/FaireTacheMsg.msg"
  "msg/MsgNumRobot.msg"
  "msg/EvacuerMsg.msg"
  DEPENDENCIES std_msgs
)

# Include directories
include_directories(
  include
)

# Déclaration des exécutables
add_executable(robot src/main_robot.cpp src/Robot.cpp src/Poste.cpp)
ament_target_dependencies(robot
  rclcpp
  std_msgs
  sensor_msgs
  commande_locale
  shuttles
  rosidl_default_runtime
)

# Récupérer la cible typesupport C++
rosidl_get_typesupport_target(
  cpp_typesupport_target
  ${PROJECT_NAME}
  "rosidl_typesupport_cpp"
)

# Lier les messages à l'exécutable
target_link_libraries(robot
  "${cpp_typesupport_target}"
)

# Installer les exécutables
install(TARGETS
  robot
  DESTINATION lib/${PROJECT_NAME}
)

# Installer tous les fichiers launch Python
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.launch.py"
)

ament_export_dependencies(rosidl_default_runtime std_msgs)

# ament package
ament_package()

