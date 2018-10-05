//Used sine_commander from Part_2 of W. Newman's
//sample files for guidance.

#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <math.h>
using namespace std;
std_msgs::Float64 vel_cmd; //create a variable of type "Float64", 

int main(int argc, char **argv) {
    ros::init(argc, argv, "sin_commander"); // name of this node will be "minimal_publisher2"
    ros::NodeHandle nh; // two lines to create a publisher object that can talk to ROS
    ros::Publisher my_publisher = nh.advertise<std_msgs::Float64>("vel_cmd", 1);
    
   double v_cmd=0.0;
   double x_cmd=0.0;
   double amplitude=0.0;
   double frequecy,omega;
   cout<<"enter displacement amplitude: ";
   cin>>amplitude;
   cout<<"enter freq (in Hz): ";
   cin>>frequecy;
   omega = frequecy*2.0*M_PI;
   double phase=0;
   double dt = 0.01;
   ros::Rate sample_rate(1/dt); 
  
    
    // do work here in infinite loop (desired for this example), but terminate if detect ROS has faulted
    while (ros::ok()) 
    {
        phase+= omega*dt;
        if (phase>2.0*M_PI) phase-=2.0*M_PI;
        x_cmd = amplitude*sin(phase);
        v_cmd = omega*amplitude*cos(phase);
        vel_cmd.data = v_cmd;

        my_publisher.publish(vel_cmd); // publish the value--of type Float64-- 

        sample_rate.sleep(); 
    }
}

