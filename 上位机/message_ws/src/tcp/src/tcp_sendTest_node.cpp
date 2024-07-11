#include "ros/ros.h"


int main(int argc, char* argv[])
{
    char* buf = new char(100);
    char client_rbuf[100];

    setlocale(LC_ALL, " ");     
    /*--- ROS INIT ----------------------------------------------------------- */
    ros::init(argc, argv, "tcp_node");

    ros::NodeHandle nh;

    ros::Subscriber tcp_environment_sub = nh.subscribe<std_msgs::String>("usart_environment_message", 1000, Tcp_SubEnvironmentMessage_CallBack);
    ros::Publisher tcp_pointer_sub = nh.advertise<std_msgs::String>("message_point_listener", 1000);



    Tcp_Client_Init(SERVER_PORT, SERVER_IP);
    /* send message 1 */
    sprintf(buf, "Do you like C++?");

    while(ros::ok())
    {
        
    }
    return 0;
}