# Utility to keep sudo active
sudo echo
while true; do sudo -n true; sleep 60; kill -0 "$$" || exit; done 2>/dev/null &

# Install wstool and rosdep
sudo apt-get update
sudo apt-get install -y python-wstool python-rosdep ninja-build

# Setup src files
wstool init src
wstool merge -t src https://raw.githubusercontent.com/kentsommer/hubo_slam_ws/master/ws.rosinstall
wstool update -t src

# Install protobuf 3
src/cartographer/scripts/install_proto3.sh

# Install .deb dependencies (sudo rosdep init will print an error
#  if it was already run, this can safely be ignored)
sudo rosdep init
rosdep update
rosdep install --from-paths src --ignore-src --rosdistro=${ROS_DISTRO} -y

catkin_make_isolated --use-ninja
source devel_isolated/setup.bash


