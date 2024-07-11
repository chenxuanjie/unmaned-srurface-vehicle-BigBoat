#include <iostream>
using namespace std;
#include "ros/ros.h"
#include "USART1.h"
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/String.h>
#include <string>

#define USART1_PORT		(char* )"/dev/ttyUSART1"

double yaw, pitch, roll;
static char* USART1_rbuf = new char(20);

ros::Publisher environment_pub;

/**
  * @brief  每隔1s，从stm32中读取一帧环境信息
  * @param  无
  * @retval 无
  */
void Usart_GetEnvironmentMessage(const ros::TimerEvent &event)
{
    /*--- Get environment message from stm32 ---*/ 
    USART_ReadString(USART1_rbuf, 20);
}

void Usart_SendAngle(const ros::TimerEvent &event)
{
    /*--- Send eular angle to stm32 ---*/ 
    char send_string[100] = {'\0'};
    sprintf(send_string, "roll=%d,pitch=%d,yaw=%d\r\n", (int)roll, (int)pitch, (int)yaw);
    USART1_SendString(send_string);
}

void GetAngle_CallBack(const nav_msgs::Odometry::ConstPtr& msg)
{
    /* --- Get eular angle ----------------------------------*/
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
}

void PubEnvironmentMessage_CallBack(const ros::TimerEvent &event)
{
    /* --- Publish environment message ---*/
    std_msgs::String environment_message;

    environment_message.data = std::string(USART1_rbuf);
    environment_pub.publish(environment_message);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, " ");     // 
    /*--- ROS INIT ----------------------------------------------------------- */
    ros::init(argc, argv, "usart_node");

    ros::NodeHandle nh;

    environment_pub = nh.advertise<std_msgs::String>("/usart_environment_message", 1000);
    ros::Subscriber listener = nh.subscribe<nav_msgs::Odometry>("/odom_local", 1000, GetAngle_CallBack);     // get eular angle from imu

    /*--- USART1 INIT ------------------------------------------- */
    char* device_usart1 = USART1_PORT;
    while (-1 == USART1_Init(device_usart1))
    {
        ROS_ERROR("USART1 init failed, trying connect again...");
        sleep(1);
    }
    /*--- end of USART1 INIT ------------------------------------ */

    /* --- ROS send environmnet message ------------------------------------------------------------------*/
    ros::Timer timer_get_environment = nh.createTimer(ros::Duration(1.0), Usart_GetEnvironmentMessage);
    ros::Timer timer_send_RPY = nh.createTimer(ros::Duration(1.0), Usart_SendAngle);
    ros::Timer timer_pub_environment = nh.createTimer(ros::Duration(1.0), PubEnvironmentMessage_CallBack);

    ros::spin();

    return 0;
}
