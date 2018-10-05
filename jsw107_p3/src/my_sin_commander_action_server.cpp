#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <actionlib/server/simple_action_server.h>
#include <jsw107_p3/cmdAction.h>
#include <math.h>
using namespace std;
std_msgs::Float64 vel_cmd; //create a variable of type "Float64", 

class SinCommanderActionServer
{
private:
    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<jsw107_p3::cmdAction> as_;

    jsw107_p3::cmdGoal goal_;
    jsw107_p3::cmdResult result_;
    jsw107_p3::cmdFeedback feedback_;

public:
    SinCommanderActionServer();

    ~SinCommanderActionServer(void){
    }
    void doWork(const actionlib::SimpleActionServer<jsw107_p3::cmdAction>::GoalConstPtr& goal);


};

SinCommanderActionServer::SinCommanderActionServer() : as_(nh_, "cmd_action", boost::bind(&SinCommanderActionServer::doWork, this, _1), false)
{
    as_.start();
}


void SinCommanderActionServer::doWork(const actionlib::SimpleActionServer<jsw107_p3::cmdAction>::GoalConstPtr& goal)
{
	ros::Publisher my_publisher = nh_.advertise<std_msgs::Float64>("vel_cmd", 1);
    
    int cyclesRun = 0;
    double v_cmd=0.0;
    double omega;
    double phase=0;
    double dt = 0.01;
    ros::Rate sample_rate(1/dt); 
	double amplitude;
	double frequency;

	ROS_INFO("receiving parameters from client");

	frequency = goal->frequency;
	amplitude = goal->amplitude;
    omega = frequency*2.0*M_PI;

    // do work here in infinite loop (desired for this example), but terminate if detect ROS has faulted
    while (ros::ok() && cyclesRun <= goal->numCycles) 
    {
        phase+= omega*dt;
        if (phase>2.0*M_PI){
            phase-=2.0*M_PI;
            cyclesRun++;
        } 
        
        v_cmd = omega*amplitude*cos(phase);
        vel_cmd.data = v_cmd;

        my_publisher.publish(vel_cmd); // publish the value--of type Float64-- 

        sample_rate.sleep(); 
    }

    v_cmd = 0.0;
    vel_cmd.data = 0.0;
    my_publisher.publish(vel_cmd);

    if(cyclesRun >= goal->numCycles){
        result_.message = "Goal cylces met!";
        ROS_INFO("Success");
        as_.setSucceeded(result_);
    }
    else{
        result_.message = "Failure: Goal cylces not met!";
        ROS_INFO("Failure");
        as_.setAborted(result_);
    }
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "sin_commander_service"); // name of this node will be "minimal_publisher2"

    ROS_INFO("Instantiating the action server");
	SinCommanderActionServer as_object;

    ros::spin();

    return 0;

}

