#include "ros/ros.h"
#include <std_msgs/String.h>
#include "Tcp.h"
#include "track.h"



ros::Publisher tcp_environment_pub;
std_msgs::String environment_message;

/**
  * @brief  从节点中读取一帧环境信息，间隔为1s
  * @param  无
  * @retval 无
  */
void Tcp_SubEnvironmentMessage_CallBack(const std_msgs::String::ConstPtr& msg)
{
    environment_message.data = msg->data;
    ROS_INFO("Recieved message from usart: %s", environment_message.data.c_str());
}

/**
  * @brief  服务器发送环境信息至上位机，间隔为1s
  * @param  无
  * @retval 无
  */
void Tcp_SendEnvironmentMessage_CallBack(const ros::TimerEvent &event)
{
    Tcp_Server_Send(environment_message.data.c_str());
    ROS_INFO("Tcp server send message: %s", environment_message.data.c_str());
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, " ");     
    /*--- ROS INIT ----------------------------------------------------------- */
    ros::init(argc, argv, "tcp_node");

    ros::NodeHandle nh;

    ros::Subscriber tcp_environment_sub = nh.subscribe<std_msgs::String>("usart_environment_message", 1000, Tcp_SubEnvironmentMessage_CallBack);
    ros::Publisher tcp_pointer_sub = nh.advertise<std_msgs::String>("message_point_listener", 1000);

    /*--- Tcp Server Init ---*/
    Tcp_Server_Init(SERVER_PORT);

    ros::Timer timer_tcp_pub = nh.createTimer(ros::Duration(1.0), Tcp_SendEnvironmentMessage_CallBack);

    // 创建一个MultiThreadedSpinner，参数为线程的数量
    ros::MultiThreadedSpinner spinner(2); // 使用2个线程
    spinner.spin();

    return 0;
}