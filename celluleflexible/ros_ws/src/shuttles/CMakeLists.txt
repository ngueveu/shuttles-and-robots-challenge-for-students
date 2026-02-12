cmake_minimum_required(VERSION 3.8)
project(shuttles)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 1. Dépendances
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(aiguillages REQUIRED)

# 2. Génération des Messages et Services
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/MsgShuttleChange.msg"
  "msg/MsgShuttleCreate.msg"
  "msg/MsgErreur.msg"
  "srv/ShuttleId.srv"
  DEPENDENCIES std_msgs aiguillages
)

# 3. Exécutable principal
add_executable(shuttle_manager 
  src/main_ShuttleManager.cpp 
  src/capteurs.cpp 
  src/FileAttente.cpp
)

# CORRECTION ICI : On inclut "src" car tes .h sont dedans
target_include_directories(shuttle_manager PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)

# Liaison avec les librairies ROS 2
ament_target_dependencies(shuttle_manager
  rclcpp
  std_msgs
  aiguillages
)

rosidl_target_interfaces(shuttle_manager
  ${PROJECT_NAME} "rosidl_typesupport_cpp"
)

# 4. Installation
install(TARGETS shuttle_manager
  DESTINATION lib/${PROJECT_NAME}
)

# CORRECTION ICI : On a supprimé la ligne "install(DIRECTORY include/ ...)" qui plantait

ament_package()
