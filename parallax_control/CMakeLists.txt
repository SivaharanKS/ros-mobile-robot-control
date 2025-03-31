cmake_minimum_required(VERSION 3.0.2)
project(parallax_control)

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

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)
catkin_package(
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${GPIOD_INCLUDE_DIRS} include
)

add_executable(controleur src/controleur.cpp)
target_link_libraries(controleur ${catkin_LIBRARIES}) 
 
add_executable(obstacles_detecteur src/obstacles_detecteur.cpp)    
target_include_directories(obstacles_detecteur PUBLIC ${catkin_INCLUDE_DIRS})
target_link_libraries(obstacles_detecteur ${catkin_LIBRARIES}
 ${GPIOD_LIBRARIES}) 

add_executable(configurateur src/configurateur.cpp)
target_link_libraries(configurateur ${catkin_LIBRARIES})
add_dependencies(configurateur ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})


add_executable(afficheur src/afficheur.cpp)
target_link_libraries(afficheur ${catkin_LIBRARIES})

 
#20.22
