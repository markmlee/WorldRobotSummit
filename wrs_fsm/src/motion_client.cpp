/*for ROS Action msg */
#include "ros/ros.h"

#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

/*custom defined Action header for robot motion */
#include <ros_podo_connector/RosPODOmotionAction.h>
#include <ros_podo_connector/RosPODO_BaseAction.h>
#include <ros_podo_connector/RosPODO_ArmAction.h>
#include <ros_podo_connector/RosPODO_GripperAction.h>
/*custom defined header to TCP/IP to PODO */
#include "../../ros_podo_connector/ros_podo_connector/src/ROSLANData.h"

/*Pre-defined Action msg*/
#include <wrs_fsm/fsm_motionAction.h>


#include <stdio.h>
#include <string.h>

enum MOTION_CMD
{
    MOTION_NONE= 0,
    MOTION_GRASPING,
    MOTION_ALIGN,
};

// global variables ==========
int motion_flag = false;  //check if the motion's running. motion running -> FSM action request holding
int product_id = 0; int found = 0;
float pose_x, pose_y, pose_z, ori_w, ori_x, ori_y , ori_z = 0;
// end global variables ======



class fsm_motionAction
{
protected:
    ros::NodeHandle nh_fsm_motion;
    actionlib::SimpleActionServer<wrs_fsm::fsm_motionAction> asfsm_motion_;
    std::string action_name_;

    // create messages that are used to published feedback&result
    wrs_fsm::fsm_motionFeedback feedback_;
    wrs_fsm::fsm_motionResult result_;

    int goal_cmd = 0;

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

        std::cout <<"==========================="  << std::endl;
        std::cout <<"GOAL CMD: "<< goal->motion_cmd << std::endl;

        get_goal(goal);

        goal_cmd = goal->motion_cmd;

        //While loop waiting unitl the ros_podo_connector result
        while(ros::ok())
        {
          ros::Duration(0.005).sleep();
          if(motion_flag) break;
        }

        motion_flag = false;
        result_.result_flag = 1;

        set_result();
        asfsm_motion_.setSucceeded(result_);
        reset_result();

    }


    //1 if alive, 0 else
    int returnServerStatus()
    {
        if(asfsm_motion_.isActive()) { return 1; }
        else { return 0; }
    }

    int getGoalCmd()
    {
      return goal_cmd;
    }

    void set_result()
    {
        result_.result_flag = true;
        result_.found = found;
        result_.product_id = product_id;
        result_.pose_x = pose_x;
        result_.pose_y = pose_y;
        result_.pose_z = pose_z;
        result_.ori_x = ori_x;
        result_.ori_y = ori_y;
        result_.ori_z = ori_z;
        result_.ori_w = ori_w;
    }

    void reset_result()
    {
        result_.result_flag = false;
        result_.found = 0;
        result_.product_id = 0;
        result_.pose_x = 0.0;
        result_.pose_y = 0.0;
        result_.pose_z = 0.0;
        result_.ori_x = 0.0;
        result_.ori_y = 0.0;
        result_.ori_z = 0.0;
        result_.ori_w = 0.0;
    }

    void get_goal(const wrs_fsm::fsm_motionGoalConstPtr &goal)
    {
      product_id = goal->product_id;
      pose_x = goal->pose_x;
      pose_y = goal->pose_y;
      pose_z = goal->pose_z;
      ori_x = goal->ori_x;
      ori_y = goal->ori_y;
      ori_z = goal->ori_z;
      ori_w = goal->ori_w;
    }



};



int main(int argc, char **argv)
{
    ros::init(argc, argv, "motion_client");

    fsm_motionAction fsm_motion("fsm_motion");
    ROS_INFO("Starting motion_client module");
    ros::Rate loop_rate(10);

    // create the action client
    actionlib::SimpleActionClient<ros_podo_connector::RosPODO_BaseAction> ac_base("rospodo_base", true);
    actionlib::SimpleActionClient<ros_podo_connector::RosPODO_ArmAction> ac_arm("rospodo_arm", true);
    actionlib::SimpleActionClient<ros_podo_connector::RosPODO_GripperAction> ac_gripper("rospodo_gripper", true);

    //ROS_INFO("Waiting for action server to start.");
    // wait for the action server to start
    //ac_base.waitForServer(); //will wait for infinite time
    //ac_arm.waitForServer(); //will wait for infinite time
    //ac_gripper.waitForServer(); //will wait for infinite time

    ROS_INFO("Action server started, sending goal.");
    // send a goal to the action
    ros_podo_connector::RosPODO_BaseGoal goal_base;
    ros_podo_connector::RosPODO_BaseGoal goal_base_return;
    ros_podo_connector::RosPODO_ArmGoal goal_arm;
    ros_podo_connector::RosPODO_GripperGoal goal_gripper;


    while(ros::ok())
    {

        if(fsm_motion.returnServerStatus())
        {
            if(motion_flag == false)
            {
              switch(fsm_motion.getGoalCmd())
              {
                case MOTION_NONE:
                  break;
                case MOTION_GRASPING:
                {
                  std::cout << "MOTION_GRASPING" << std::endl;

                  std::cout << "product_id: " << product_id << std::endl;
                  std::cout << "pose_x: " << pose_x << std::endl;
                  std::cout << "pose_y: " << pose_y << std::endl;
                  std::cout << "pose_z: " << pose_z << std::endl;
                  std::cout << "ori_x: " << ori_x << std::endl;
                  std::cout << "ori_y: " << ori_y << std::endl;
                  std::cout << "ori_z: " << ori_z << std::endl;
                  std::cout << "ori_w: " << ori_w << std::endl;

                  // move arm backward
                  goal_arm.jointmove_cmd = 3;
                  goal_arm.wbik_ref[RIGHT_HAND].OnOff_position = CONTROL_ON;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[0] = 0.5;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[1] = -0.3;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[2] = 0.3;
                  goal_arm.wbik_ref[RIGHT_HAND].GoalmsTime = 2000;


                  goal_arm.wbik_ref[RIGHT_ELBOW].OnOff_position = CONTROL_ON;
                  goal_arm.wbik_ref[RIGHT_ELBOW].goal_angle = -30.0;
                  goal_arm.wbik_ref[RIGHT_ELBOW].GoalmsTime = 2000;

                  ac_arm.sendGoal(goal_arm);


                  //wait for the action to return
                  bool finished_before_timeout = ac_arm.waitForResult(ros::Duration(60.0));

                  if (finished_before_timeout)
                  {
                    actionlib::SimpleClientGoalState state = ac_arm.getState();
                    ROS_INFO("Action part1 finished: %s",state.toString().c_str());
                  }
                  else
                  {
                    ROS_INFO("Action did not finish before the time out.");
                    break;
                  }


                  //Grasping Pose
                  goal_arm.jointmove_cmd = 3;
                  goal_arm.wbik_ref[RIGHT_HAND].OnOff_position = CONTROL_ON;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[0] = 0.5;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[1] = -0.3;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[2] = 0.2;
                  goal_arm.wbik_ref[RIGHT_HAND].GoalmsTime = 2000;


                  goal_arm.wbik_ref[RIGHT_ELBOW].OnOff_position = CONTROL_ON;
                  goal_arm.wbik_ref[RIGHT_ELBOW].goal_angle = -30.0;
                  goal_arm.wbik_ref[RIGHT_ELBOW].GoalmsTime = 2000;


                  ac_arm.sendGoal(goal_arm);

                  finished_before_timeout = ac_arm.waitForResult(ros::Duration(60.0));
                  if (finished_before_timeout)
                  {
                    actionlib::SimpleClientGoalState state = ac_arm.getState();
                    ROS_INFO("Action finished: %s",state.toString().c_str());
                  }
                  else
                    ROS_INFO("Action did not finish before the time out.");


                  motion_flag = true;
                  break;
                }
                case MOTION_ALIGN:
                {
                  std::cout << "MOTION_ALIGN" << std::endl;

                  int pre_motion_flag = false;

                  goal_arm.jointmove_cmd = 3;
                  goal_arm.wbik_ref[RIGHT_HAND].OnOff_position = CONTROL_ON;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[0] = 0.4;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[1] = -0.2;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_position[2] = 0.2;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_orientation[0] = 1.0;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_orientation[1] = 0.0;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_orientation[2] = 0.0;
                  goal_arm.wbik_ref[RIGHT_HAND].goal_orientation[3] = 0.0;
                  goal_arm.wbik_ref[RIGHT_HAND].GoalmsTime = 2000;

                  goal_arm.wbik_ref[RIGHT_ELBOW].OnOff_position = CONTROL_ON;
                  goal_arm.wbik_ref[RIGHT_ELBOW].goal_angle = -15.0;
                  goal_arm.wbik_ref[RIGHT_ELBOW].GoalmsTime = 2000;

                  ac_arm.sendGoal(goal_arm);

                  //wait for the action to return
                  bool finished_before_timeout = ac_arm.waitForResult(ros::Duration(60.0));

                  if (finished_before_timeout)
                  {
                    actionlib::SimpleClientGoalState state = ac_arm.getState();
                    ROS_INFO("Action finished: %s",state.toString().c_str());
                    motion_flag = true;
                  }
                  else
                    ROS_INFO("Action did not finish before the time out.");

                  break;
                }
              }
            }

        }
        else {
          //std::cout << "server dead" <<std::endl;
        }

        ros::spinOnce();

        //loop at desired rate
        loop_rate.sleep();

    }

    return 0;

}

