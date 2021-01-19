#include "ros/ros.h"
#include "ball_chaser/drivetotarget.h"
#include <sensor_msgs/Image.h>
#include <signal.h>
// Define a global client that can request services
ros::ServiceClient client;





// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO("lin :%f , ang :%f",lin_x,ang_z);
    if ((lin_x==0) && (ang_z ==0)){
    ROS_INFO_STREAM("commmading the robot to stop");}

    else if (ang_z<0){
    ROS_INFO_STREAM("commmading the robot to turn right");}
    else{ROS_INFO_STREAM("commmading the robot to turn left");}

    ball_chaser::drivetotarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

	float angular_vel;
	bool white_ball =false;
        int white_pixel = 255;

	for (int i = 0; i <  img.step; i++){
		for (int j = 1; j < img.height; j++) {
			if (img.data[j+(img.height*i)] == white_pixel) {
			    angular_vel=(((400.00-j)/400.00)*1.0);
			    ROS_INFO("i :%d ,j:%d",i,j);
			    white_ball =true;
			    break;
        }}
if (white_ball ==true) { break;}
}

	if (white_ball == true)
	{
		drive_robot(0.4, angular_vel)	;
		
		
	}
	else
	{
		drive_robot(0.0, 0.0);
	}

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::drivetotarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
