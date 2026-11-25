cmake_minimum_required(VERSION 3.8)
project(commande)



# Packages ROS2
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(robots REQUIRED)
find_package(commande_locale REQUIRED)
find_package(shuttles REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)


# Exécutable
add_executable(run
  src/main_commande.cpp
  src/commande.cpp
  src/capteurs.cpp
  src/actionneurs.cpp
  src/communication_baxter.cpp
  src/RobotsInterface.cpp
  src/AigsInterface.cpp
)

# Dépendances ROS2
ament_target_dependencies(run
  rclcpp
  std_msgs
  robots
  commande_locale
  shuttles
)

# Link OpenCV
target_link_libraries(run
  ${OpenCV_LIBRARIES}
)

# Installation
install(TARGETS
  run
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
