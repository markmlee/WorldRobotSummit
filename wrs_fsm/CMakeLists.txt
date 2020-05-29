cmake_minimum_required(VERSION 2.8.3)
project(wrs_fsm)

add_compile_options(-std=c++14)

## Find catkin dependencies
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  tf
  nav_msgs
  actionlib
  actionlib_msgs
  geometry_msgs
  tf2_ros
  tf2_eigen
  tf2_geometry_msgs
)


## Find Boost
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(Boost REQUIRED COMPONENTS system)

## Add actions
add_action_files(DIRECTORY action FILES navi.action fsm_motion.action detect.action motion_direction.action manipulation.action)

## Add topics
add_message_files(
  FILES
  tf_broadcast.msg
)

## Generate messages
generate_messages(DEPENDENCIES std_msgs actionlib_msgs)

# Define catkin exports
catkin_package(
  CATKIN_DEPENDS actionlib
  DEPENDS Boost
)
#catkin_package(
##  INCLUDE_DIRS include
# CATKIN_DEPENDS actionlib actionlib_msgs message_generation roscpp std_msgs
# DEPENDS Boost
#)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

## Setup include dirs
include_directories(include ${catkin_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})



###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need



## mark examples for installation
#install(DIRECTORY examples
#  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
#  USE_SOURCE_PERMISSIONS
#  FILES_MATCHING PATTERN "*.py"
#  PATTERN ".svn" EXCLUDE
#)



#node for navi_dummy
add_executable(navi_dummy src/navi_dummy.cpp)
add_dependencies(navi_dummy ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(navi_dummy ${catkin_LIBRARIES})


#node for motion_client
#add_executable(motion_client src/motion_client.cpp)
#add_dependencies(motion_client ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#target_link_libraries(motion_client ${catkin_LIBRARIES})


#node for detection_dummy
add_executable(detection_dummy src/detection_dummy.cpp)
add_dependencies(detection_dummy ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(detection_dummy ${catkin_LIBRARIES})


#node for motion_director
add_executable(motion_director src/motion_director.cpp)
add_dependencies(motion_director ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(motion_director ${catkin_LIBRARIES})


#node for motion_bridge_dummy
add_executable(motion_bridge_dummy src/motion_bridge_dummy.cpp)
add_dependencies(motion_bridge_dummy ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(motion_bridge_dummy ${catkin_LIBRARIES})

#node for object_pose_test
add_executable(object_pose_test src/object_pose_test.cpp)
add_dependencies(object_pose_test ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(object_pose_test ${catkin_LIBRARIES})


