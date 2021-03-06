#ifndef step_control_h
#define step_control_h

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <sensor_msgs/Joy.h>
#include <dynamic_reconfigure/server.h>
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include <step_msgs/Control.h>
#include <record_ros/String_cmd.h>
//#include <step_msgs/Step.h>


#include <cmath>
#include <sstream>
#include <string>
#include <step_control/enum_control.h>



class StepControl
{
public:
    /**
         * @brief Constructor. Initializes publishers and subscribers, gives deafult values to variables, starts threads
         */
    StepControl();

    /**
         * @brief Destructor. Deletes objects and stops threads
         */
    ~StepControl();

    ros::NodeHandle nh;
    ros::NodeHandle nh_priv;
    ros::Subscriber joy_sub;
    ros::Publisher control_pub;
    ros::Publisher command_pub;
    ros::Publisher enable_pub;
    ros::ServiceClient record_client;


    void JoyMessageReceived(const sensor_msgs::Joy &joy);






private:

    bool init_flag;
    int steps;
    bool joy_last_buttons[JOY_BUTTONS];
    bool joy_last_axes[4];
    std_msgs::String voice_command;

    std::string record_topics;
    bool _enable;

    void sendControl (int control_request);
    bool checkButton (sensor_msgs::Joy joy, int button_id);
    void recordTopicStart();
    void recordTopicStop();
    void stepDetectorStart();
    void stepDetectorStop();
    void sendEnable();




    int checkAxes(sensor_msgs::Joy joy);
};

#endif

