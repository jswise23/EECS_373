#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <jsw107_p2/cmd_msg.h>
using namespace std; 

int main(int argc, char **argv) {
  ros::init(argc, argv, "sin_commander_client"); // name of this node will be "minimal_publisher2"
  ros::NodeHandle nh; // two lines to create a publisher object that can talk to ROS
  ros::ServiceClient client = nh.serviceClient<jsw107_p2::cmd_msg>("get_params");

  jsw107_p2::cmd_msg srv;
  double in_amplitude = 0.0;
  double in_frequecy = 0.0;

    // do work here in infinite loop (desired for this example), but terminate if detect ROS has faulted
    while (ros::ok()) 
    {
      cout<<"enter displacement amplitude: ";
      cin>>in_amplitude;
      cout<<"enter freq (in Hz): ";
      cin>>in_frequecy;

      srv.request.frequency = in_frequecy;
      srv.request.amplitude = in_amplitude;

      if(client.call(srv))
      {
        ROS_INFO("paramaters sent to sin commander");
      }
      else
      {
        ROS_ERROR("Failed to call service get_params");
        return 1;
      }
    }
  return 0;
}
