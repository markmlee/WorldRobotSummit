/*for ROS Action msg */
#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>

/*Pre-defined Action msg*/
#include <wrs_fsm/naviAction.h>
#include <wrs_fsm/detectAction.h>

//aruco
//#include <aruco_ros/aruco_ros_utils.h>
#include <aruco_msgs/MarkerArray.h>


#include <stdio.h>
#include <string.h>

// === Global variables =====
int detection_start_flag = false;
int marker_detected_flag = false;

int result_flag = 1;
int product_id = 1;
float pose_x = 3.0;
float pose_y = 0.0;
float pose_z = 0.0;
float ori_x = 0.0;
float ori_y = 0.0;
float ori_z = 0.0;
float ori_w = 1.0;
// End Global variables =====

void aruco30_callback(const aruco_msgs::MarkerArray& aurco_msg){


    if(detection_start_flag)
    {
      //debugging
      //std::cout<< "aruco30_callback is called" << std::endl;

      std::cout<< "id: " << aurco_msg.markers[0].id << std::endl;
      std::cout<< "pose x: " << aurco_msg.markers[0].pose.pose.position.x << std::endl;

      product_id = aurco_msg.markers[0].id;
      pose_x = aurco_msg.markers[0].pose.pose.position.x;
      pose_y = aurco_msg.markers[0].pose.pose.position.y;
      pose_z = aurco_msg.markers[0].pose.pose.position.z;
      ori_x = aurco_msg.markers[0].pose.pose.orientation.x;
      ori_y = aurco_msg.markers[0].pose.pose.orientation.y;
      ori_z = aurco_msg.markers[0].pose.pose.orientation.z;
      ori_w = aurco_msg.markers[0].pose.pose.orientation.w;
    }

    marker_detected_flag = true;
    detection_start_flag = false;

}

class detectAction
{
protected:
    ros::NodeHandle nh_detect;
    actionlib::SimpleActionServer<wrs_fsm::detectAction> asDetect_;
    std::string action_name_;

    // create messages that are used to published feedback&result
    wrs_fsm::detectFeedback feedback_;
    wrs_fsm::detectResult result_;




public:

    detectAction(std::string name) :
        asDetect_(nh_detect, name, boost::bind(&detectAction::executeCB, this, _1), false),
        action_name_(name)
    {
        asDetect_.start();
    }

    ~detectAction(void)
    {
    }

    /*Call Back function when goal is received from Action client*/
    void executeCB(const wrs_fsm::detectGoalConstPtr &goal)
    {
        ros::Rate r(200);
        r.sleep();

        std::cout << std::endl << "detect_dummy CB" <<std::endl;

        //Print recieved msg
        std::cout <<"aruco_mode: " << goal->aruco_mode << std::endl;
        detection_start_flag = true;


        /* ============== aruco_ros 30mm subscriber==============  */
        ros::Subscriber aruco30_sub;
        ros::NodeHandle n;
        aruco30_sub = n.subscribe("/aruco_marker_size020/markers", 100, aruco30_callback);


        while(marker_detected_flag == 0)
        {
          r.sleep();
        }
        marker_detected_flag = false;


        std::cout << "result_flag: " << result_flag << std::endl;
        std::cout << "product_id: " << product_id << std::endl;
        std::cout << "pose_x: " << pose_x << std::endl;
        std::cout << "pose_y: " << pose_y << std::endl;
        std::cout << "pose_z: " << pose_z << std::endl;
        std::cout << "ori_x: " << ori_x << std::endl;
        std::cout << "ori_y: " << ori_y << std::endl;
        std::cout << "ori_z: " << ori_z << std::endl;
        std::cout << "ori_w: " << ori_w << std::endl;


        //set Result
        result_.result_flag = result_flag;
        result_.product_id = product_id;
        result_.pose_x = pose_x;
        result_.pose_y = pose_y;
        result_.pose_z = pose_z;
        result_.ori_x = ori_x;
        result_.ori_y = ori_y;
        result_.ori_z = ori_z;
        result_.ori_w = ori_w;

        asDetect_.setSucceeded(result_);
    }
};



int main(int argc, char **argv)
{
  ros::init(argc, argv, "detect_dummy");

  detectAction detect_dummy("detect_dummy");
  ROS_INFO("Starting detect_dummy module");



  ros::spin();
  return 0;

}

