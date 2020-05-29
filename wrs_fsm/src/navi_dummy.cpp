/*for ROS Action msg */
#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>

/*Pre-defined Action msg*/
#include <wrs_fsm/naviAction.h>
#include <mobile_path_planning/naviAction.h>


#include <stdio.h>
#include <string.h>



class naviAction
{
protected:
    ros::NodeHandle nh_navi;
//    actionlib::SimpleActionServer<wrs_fsm::naviAction> asNavi_;
    actionlib::SimpleActionServer<mobile_path_planning::naviAction> asNavi_;
    std::string action_name_;

    // create messages that are used to published feedback&result
//    wrs_fsm::naviFeedback feedback_;
//    wrs_fsm::naviResult result_;

    mobile_path_planning::naviFeedback feedback_;
    mobile_path_planning::naviResult result_;

public:

    naviAction(std::string name) :
        asNavi_(nh_navi, name, boost::bind(&naviAction::executeCB, this, _1), false),
        action_name_(name)
    {
        asNavi_.start();
    }

    ~naviAction(void)
    {
    }



    /*Call Back function when goal is received from Action client*/
//    void executeCB(const wrs_fsm::naviGoalConstPtr &goal)
    void executeCB(const mobile_path_planning::naviGoalConstPtr &goal)
    {
        ros::Rate r(1);
        r.sleep();

        std::cout << std::endl << "navi_dummy CB" <<std::endl;

        //Print recieved msg
        std::cout <<"use_marker: " << goal->use_marker << std::endl;
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


        asNavi_.setSucceeded(result_);

    }
};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "navi_dummy");

  naviAction navi_dummy("hubo_navigation");
  //  naviAction navi_dummy("navi_dummy");
  ROS_INFO("Starting navi_dummy module");

  ros::spin();
  return 0;

}

