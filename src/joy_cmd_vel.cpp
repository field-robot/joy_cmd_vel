#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int8.h>

#define a_loc 0
#define b_loc 1
#define x_loc 2
#define y_loc 3
#define lb_loc 4
#define rb_loc 5
#define lt_loc 6
#define rt_loc 7


double direction;
int a_button;
int b_button;
int x_button;
int y_button;
int rt_button;
int lb_button;
int lt_button;
int rb_button;

int mode = 0;

double x_velocity;
double omega;

double x_vel_prev = 0.0001;
double omega_prev= 0.0001;

void joy_stick (const sensor_msgs::Joy &stick)
{
	direction = stick.axes[0];
	a_button = stick.buttons[a_loc];
	b_button = stick.buttons[b_loc];
	x_button = stick.buttons[x_loc];
	y_button = stick.buttons[y_loc];
	rt_button = stick.buttons[rt_loc];
	lb_button = stick.buttons[lb_loc];
	rb_button = stick.buttons[rb_loc];
	lt_button = stick.buttons[lt_loc];
	
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


	ros::Subscriber joystick = nh.subscribe("joy", 1, &joy_stick);	
	ros::Publisher send_cmd_vel = nh.advertise<geometry_msgs::Twist>("cmd_vel",1);
	ros::Publisher send_mode = nh.advertise<std_msgs::Int8>("Mode",1);

	ros::Rate loop_rate(10);
	while(ros::ok())
	{
		if ((rt_button == 1)&(mode==0)){
			 mode = 1;
			ROS_INFO("Mode is 1");
			system("rosrun wheels_controller wheels_controller &");
			
		}
		if ((rb_button == 1)&(mode==0)){
			mode = 2;
			ROS_INFO("Mode is 2");
		}
		if ((lt_button == 1)&(mode==0)){
			 mode = 3;
			ROS_INFO("Mode is 3");
		}
		if ((lb_button == 1)&(mode==1)){
			system("rosnode kill wheels_controller");		    	
			mode = 0;
		 	
		}
		if ((lb_button == 1)&(mode==2)) mode = 0;
		if ((lb_button == 1)&(mode==3)) mode = 0;
		if (mode == 0)
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
			omega_prev = omega;
			x_vel_prev = x_velocity;
			ROS_INFO("Sending");
		}	
		std_msgs::Int8 mode_;
		mode_.data = mode;
		send_mode.publish(mode_);
		ros::spinOnce();		
		loop_rate.sleep();
	}

}
