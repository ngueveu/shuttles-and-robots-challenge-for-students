cmake_minimum_required(VERSION 3.8)
project(launcher)

#######################################
## Find ament macros and dependencies #
#######################################

# ament_cmake remplace catkin
find_package(ament_cmake REQUIRED)

# rclcpp remplace roscpp pour ROS 2
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

################################################
## Declare ROS messages, services and actions ##
################################################
## En ROS 2, cela se gère avec rosidl_default_generators
# find_package(rosidl_default_generators REQUIRED)
# rosidl_generate_interfaces(${PROJECT_NAME}
#   "msg/Message1.msg"
#   "srv/Service1.srv"
#   "action/Action1.action"
#   DEPENDENCIES std_msgs
# )

###########
## Build ##
###########

## Déclarer les dossiers d'en-tête (include)
# include_directories(include)

## Déclarer une bibliothèque C++
# add_library(${PROJECT_NAME}_lib src/launcher.cpp)
# target_include_directories(${PROJECT_NAME}_lib PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)
# ament_target_dependencies(${PROJECT_NAME}_lib rclcpp std_msgs)

## Déclarer un exécutable C++ (Nœud ROS 2)
# add_executable(launcher_node src/launcher_node.cpp)

## Lier l'exécutable aux dépendances ROS 2 (remplace target_link_libraries avec catkin)
# ament_target_dependencies(launcher_node rclcpp std_msgs)

#############
## Install 
#############


## Installer les exécutables dans le dossier lib/ du package
# install(TARGETS launcher_node
#   DESTINATION lib/${PROJECT_NAME}
# )

## Installer les bibliothèques
# install(TARGETS ${PROJECT_NAME}_lib
#   ARCHIVE DESTINATION lib
#   LIBRARY DESTINATION lib
#   RUNTIME DESTINATION bin
# )

## Installer les fichiers d'en-tête (headers)
# install(DIRECTORY include/
#   DESTINATION include
# )

## Installer les dossiers launch
## C'EST ICI QUE LA MODIFICATION A ÉTÉ FAITE POUR ROS 2
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

#############
## Testing ##
#############

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
 
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

###################################
## ament specific configuration ###
###################################
## ament_package() génère toute la configuration finale (remplace catkin_package)
ament_package()
