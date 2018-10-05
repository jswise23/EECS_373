#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <jsw107_p3/cmdAction.h>
#include <actionlib/client/simple_action_client.h>
#include <ctime>
using namespace std; 

int main(int argc, char **argv) {
  ros::init(argc, argv, "sin_commander_action_client"); // name of this node will be "minimal_publisher2

  jsw107_p3::cmdGoal goal;
  actionlib::SimpleActionClient<jsw107_p3::cmdAction> action_client("cmd_action", true);


  ROS_INFO("connecting to server...");
  bool server_exists = action_client.waitForServer();

  if(!server_exists){
    ROS_WARN("could not connect to server");
    return 0;
  }

  double in_amplitude = 0.0;
  double in_frequecy = 0.0;
  int in_numCycles = 0;

  ROS_INFO("--connected to server--");

  // do work here in infinite loop (desired for this example), but terminate if detect ROS has faulted 
  cout<<"enter displacement amplitude: ";
  cin>>in_amplitude;
  cout<<"enter freq (in Hz): ";
  cin>>in_frequecy;
  cout<<"enter number of cycles: ";
  cin>>in_numCycles;

  goal.amplitude = in_amplitude;
  goal.frequency = in_frequecy;
  goal.numCycles = in_numCycles;

  action_client.sendGoal(goal);
  clock_t startTime = clock();
  ROS_INFO("timing service... ... ...");
  bool finished = action_client.waitForResult();
  clock_t stopTime = clock();
  double secsElapsed = difftime(stopTime, startTime) / 1000.0;
  cout<<"Time elapsed: " << secsElapsed << " seconds\n";
  return 0;
}
