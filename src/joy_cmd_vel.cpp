#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>

#define a_loc 0
#define b_loc 1
#define x_loc 2
#define y_loc 3


double direction;
int a_button;
int b_button;
int x_button;
int y_button;


double x_velocity;
double omega;

void joy_stick (const sensor_msgs::Joy &stick)
{
	direction = stick.axes[0];
	a_button = stick.buttons[a_loc];
	b_button = stick.buttons[b_loc];
	x_button = stick.buttons[x_loc];
	y_button = stick.buttons[y_loc];
}




int main(int argc, char **argv)
{
	ros::init(argc, argv, "joy_cmd_vel");
	//initializing package
	ros::NodeHandle nh;
	
	double max_vel_;
	double max_ang_;
	nh.param("max_vel",max_vel_,1.0);
	nh.param("max_ang",max_ang_,1.0);
	ROS_INFO("Max vel = %d", max_vel_);
	ROS_INFO("Max ang = %d", max_ang_);

	ros::Subscriber joystick = nh.subscribe("joy", 1, &joy_stick);	
	ros::Publisher send_cmd_vel = nh.advertise<geometry_msgs::Twist>("cmd_vel",1);
	ros::Rate loop_rate(10);
	while(ros::ok())
	{
		if (a_button == 1)
		{
			x_velocity = max_vel_;
		}
		if (b_button == 1)
		{
			x_velocity = -max_vel_;
		}
		if ((a_button+b_button) == 0)
		{
			x_velocity = 0;
		}
		omega = direction*max_ang_;
		
	
		geometry_msgs::Twist vel_;
		vel_.angular.z = omega;
		vel_.linear.x = x_velocity;
		send_cmd_vel.publish(vel_);
		ros::spinOnce();		
		loop_rate.sleep();
	}

}
