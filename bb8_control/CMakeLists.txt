#VERSION PROMO SE 2024-2025
cmake_minimum_required(VERSION 3.0.2)
project(bb8_control)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  rospy
  sensor_msgs
  std_msgs
  tf
  tf2
  tf2_ros
)
catkin_package(
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(controleur src/controleur.cpp)
target_link_libraries(controleur ${catkin_LIBRARIES}) 
 
#add_executable(localisation src/localisation.cpp)    
#target_link_libraries(localisation ${catkin_LIBRARIES}) 

add_executable(obstacles_detecteur src/obstacles_detecteur.cpp)    
target_link_libraries(obstacles_detecteur ${catkin_LIBRARIES}) 
#20.22
