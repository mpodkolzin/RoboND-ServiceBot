#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#include <string>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

move_base_msgs::MoveBaseGoal set_goal(double x, double y, double z, double w) 
{
	
	move_base_msgs::MoveBaseGoal goal;

	//frame parameters
  goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;

  goal.target_pose.pose.orientation.w = w;
  goal.target_pose.pose.orientation.z = z;

	return goal;
}

bool check_goal(MoveBaseClient& ac, std::string success_msg, std::string fail_msg)
{

	bool reached_goal = false;

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("%s", success_msg.c_str());
		reached_goal = true;
	}
  else
    ROS_INFO("%s", fail_msg.c_str());

	return reached_goal;
}

int main(int argc, char** argv)
{

  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal pickup_goal  = set_goal(3.529611650, 7.1201573094, 0.6797769607, 0.7334189005);
  move_base_msgs::MoveBaseGoal dropoff_goal = set_goal(-4.4274034, 6.412486618, 0.6913955444, 0.7008095470);

  // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending pickup goal");
  ac.sendGoal(pickup_goal);
  // Wait an infinite time for the results
  ac.waitForResult();
  
	// Check if te goal is reached
	bool check_pickup = check_goal(ac, "Pickup zone reached", "Unable to reach pickup zone");

	ROS_INFO("Picking up...");
	// Sleep for 5 seconds
	ros::Duration(5.0).sleep();

  // Send the goal position and orientation for the robot to reach
  ROS_INFO("Sending dropoff goal");
  ac.sendGoal(dropoff_goal);

  // Wait an infinite time for the results
  ac.waitForResult();
	// Check if the robot reached its goal
	bool check_dropoff = check_goal(ac, "Dropoff zone reached", "Unable to reach dropoff zone");

	// Infom user if both goals were accomplished or not
	if (check_pickup && check_dropoff)
		ROS_INFO("Robot successfully reached both zones!");
	else 
		ROS_INFO("Robot did not reach both zones");

	// Sleep for 5 seconds
	ros::Duration(5.0).sleep();

  return 0;
}
