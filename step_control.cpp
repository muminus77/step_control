/**
 @file		step_control.cpp
 @author	Marcin Umiński
 @date		27-05-2017
 @brief		step_control class
 @details
 */
#include <step_control/step_control.h>

///@brief konstruktor
StepControl::StepControl():
    nh_priv("~")
{

    control_pub = nh.advertise<step_msgs::Control>("step_control", 1000);
    joy_sub = nh.subscribe("joy", 10, &StepControl::JoyMessageReceived, this);
    init_flag=true;

}


StepControl::~StepControl()
{
}

void StepControl::JoyMessageReceived(const sensor_msgs::Joy &joy)
{
    if(init_flag)
    {
        if(!joy.buttons.empty())
        {
            for(int i=0; i<joy.buttons.size(); i++)
            {
                joy_last_buttons[i]=joy.buttons[i];
            }
            init_flag=false;
        }
    }
    else
    {
        if(joy.buttons[0]!=joy_last_buttons[0])
        {
            joy_last_buttons[0]=joy.buttons[0];
            if(joy_last_buttons[0])
            {
                sendControl(NAVIGATION_START);

            }
        }
        if(joy.buttons[1]!=joy_last_buttons[1])
        {
            joy_last_buttons[1]=joy.buttons[1];
            if(joy_last_buttons[1])
            {
                sendControl(NAVIGATION_STOP);

            }
        }
    }

}

void StepControl::sendControl(int control_request)
{
    step_msgs::Control control_msg;
    control_msg.control_request=control_request;
    control_pub.publish(control_msg);

}
