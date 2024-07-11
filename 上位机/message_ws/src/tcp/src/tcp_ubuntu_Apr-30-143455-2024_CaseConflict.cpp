#include "ros/ros.h"
#include <std_msgs/String.h>


ros::Publisher tcp_environment_pub;
std_msgs::String environment_message;

void Tcp_SubEnvironmentMessage_CallBack(const std_msgs::String::ConstPtr& msg)
{
    environment_message.data = msg->data;
    ROS_INFO("Recieved message from usart: %s", environment_message.data.c_str());
}

void Tcp_PubEnvironmentMessage_CallBack(const ros::TimerEvent &event)
{
    tcp_environment_pub.publish(environment_message);
    ROS_INFO("Publish message to tcp: %s", environment_message.data.c_str());
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, " ");     
    /*--- ROS INIT ----------------------------------------------------------- */
    ros::init(argc, argv, "tcp_node");

    ros::NodeHandle nh;

    ros::Subscriber tcp_environment_sub = nh.subscribe<std_msgs::String>("usart_environment_message", 1000, Tcp_SubEnvironmentMessage_CallBack);
    ros::Publisher tcp_environment_pub = nh.advertise<std_msgs::String>("tcp_environment_message", 1000);
    ros::Publisher tcp_pointer_sub = nh.advertise<std_msgs::String>("message_point_listener", 1000);

    ros::Timer timer_tcp_pub = nh.createTimer(ros::Duration(1.0), Tcp_PubEnvironmentMessage_CallBack);

    ros::spin();

    return 0;
}