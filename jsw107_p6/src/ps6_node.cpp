
#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <osrf_gear/ConveyorBeltControl.h>
#include <osrf_gear/DroneControl.h>
#include <osrf_gear/LogicalCameraImage.h>
#include <iostream>
#include <string>

using namespace std;


bool g_take_new_snapshot = false;
osrf_gear::LogicalCameraImage g_cam2_data;

void cam2CB(const osrf_gear::LogicalCameraImage& message_holder) {
    if (g_take_new_snapshot) {
        //ROS_INFO_STREAM("image from cam2: " << message_holder << endl);
        g_cam2_data = message_holder;
        
        /*
        if(g_cam2_data.models.size() >= 1){
            if(g_cam2_data.models[0].pose.position.z < 0.1 && g_cam2_data.models[0].pose.position.z > -0.01)
                g_take_new_snapshot = false;
            else
                g_take_new_snapshot = true;
        }
        else
            g_take_new_snapshot = true;
        */
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ps6");
    ros::NodeHandle n;

    ros::ServiceClient startup_client = n.serviceClient<std_srvs::Trigger>("/ariac/start_competition");
    std_srvs::Trigger startup_srv;

    ros::ServiceClient conveyor_client = n.serviceClient<osrf_gear::ConveyorBeltControl>("/ariac/conveyor/control");
    osrf_gear::ConveyorBeltControl conveyor_srv;

    ros::ServiceClient drone_client = n.serviceClient<osrf_gear::DroneControl>("/ariac/drone");
    osrf_gear::DroneControl drone_srv;

    ros::Subscriber cam2_subscriber = n.subscribe("/ariac/logical_camera_2", 1, cam2CB);

    //Run startup service
    startup_srv.response.success = false;
    while (!startup_srv.response.success) {
        ROS_WARN("not started up yet...");
        startup_client.call(startup_srv);
        ros::Duration(0.5).sleep();
    }
    ROS_INFO("recieved success signal from startup service");

    //Run conveyor service
    conveyor_srv.request.power = 100.0;
    conveyor_srv.response.success = false;
    while (!conveyor_srv.response.success) {
        ROS_WARN("not started conveyor yet...");
        conveyor_client.call(conveyor_srv);
        ros::Duration(0.5).sleep();
    }
    ROS_INFO("recieved success signal from conveyor service");

    //Camera 2, find box
    g_take_new_snapshot = true;
    while (g_cam2_data.models.size() < 1) {
        ros::spinOnce();
        ros::Duration(0.5).sleep();
    }
    ROS_INFO("found box!");

    //wait for box to center
    ROS_INFO("waiting to center...");
    while(g_cam2_data.models[0].pose.position.z > 0.1 || g_cam2_data.models[0].pose.position.z < -0.1) {
        ros::spinOnce();
        ros::Duration(0.2).sleep();
    }
    ROS_INFO("centered!");
    g_take_new_snapshot = false;

    //delay, "loading box"
    ROS_INFO("waiting to load...");
    conveyor_srv.request.power = 0.0;
    conveyor_client.call(conveyor_srv);
    ros::Duration(5.0).sleep();
    conveyor_srv.request.power = 100.0;
    conveyor_client.call(conveyor_srv);
    ROS_INFO("continue...");

    //Run drone service
    drone_srv.request.shipment_type = "cool";
    drone_srv.response.success = false;
    while (!drone_srv.response.success) {
        ROS_WARN("not started drone yet...");
        drone_client.call(drone_srv);
        ros::Duration(0.5).sleep();
    }
    ROS_INFO("recieved success signal from drone service");



    return 0;
}
