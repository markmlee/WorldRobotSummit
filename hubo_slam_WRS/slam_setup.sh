#!bin/bash
cd /home/rainbow/Desktop/hubo_slam_ws
. source
cd /home/rainbow/Desktop/hubo_slam_ws/src/microstrain_3DM_GX5
. setup_device.sh

cd ~/Desktop

roslaunch /home/rainbow/Desktop/hubo_slam_ws/src/hubo_slam/launch/pathplan_2d_marker.launch

