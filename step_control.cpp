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
    command_pub = nh.advertise<std_msgs::String>("step_command", 1000);
    enable_pub = nh.advertise<std_msgs::Bool>("step_enable", 1000);

    joy_sub = nh.subscribe("joy", 10, &StepControl::JoyMessageReceived, this);
    init_flag=true;

    record_topics="topic";
    nh_priv.getParam("record_topics", record_topics);
    ROS_INFO_STREAM("Topic to that can be recorded: " << record_topics);

    record_client= nh.serviceClient<record_ros::String_cmd>("record/cmd");

    _enable=false;



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
        int pressed_button_id=-1;
        for(int i=0; i<8; i++)
        {
            if(checkButton(joy, i))
            {
                pressed_button_id=i;
                break;
            }
        }
        switch(pressed_button_id)
        {

        case 0:
            //sendControl(NAVIGATION_START);
            stepDetectorStart();

        break;
        case 1:
            //sendControl(NAVIGATION_STOP);
        break;
        case 2:
            recordTopicStart();

        break;
        case 3:
            stepDetectorStop();

        break;
        case 4:

        break;
        case 5:
            recordTopicStop();
        break;
        case 6:
            sendEnable();

        break;
        case 7:
        break;
        case -1:
        break;


        }
    }

}

void StepControl::sendControl(int control_request)
{
    step_msgs::Control control_msg;
    control_msg.control_request=control_request;
    control_pub.publish(control_msg);

}

bool StepControl::checkButton(sensor_msgs::Joy joy, int button_id)
{
    if(joy.buttons[button_id]!=joy_last_buttons[button_id])
    {
        joy_last_buttons[button_id]=joy.buttons[button_id];
        if(joy_last_buttons[button_id])
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void StepControl::recordTopicStart()
{
    //call service
    record_ros::String_cmd srv;
    srv.request.cmd="record";
    if(record_client.call(srv))
    {
        voice_command.data="Nagrywanie rozpoczęte";
        command_pub.publish(voice_command);
    }
    else
    {
        voice_command.data="Błąd nagrywania";
        command_pub.publish(voice_command);
    }
}

void StepControl::recordTopicStop()
{
    //call service
    record_ros::String_cmd srv;
    srv.request.cmd="stop";
    record_client.call(srv);    //no response - record node is simply killed to stop recording
        voice_command.data="Nagrywanie zakończone";
        command_pub.publish(voice_command);
}

void StepControl::stepDetectorStart()
{
    std::stringstream ss;
    ss << "screen -S pride -p 3 -X stuff \"roslaunch step_detector step_detector.launch \n\"";
    system (ss.str().c_str());
    ss.str("");
}

void StepControl::stepDetectorStop()
{
    std::stringstream ss;
    ss << "screen -S pride -p 3 -X  stuff \"^C\n\"";
     system (ss.str().c_str());
     ss.str("");
}

void StepControl::sendEnable()
{
    _enable=!_enable;
    std_msgs::Bool enable_msg;
    enable_msg.data=_enable;
    enable_pub.publish(enable_msg);
}

