#include <step_control/step_control.h>
#include <signal.h>

StepControl *_step_control;
void mySigintHandler(int sig)
{
    ros::shutdown();
}

int main(int argc, char **argv)
{    
    ros::init(argc, argv, "step_control_node");
    _step_control=new StepControl();
    ros::Rate loop_rate(100);
    //signal(SIGINT, mySigintHandler);
    while(ros::ok())
    {
        ros::spinOnce();
        if(!loop_rate.sleep())
        {
            //ROS_WARN("Control loop too slow !");
        }
    }
    delete _step_control;


}
