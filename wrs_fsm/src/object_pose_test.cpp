/*for ROS Action msg */
#include "ros/ros.h"
#include <actionlib/server/simple_action_server.h>

/*Pre-defined Action msg*/
#include <wrs_fsm/naviAction.h>
#include <wrs_fsm/detectAction.h>
#include <rb5_ros_wrapper/update.h>


//aruco
//#include <aruco_ros/aruco_ros_utils.h>
#include <aruco_msgs/MarkerArray.h>

#include <wrs_fsm/tf_broadcast.h>


#include <stdio.h>
#include <string.h>



//tf
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/Pose.h"
#include <tf/transform_listener.h>
#include <tf/LinearMath/Matrix3x3.h>
#include "tf/transform_datatypes.h"


#define D2R 0.0174533

// === Global variables =====
int detection_start_flag = false;
int marker_detected_flag = false;

int result_flag = 0;
int product_id = 0;
float aruco_x = 0.0;
float aruco_y = 0.0;
float aruco_z = 0.0;
float aruco_wx = 0.0;
float aruco_wy = 0.0;
float aruco_wz = 0.0;
float aruco_w = 1.0;

//robot tcp
float robot_x = 0.0;
float robot_y = 0.0;
float robot_z = 0.0;
float robot_rr = 0.0;
float robot_pp = 0.0;
float robot_yy = 0.0;


//after conversion
float pose_x = 0.0;
float pose_y = 0.0;
float pose_z = 0.0;
float pose_rr = 0.0;
float pose_pp = 0.0;
float pose_yy = 0.0;


int grasp_id = 0;

// End Global variables =====

void aruco30_callback(const aruco_msgs::MarkerArray& aurco_msg){

    //debugging
    //std::cout<< "aruco30_callback is called" << std::endl;

//    std::cout<< std::endl << "+++ aruco30_callback +++" << std::endl;

    product_id = aurco_msg.markers[0].id;
    aruco_x  = aurco_msg.markers[0].pose.pose.position.x;
    aruco_y  = aurco_msg.markers[0].pose.pose.position.y;
    aruco_z  = aurco_msg.markers[0].pose.pose.position.z;
    aruco_wx = aurco_msg.markers[0].pose.pose.orientation.x;
    aruco_wy = aurco_msg.markers[0].pose.pose.orientation.y;
    aruco_wz = aurco_msg.markers[0].pose.pose.orientation.z;
    aruco_w  = aurco_msg.markers[0].pose.pose.orientation.w;

//    std::cout << "product_id: " << product_id << std::endl;
//    std::cout << "aruco_x: "    << aruco_x << std::endl;
//    std::cout << "aruco_y: "    << aruco_y << std::endl;
//    std::cout << "aruco_z: "    << aruco_z << std::endl;
//    std::cout << "aruco_wx: "   << aruco_wx << std::endl;
//    std::cout << "aruco_wy: "   << aruco_wy << std::endl;
//    std::cout << "aruco_wz: "   << aruco_wz << std::endl;
//    std::cout << "aruco_w: "    << aruco_w << std::endl;

    marker_detected_flag = true;
    detection_start_flag = false;

}

/* === robot States Call back === */
void robot_states_callback(const rb5_ros_wrapper::update& robot_states_msg){

//    std::cout<< std::endl << "=== robot_states_callback ===" << std::endl;

    robot_x  = robot_states_msg.tcp_position[0]/1000.0;
    robot_y  = robot_states_msg.tcp_position[1]/1000.0;
    robot_z  = robot_states_msg.tcp_position[2]/1000.0;
    robot_rr = robot_states_msg.tcp_position[3]*D2R;
    robot_pp = robot_states_msg.tcp_position[4]*D2R;
    robot_yy = robot_states_msg.tcp_position[5]*D2R;

    std::cout << "x = "  << robot_x  << std::endl;
    std::cout << "y = "  << robot_y  << std::endl;
    std::cout << "z = "  << robot_z  << std::endl;
    std::cout << "rr = " << robot_rr << std::endl;
    std::cout << "pp = " << robot_pp << std::endl;
    std::cout << "yy = " << robot_yy << std::endl << std::endl;

}

void object_pose()
{
    // TOOL HTM from BASE to TOOL
    tf::Vector3     _tool_pos(robot_x,robot_y,robot_z);
    tf::Matrix3x3   _tool_rot;
    _tool_rot.setEulerYPR(robot_yy,robot_pp,robot_rr);
    tf::Transform   _tool_htm(_tool_rot,_tool_pos);

    // CAMERA HTM from TOOL to CAMERA
    tf::Vector3     _camera_pos(0.0325,  -0.1008,  0.0730);
    tf::Matrix3x3   _camera_rot(-1.0,  0.0,  0.0,
                                 0.0,  0.0, -1.0,
                                 0.0, -1.0,  0.0);

    tf::Transform   _camera_htm(_camera_rot,_camera_pos);

    // Marker HTM from Camera to Marker
    tf::Vector3     _marker_pos(aruco_x,aruco_y,aruco_z);
    tf::Quaternion  _marker_qua(aruco_wx,aruco_wy,aruco_wz,aruco_w);
    tf::Transform   _marker_htm(_marker_qua,_marker_pos);

    // Marker HTM from Base to Marker
    tf::Vector3     _object_pos;
    tf::Matrix3x3   _object_rot;
    double _object_px, _object_py, _object_pz;
    double _object_rr, _object_pp, _object_yy;

//    tf::Transform   _object_htm=_tool_htm*_camera_htm*_marker_htm;
    tf::Transform   _object_htm=_tool_htm*_camera_htm*_marker_htm;


    _object_pos=_object_htm.getOrigin();
    _object_rot=_object_htm.getBasis();

    _object_px=_object_pos.getX();
    _object_py=_object_pos.getY();
    _object_pz=_object_pos.getZ();

    _object_rot.getEulerYPR(_object_yy,_object_pp,_object_rr);

    std::cout << " x: "     << _object_px << std::endl;
    std::cout << " y: "     << _object_py << std::endl;
    std::cout << " z: "     << _object_pz << std::endl;
    std::cout << " roll: "  << _object_rr/D2R << std::endl;
    std::cout << " pitch: " << _object_pp/D2R << std::endl;
    std::cout << " yaw: "   << _object_yy/D2R << std::endl;

}

/* === transform function === */
void aruco2grasp_coordinate()
{
    tf::Vector3     _maker_pos(aruco_x,aruco_y,aruco_z);
    tf::Quaternion  _maker_qua(aruco_wx,aruco_wy,aruco_wz,aruco_w);
    tf::Transform   _marker(_maker_qua,_maker_pos);

    tf::Vector3     _offset_pos(0.0,  0.0,  0.0);
    tf::Matrix3x3   _offset_rot(1.0,  0.0,  0.0,
                                0.0,  1.0,  0.0,
                                0.0,  0.0,  1.0);

	grasp_id = 1;
    //set _offset_pos & _offset_rot
    switch(grasp_id)
    {
    case 0:     //No translation
        std::cout << "grasp case 0" << std::endl;
        _offset_pos.setValue(0.0, 0.0, 0.0);
        _offset_rot.setValue(1.0, 0.0,  0.0,
                             0.0, 1.0,  0.0,
                             0.0, 0.0,  1.0);
        break;
    case 1:     //Sandwich,Pouch front
        std::cout << "grasp case 1" << std::endl;
        _offset_pos.setValue(0.0, -0.001, 0.0);
        _offset_rot.setValue(0.0,    0.0,    1.0,
                             0.0,    1.0,    0.0,
                             -1.0,   0.0,    0.0);
        break;
    case 2:     //Sandwich,pouch back
        std::cout << "grasp case 2" << std::endl;
        _offset_pos.setValue(0.0, -0.001, 0.0);
        _offset_rot.setValue(0.0,    0.0,    1.0,
                             0.0,    -1.0,   0.0,
                             1.0,    0.0,    0.0);
        break;
    case 3:     //No case assigned yet
        std::cout << "grasp case 3" << std::endl;
        _offset_pos.setValue(0.0, 0.0, 0.0);
        _offset_rot.setValue(1.0, 0.0,  0.0,
                             0.0, 1.0,  0.0,
                             0.0, 0.0,  1.0);
        break;
    case 4:     //No case assigned yet
        std::cout << "grasp case 4" << std::endl;
        _offset_pos.setValue(0.0, 0.0, 0.0);
        _offset_rot.setValue(1.0, 0.0,  0.0,
                             0.0, 1.0,  0.0,
                             0.0, 0.0,  1.0);
        break;
    case 5:     //kimbap    front
        std::cout << "grasp case 5" << std::endl;
        _offset_pos.setValue(0.0, -0.001, 0.0);
        _offset_rot.setValue(0.0,   0.0,    1.0,
                             1.0,   0.0,    0.0,
                             0.0,   1.0,    0.0);
        break;
    case 6:     //kimbap    back
        std::cout << "grasp case 6" << std::endl;
        _offset_pos.setValue(0.0, -0.001, 0.0);
        _offset_rot.setValue( 0.0,   0.0,   1.0,
                             -1.0,   0.0,   0.0,
                              0.0,  -1.0,   0.0);
        break;
    default:
        std::cout << "default" << std::endl;
        break;
    }

    tf::Transform   _offset(_offset_rot,_offset_pos);

    tf::Transform   _ctr=_marker*_offset;
    tf::Vector3     _ctr_pos=_ctr.getOrigin();
    tf::Quaternion  _ctr_qua=_ctr.getRotation();

    //debug
    //DEBUG PRINTING
//    std::cout << "id = "  << aruco_data[0].id << ": ";
//    std::cout << "x = "   << _ctr_pos.getX() << ", "
//              << "y = "   << _ctr_pos.getY() << ", "
//              << "z = "   << _ctr_pos.getZ() << ", "
//              << "wx = "  << _ctr_qua.getAxis().getX() << ", "
//              << "wy = "  << _ctr_qua.getAxis().getY() << ", "
//              << "wz = "  << _ctr_qua.getAxis().getZ() << ", "
//              << "w = "   << _ctr_qua.getW() << std::endl << std::endl;

    aruco_x   = _ctr_pos.getX();
    aruco_y   = _ctr_pos.getY();
    aruco_z   = _ctr_pos.getZ();
    aruco_wx  = _ctr_qua.getAxis().getX();
    aruco_wy  = _ctr_qua.getAxis().getY();
    aruco_wz  = _ctr_qua.getAxis().getZ();
    aruco_w   = _ctr_qua.getW();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "object_pose_test");

  ros::Subscriber aruco30_sub, robot_state_sub;
  ros::NodeHandle n;
  
    ros::NodeHandle nh_broadCast;
  
  aruco30_sub = n.subscribe("/aruco_marker_size030/markers", 1, aruco30_callback);
  robot_state_sub = n.subscribe("/robot_states", 1, robot_states_callback);
  ros::Publisher pb_broadcast = nh_broadCast.advertise<wrs_fsm::tf_broadcast>("marker_tf", 10);

  ros::Rate rate(1);
  int just_count = 0;

  wrs_fsm::tf_broadcast marker_tf;
  
  while(ros::ok())
  {
      std::cout << std::endl << "-----------------------" << std::endl;
//      std::cout << "loop count: " << ++just_count << std::endl;

      ros::spinOnce();
		
        std::cout << "Before" << std::endl;
		std::cout << "aruco_x= " << aruco_x;
		std::cout << "aruco_y= " << aruco_y;
		std::cout << "aruco_z= " << aruco_z;
		std::cout << "aruco_w= " << aruco_w;
		std::cout << "aruco_wx= " << aruco_wx;
		std::cout << "aruco_wy= " << aruco_wy;
        std::cout << "aruco_wz= " << aruco_wz;
        
        
        aruco2grasp_coordinate();
        
        std::cout << "After" << std::endl;
		std::cout << "aruco_x= " << aruco_x;
		std::cout << "aruco_y= " << aruco_y;
		std::cout << "aruco_z= " << aruco_z;
		std::cout << "aruco_w= " << aruco_w;
		std::cout << "aruco_wx= " << aruco_wx;
		std::cout << "aruco_wy= " << aruco_wy;
        std::cout << "aruco_wz= " << aruco_wz;
                    
		marker_tf.x  = aruco_x;
		marker_tf.y  = aruco_y;
		marker_tf.z  = aruco_z;
		marker_tf.w  = aruco_w;
		marker_tf.wx = aruco_wx;
		marker_tf.wy = aruco_wy;
		marker_tf.wz = aruco_wz;

		pb_broadcast.publish(marker_tf);



      //object_pose();

      rate.sleep();

  }

  return 0;

}

