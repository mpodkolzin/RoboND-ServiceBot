#!/bin/sh

xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/workspace/RoboND-ServiceBot/catkin_ws/src/World/MyWorld.world" &
sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=//home/workspace/RoboND-ServiceBot/catkin_ws/src/World/MyWorld.yaml 3d_sensor:=kinect" &
sleep 5
#xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" &
xterm -e "roslaunch add_markers view_navigation.launch" &
sleep 5
xterm -e " rosrun add_markers add_markers_node" &
sleep 5
xterm -e " rosrun pick_objects pick_objects_node"
