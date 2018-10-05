#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <jsw107_p2/cmd_msg.h>
#include <math.h>
using namespace std;
std_msgs::Float64 vel_cmd; //create a variable of type "Float64", 



bool callback(jsw107_p2::cmd_msg::Request& request, jsw107_p2::cmd_msg::Response& response)
{
	ros::NodeHandle nh2;

	ros::Publisher my_publisher = nh2.advertise<std_msgs::Float64>("vel_cmd", 1);
    

    double v_cmd=0.0;
    double x_cmd=0.0;
    double omega;
    double phase=0;
    double dt = 0.01;
    ros::Rate sample_rate(1/dt); 
	double amplitude;
	double frequency;

	ROS_INFO("receiving parameters from client");

	frequency = request.frequency;
	amplitude = request.amplitude;
    omega = frequency*2.0*M_PI;

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

	return true;
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "sin_commander_service"); // name of this node will be "minimal_publisher2"
    ros::NodeHandle nh; // two lines to create a publisher object that can talk to ROS
    
	ros::ServiceServer service = nh.advertiseService("get_params", callback);
	ROS_INFO("Waiting to receive parameters...");

    ros::spin();

    return 0;

}

