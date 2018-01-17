cmake_minimum_required(VERSION 2.8.3)
project(step_control)

find_package(catkin REQUIRED COMPONENTS
roscpp
rospy
std_msgs
geometry_msgs
sensor_msgs
step_msgs
tf
)


#set(CMAKE_INCLUDE_CURRENT_DIR ON)
link_directories(/usr/local/lib)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES pionier
#  CATKIN_DEPENDS message_runtime
#  DEPENDS system_lib
)
include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    /usr/local/include/
)

#add_library(acc_test acc_test.cpp)
#target_link_libraries(acc_test ${catkin_LIBRARIES})
add_library(step_control step_control.cpp)
target_link_libraries(step_control ${catkin_LIBRARIES})
add_executable(step_control_node step_control_node.cpp)
target_link_libraries(step_control_node ${catkin_LIBRARIES} step_control)
