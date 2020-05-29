/*for ROS Action msg */
#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>

/*Pre-defined Action msg*/
#include <wrs_fsm/naviAction.h>
#include <wrs_fsm/fsm_motionAction.h>
#include <wrs_fsm/manipulationAction.h>
#include <rb5_ros_wrapper/manipulationAction.h>


#include <stdio.h>
#include <string.h>



class fsm_motionAction
{
protected:
    ros::NodeHandle nh_fsm_motion;
    actionlib::SimpleActionServer<wrs_fsm::fsm_motionAction> asfsm_motion_;
    std::string action_name_;

    // create messages that are used to published feedback&result
    wrs_fsm::fsm_motionFeedback feedback_;
    wrs_fsm::fsm_motionResult result_;


public:

    fsm_motionAction(std::string name) :
        asfsm_motion_(nh_fsm_motion, name, boost::bind(&fsm_motionAction::executeCB, this, _1), false),
        action_name_(name)
    {
        asfsm_motion_.start();
    }

    ~fsm_motionAction(void)
    {
    }



    /*Call Back function when goal is received from Action client*/
    void executeCB(const wrs_fsm::fsm_motionGoalConstPtr &goal)
    {
        ros::Rate r(1);
        r.sleep();

        std::cout << std::endl << "motion_bridge_dummy CB" <<std::endl;

        //Print recieved msg
        std::cout <<"pose_x: " << goal->pose_x << std::endl;
        std::cout <<"pose_y: " << goal->pose_y << std::endl;
        std::cout <<"pose_z: " << goal->pose_z << std::endl;
        std::cout <<"ori_w: "  << goal->ori_w  << std::endl;
        std::cout <<"ori_x: "  << goal->ori_x  << std::endl;
        std::cout <<"ori_y: "  << goal->ori_y  << std::endl;
        std::cout <<"ori_z: "  << goal->ori_z  << std::endl;



        //set Result
        result_.result_flag = 1;
        result_.pose_x = 3.0;
        result_.pose_y = 0.0;
        result_.pose_z = 0.0;
        result_.ori_x = 0.0;
        result_.ori_y = 0.0;
        result_.ori_z = 0.0;
        result_.ori_w = 1.0;


        asfsm_motion_.setSucceeded(result_);

    }
};

class manipulationAction
{
protected:
    ros::NodeHandle nh_manipulation;
//    actionlib::SimpleActionServer<wrs_fsm::manipulationAction> asmanipulation_;
    actionlib::SimpleActionServer<rb5_ros_wrapper::manipulationAction> asmanipulation_;
    std::string action_name_;

    // create messages that are used to published feedback&result
//    wrs_fsm::manipulationFeedback feedback_;
//    wrs_fsm::manipulationResult result_;
    rb5_ros_wrapper::manipulationFeedback feedback_;
    rb5_ros_wrapper::manipulationResult result_;


public:

    manipulationAction(std::string name) :
        asmanipulation_(nh_manipulation, name, boost::bind(&manipulationAction::executeCB, this, _1), false),
        action_name_(name)
    {
        asmanipulation_.start();
    }

    ~manipulationAction(void)
    {
    }



    /*Call Back function when goal is received from Action client*/
//    void executeCB(const wrs_fsm::manipulationGoalConstPtr &goal)
    void executeCB(const rb5_ros_wrapper::manipulationGoalConstPtr &goal)
    {
        ros::Rate r(1);
        r.sleep();

        std::cout << std::endl << "motion_bridge_dummy CB" <<std::endl;

        //Print recieved msg
        std::cout <<"pose_x: " << goal->pose_x << std::endl;
        std::cout <<"pose_y: " << goal->pose_y << std::endl;
        std::cout <<"pose_z: " << goal->pose_z << std::endl;
        std::cout <<"ori_r: "  << goal->ori_r  << std::endl;
        std::cout <<"ori_p: "  << goal->ori_p  << std::endl;
        std::cout <<"ori_y: "  << goal->ori_y  << std::endl;



        //set Result
        result_.result_flag = 1;
        result_.pose_x = 3.0;
        result_.pose_y = 0.0;
        result_.pose_z = 0.0;
        result_.ori_r = 0.0;
        result_.ori_p = 0.0;
        result_.ori_y = 0.0;


        asmanipulation_.setSucceeded(result_);

    }
};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "motion_bridge_dummy");

//  fsm_motionAction motion_bridge_dummy("fsm_motion");
  manipulationAction motion_bridge_dummy("manipulation");
  ROS_INFO("Starting motion_bridge_dummy module");

  ros::spin();
  return 0;

}

