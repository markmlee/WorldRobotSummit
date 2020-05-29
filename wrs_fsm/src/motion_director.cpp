/*for ROS Action msg */
#include "ros/ros.h"

/*action libraries*/
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

/*custom defined Action header for robot motion */
#include <wrs_fsm/motion_directionAction.h>
#include <wrs_fsm/fsm_motionAction.h>
#include <rb5_ros_wrapper/manipulationAction.h>
#include "rb5_ros_wrapper/update.h"
#include <wrs_fsm/tf_broadcast.h>
#include "/home/rainbow/catkin_ws/src/ROS_rb5_wrs/src/wrs_parameter.h"
//#include "../../ROS_rb5_wrs/src/wrs_parameter.h"

//aruco marker
#include <aruco_msgs/MarkerArray.h>

// Cpp libraries
#include <stdio.h>
#include <string.h>

//tf 
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/Pose.h"
#include <tf/transform_listener.h>
#include <tf/LinearMath/Matrix3x3.h>
#include "tf/transform_datatypes.h"


//Custom header for inventory
#include "inventory.h"

//subscriber
ros::Subscriber manipulation_result_sub;

// === Global variables =====
#define D2R 0.0174533

enum MOTION_CMD
{
    MOTION_NONE= 0,
    MOTION_GRASPING,
    MOTION_TEST,
    MOTION_SHELFOUT,
    MOTION_SHELFIN,
    MOTION_DISPOSE,
    MOTION_DETECT_ONSHELF,
    MOTION_ALIGN_OLD,
    MOTION_SEARCH_NEW,
    MOTION_DISPLAY_NEW,
    MOTION_HOME_POSE,
};

//expire_state
enum{
  EXPIRED = 0, OLD, NEW, NEW2
};
//Result to FSM
enum{
  FAIL = 0, SUCCESS, FALLDOWN,
};

//Thread
pthread_t THREAD_t;
void* ThreadBroadCast(void*);

//flags
int detection_start_flag = false;   //aruco_marker30 read flag
int shelf_detection_start_flag = false;   //aruco_marker80 read flag
int target_detected_flag = true;
int motion_flag = false;            //check if the motion's running. motion running -> FSM action request holding. This only indicates each case is running or not.
int faceup_flag = true;

//aruco marker data
float aruco_x, aruco_y, aruco_z, aruco_w, aruco_wx, aruco_wy , aruco_wz = 0.;
float shelf_aruco_x, shelf_aruco_y, shelf_aruco_z, shelf_aruco_w, shelf_aruco_wx, shelf_aruco_wy, shelf_aruco_wz = 0.;

int array_size = 0;
int shelf_array_size = 0;
int table_size = 48;
ITEM_DATA   aruco_data[30];             //data from aruco_marker callback; maximum num of object = 12
ITEM_DATA   shelf_aruco_data[30];       //data from aruco_marker callback; maximum num of object = 12
ITEM_DATA   item[30];                   //data used for actual calculation
ITEM_TABLE  inventory_table[64];        //saved data in text file
int back_shelf_array[3] ={8,1,0};


//robot marker data
float robot_x, robot_y, robot_z = 0.;
float robot_rr, robot_pp, robot_yy = 0.;

//fsm Actions
int fsm_product_id, shelf_id = 0;
int tool_id = 0;
int item_id = 0;
int grasp_id = 0;
int motion_result = 0;       //return result to FSM
int manipulation_result = 0; // returned from Grasp.cpp

//Items with more than one new products
int put_new = 0;
//unexpected skip
int old_fail = 0;

//Result
int result = 100;

//DUMMY
char object_char = 0;


typedef actionlib::SimpleActionClient<rb5_ros_wrapper::manipulationAction> Client;
//  === END Global variables =====



/* === print wait for Result === */
bool waitForResult(actionlib::SimpleActionClient<rb5_ros_wrapper::manipulationAction> &ac)
{
    //wait for the action to return
    bool finished_before_timeout = ac.waitForResult(ros::Duration(300.0));

    if (finished_before_timeout)
    {

        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished: %s",state.toString().c_str());
        return true;
    }
    else
    {
        ROS_INFO("Action did not finish before the time out.");
        return false;
    }

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

/* === robot States Call back === */
void robot_states_callback(const rb5_ros_wrapper::update& robot_states_msg){

    robot_x  = robot_states_msg.tcp_position[0]/1000.0;
    robot_y  = robot_states_msg.tcp_position[1]/1000.0;
    robot_z  = robot_states_msg.tcp_position[2]/1000.0;
    robot_rr = robot_states_msg.tcp_position[3]*D2R;
    robot_pp = robot_states_msg.tcp_position[4]*D2R;
    robot_yy = robot_states_msg.tcp_position[5]*D2R;


//    std::cout << "x = "  << robot_x  << std::endl;
//    std::cout << "y = "  << robot_y  << std::endl;
//    std::cout << "z = "  << robot_z  << std::endl;
//    std::cout << "rr = " << robot_rr << std::endl;
//    std::cout << "pp = " << robot_pp << std::endl;
//    std::cout << "yy = " << robot_yy << std::endl << std::endl;

}

/* === Object Pose function === */
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

    float object_pitch = _object_pp/D2R;

    faceup_flag = true;

    switch(fsm_product_id){
    case 0:
        std::cout << "faceup case 0" << std::endl;
    case 1:

        std::cout << "faceup case 1" << std::endl;
        if( abs(object_pitch) <10){
            faceup_flag = false;
        }
        else{
            faceup_flag = true;
        }
        break;
    case 2:
        std::cout << "faceup case 2" << std::endl;
        if( abs(object_pitch) < 20){
            faceup_flag = false;
        }
        else{
            faceup_flag = true;
        }
        break;
    case 3:
        std::cout << "faceup case 3" << std::endl;
        if( abs(object_pitch) < 20){
            faceup_flag = false;
        }
        else{
            faceup_flag = true;
        }
        break;
    case 4:
        std::cout << "faceup case 4" << std::endl;
        if( abs(object_pitch) <10){
            faceup_flag = true;
        }
        else{
            faceup_flag = false;
        }
        break;
    case 5:
        std::cout << "faceup case 5" << std::endl;
        faceup_flag = true;
        break;
    default:
        std::cout << "faceup default" << std::endl;
        faceup_flag = true;
        break;
    }

    if(faceup_flag)
        std::cout << "FACE UP" << std::endl;
    else
        std::cout << "NOT FACE UP" << std::endl;



}

/* ==== Aruco marker Callbacks === */
void aruco30_callback(const aruco_msgs::MarkerArray& aurco_msg)
{
    if(detection_start_flag)
    { //debugging
        std::cout<< "aruco array size: " << aurco_msg.markers.size() << std::endl;

        //Set size
        array_size = aurco_msg.markers.size();

        for(int i = 0; i < array_size; i++)
        {
            aruco_data[i].id = aurco_msg.markers[i].id;
            aruco_data[i].cur_x = aurco_msg.markers[i].pose.pose.position.x;
            aruco_data[i].cur_y = aurco_msg.markers[i].pose.pose.position.y;
            aruco_data[i].cur_z = aurco_msg.markers[i].pose.pose.position.z;
            aruco_data[i].cur_w = aurco_msg.markers[i].pose.pose.orientation.w;
            aruco_data[i].cur_wx = aurco_msg.markers[i].pose.pose.orientation.x;
            aruco_data[i].cur_wy = aurco_msg.markers[i].pose.pose.orientation.y;
            aruco_data[i].cur_wz = aurco_msg.markers[i].pose.pose.orientation.z;

            //Get data from inventory Table
            for(int table_i = 0; table_i < table_size; table_i++)
            {
                if(aurco_msg.markers[i].id == inventory_table[table_i].aruco_id)
                {
                    aruco_data[i].object_id      = inventory_table[table_i].object_id   ;
                    aruco_data[i].grasp_case     = inventory_table[table_i].grasp_case  ;
                    aruco_data[i].expire_state   = inventory_table[table_i].expire_state;
                }
            }
        }
    }

    detection_start_flag = false;
}


/* ==== SHELF Aruco marker Call back === */
void aruco80_callback(const aruco_msgs::MarkerArray& aurco_msg)
{

    //debugging
    std::cout<< "aruco80_CALLBACK: " << aurco_msg.markers.size() << std::endl;

    if(shelf_detection_start_flag)
    {
        //debugging
        std::cout<< "shelf aruco array size: " << aurco_msg.markers.size() << std::endl;

        //Set size
        shelf_array_size = aurco_msg.markers.size();

        for(int i = 0; i < shelf_array_size; i++)
        {
            shelf_aruco_data[i].id      = aurco_msg.markers[i].id;
            shelf_aruco_data[i].cur_x = aurco_msg.markers[i].pose.pose.position.x;
            shelf_aruco_data[i].cur_y = aurco_msg.markers[i].pose.pose.position.y;
            shelf_aruco_data[i].cur_z = aurco_msg.markers[i].pose.pose.position.z;
            shelf_aruco_data[i].cur_w = aurco_msg.markers[i].pose.pose.orientation.w;
            shelf_aruco_data[i].cur_wx = aurco_msg.markers[i].pose.pose.orientation.x;
            shelf_aruco_data[i].cur_wy = aurco_msg.markers[i].pose.pose.orientation.y;
            shelf_aruco_data[i].cur_wz = aurco_msg.markers[i].pose.pose.orientation.z;

            std::cout << " shelf_aruco_data["<< i << "].id    = " << shelf_aruco_data[i].id     << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_x = " << shelf_aruco_data[i].cur_x  << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_y = " << shelf_aruco_data[i].cur_y  << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_z = " << shelf_aruco_data[i].cur_z  << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_w = " << shelf_aruco_data[i].cur_w  << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_wx= " << shelf_aruco_data[i].cur_wx << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_wy= " << shelf_aruco_data[i].cur_wy << std::endl;
            std::cout << " shelf_aruco_data["<< i << "].cur_wz= " << shelf_aruco_data[i].cur_wz << std::endl;
        }
    }

    shelf_detection_start_flag = false;
}


class ManipulationAction
{
public:
    ManipulationAction() : ac_("manipulation", true)
    {
        ROS_INFO("Waiting for action server to start.");
        ac_.waitForServer();
        ROS_INFO("Action server started.");
    }

    rb5_ros_wrapper::manipulationGoal goal_manipulation;
    ros::NodeHandle n;

    Client ac_;

    ///* === manipulation_result_callback callback === */
    //void manipulation_result_callback(const rb5_ros_wrapper::manipulationAction& manipulation_result){
    //    std::cout << "manipulation_result_callback called" << std::endl;
    ////    std::cout


    void doneCb(const actionlib::SimpleClientGoalState& state,
                const rb5_ros_wrapper::manipulationResultConstPtr& result)
    {
        ROS_INFO("Done CB");
        std::cout << "result: " << result->result_flag << std::endl;


        manipulation_result = true;

        if(result-> result_flag == 3)
        {
            std::cout <<"STATE ERROR OR NOT 2"  <<std::endl;
            manipulation_result = false;
        }
        else if(result-> result_flag == 4){
            std::cout << "RESULT: INPUT error" << std::endl;
            manipulation_result = false;
        }
        else if(result-> result_flag == 5){
            std::cout << "RESULT: 5 error STOP" << std::endl;
            manipulation_result = false;
        }
        else if(result-> result_flag == 6){
            std::cout << "RESULT: COLLISION" << std::endl;
            manipulation_result = false;
        }
        else{
            manipulation_result = true;
        }
    }

    void activeCb()
    {
    }

    void feedbackCb(const rb5_ros_wrapper::manipulationFeedbackConstPtr& feedback)
    {
    }

    void SendGoalandWait()
    {
        ac_.sendGoal(goal_manipulation, boost::bind(&ManipulationAction::doneCb, this, _1, _2), boost::bind(&ManipulationAction::activeCb, this), boost::bind(&ManipulationAction::feedbackCb, this, _1));
        ac_.waitForResult();


    }

protected:


};


/* ==== Motion_direction action class === */
class motion_directionAction
{
protected:
    ros::NodeHandle nh_motion_direction;
    actionlib::SimpleActionServer<wrs_fsm::motion_directionAction> asmotion_direction_;
    std::string action_name_;

    // create messages that are used to published feedback&result
    wrs_fsm::motion_directionFeedback feedback_;
    wrs_fsm::motion_directionResult result_;

    int goal_cmd = 0;

public:

    motion_directionAction(std::string name) :
        asmotion_direction_(nh_motion_direction, name, boost::bind(&motion_directionAction::executeCB, this, _1), false),
        action_name_(name)
    {
        asmotion_direction_.start();
    }

    ~motion_directionAction(void)
    {

    }


    /*Call Back function when goal is received from Action client*/
    void executeCB(const wrs_fsm::motion_directionGoalConstPtr &goal)
    {

//        std::cout <<"==========================="  << std::endl;
//        std::cout <<"GOAL CMD: "<< goal->motion_cmd << std::endl;
        ROS_INFO("========= FSM request ========");
        ROS_INFO("GOAL CMD: %d",goal->motion_cmd);

        //set global variable with FSM request info
        get_goal(goal);

        goal_cmd = goal->motion_cmd;
        motion_flag = false;

        //While loop waiting unitl the manipulation result recieved
        while(ros::ok())
        {
            ros::Duration(0.005).sleep();
            if(motion_flag) break;
        }

        motion_flag = false;
        result_.result_flag = 1;

        set_result();
        asmotion_direction_.setSucceeded(result_);
        reset_result();

    }


    //1 if alive, 0 else
    int returnServerStatus()
    {
        if(asmotion_direction_.isActive()) { return 1; }
        else { return 0; }
    }

    int getGoalCmd()
    {
        return goal_cmd;
    }

    void set_result()
    {
        result_.result_flag = motion_result;
    }

    void reset_result()
    {
        result_.result_flag = false;
    }

    void get_goal(const wrs_fsm::motion_directionGoalConstPtr &goal)
    {
        fsm_product_id = goal->object_id;
        shelf_id = goal->shelf_num;
        put_new = goal->put_new;

        switch(fsm_product_id){
        case 0:
        case 1:
        case 2:
        case 3:
            tool_id = TOOLMODE_GRIPPER;
            break;
        case 4:
        case 5:
            tool_id = TOOLMODE_SUCTION;
            break;
        }
    }
};

/*=== Shelf distance ===*/
int check_shelf_distance(int shelf_num)
{
    shelf_aruco_z;

    int dist_check_result;

    switch(shelf_num)
    {
    case 10:
        {
            std::cout << "SHELF ARUCO 10" << std::endl;
            if( shelf_aruco_z > 0.25 && shelf_aruco_z < 0.45){
                dist_check_result = true;
            }
            else
                dist_check_result = false;

            break;
        }
    case 11:
        {
            std::cout << "SHELF ARUCO 11" << std::endl;
            if( shelf_aruco_z > 0.2 && shelf_aruco_z < 0.35){
                dist_check_result = true;
            }
            else
                dist_check_result = false;
            break;
        }
    case 20:
        {
            std::cout << "SHELF ARUCO 20" << std::endl;
            if( shelf_aruco_z > 0.25 && shelf_aruco_z < 0.45){
                dist_check_result = true;
            }
            else
                dist_check_result = false;
            break;
        }
    case 21:
        {
            std::cout << "SHELF ARUCO 21" << std::endl;
            if( shelf_aruco_z > 0.2 && shelf_aruco_z < 0.4){
                dist_check_result = true;
            }
            else
                dist_check_result = false;
            break;
        }
    case 30:
        {
            std::cout << "SHELF ARUCO 30" << std::endl;
            if( shelf_aruco_z > 0.25 && shelf_aruco_z < 0.45){
                dist_check_result = true;
            }
            else
                dist_check_result = false;
            break;
        }
    case 31:
        {
            std::cout << "SHELF ARUCO 31" << std::endl;
            if( shelf_aruco_z > 0.2 && shelf_aruco_z < 0.4){
                dist_check_result = true;
            }
            else
                dist_check_result = false;
            break;
        }
    }

    std::cout << "distance result: " << dist_check_result << std::endl;
    return dist_check_result;
}

/* === Set Shelf tf as 100 ===*/
void setShelfTf(int shelf_num, int frontOrRare)
{
    shelf_aruco_x = 100.0;
    shelf_aruco_y = 100.0;
    shelf_aruco_z = 100.0;
    shelf_aruco_w = 1.0;
    shelf_aruco_wx= 0.0;
    shelf_aruco_wy= 0.0;
    shelf_aruco_wz= 0.0;
}

/* === Print item coordinates ===*/
void print_item_coordinates(int index)
{
    std::cout << "id: "              << item[index].id                   << std::endl;
    std::cout << "grasp_id: "        << item[index].grasp_case           << std::endl;
    std::cout << "(x,y,z) = ("       << item[index].cur_x     <<", "
                                     << item[index].cur_y     <<", "
                                     << item[index].cur_z     <<") "     << std::endl;
    std::cout << "(w,wx,wy,wz) = ("  << item[index].cur_w     <<", "
                                     << item[index].cur_wx    <<", "
                                     << item[index].cur_wy    <<", "
                                     << item[index].cur_wz    <<") "      << std::endl;
}

/* === Print Aruco marker ===*/
void print_aruco_coordinates(){
    std::cout << "------------------"        << std::endl;
    std::cout << "item_id  = "               << item_id      << std::endl;
    std::cout << "grasp_id = "               << grasp_id     << std::endl;
    std::cout << "------------------"        << std::endl;
    std::cout << "aruco (x,y,z) = ("         << aruco_x    << ", "
                                             << aruco_y    << ", "
                                             << aruco_z    << ")"  << std::endl;
    std::cout << "aruco (wx, wy, wz, w) = (" << aruco_wx   << ", "
                                             << aruco_wy   << ", "
                                             << aruco_wz   << ", "
                                             << aruco_w    << ") " << std::endl;
}

/* === Thread for topics ===*/
void* ThreadBroadCast(void *)
{
    //initialization
    aruco_x = aruco_y = aruco_z = 0.;
    aruco_wx = aruco_wy = aruco_wz = 0.; aruco_w = 1.;

    shelf_aruco_x = shelf_aruco_y = shelf_aruco_z = 0.;
    shelf_aruco_wx = shelf_aruco_wy = shelf_aruco_wz = 0.; shelf_aruco_w = 1.;

    //Publisher
    ros::NodeHandle nh_broadCast;
    ros::Publisher pb_broadcast = nh_broadCast.advertise<wrs_fsm::tf_broadcast>("marker_tf", 1);
    ros::Publisher pb_broadcast_shelf = nh_broadCast.advertise<wrs_fsm::tf_broadcast>("shelf_marker_tf", 1);

    wrs_fsm::tf_broadcast marker_tf;
    wrs_fsm::tf_broadcast shelf_marker_tf;

    //Assign current values
    marker_tf.x = aruco_x;
    marker_tf.y = aruco_y;
    marker_tf.z = aruco_z;
    marker_tf.w = aruco_w;
    marker_tf.wx = aruco_wx;
    marker_tf.wy = aruco_wy;
    marker_tf.wz = aruco_wz;

    shelf_marker_tf.x  = shelf_aruco_x;
    shelf_marker_tf.y  = shelf_aruco_y;
    shelf_marker_tf.z  = shelf_aruco_z;
    shelf_marker_tf.w  = shelf_aruco_w;
    shelf_marker_tf.wx = shelf_aruco_wx;
    shelf_marker_tf.wy = shelf_aruco_wy;
    shelf_marker_tf.wz = shelf_aruco_wz;

    //publish infintely
    ros::Rate rate(10.0);
    while(1)
    {
        marker_tf.x = aruco_x;
        marker_tf.y = aruco_y;
        marker_tf.z = aruco_z;
        marker_tf.w = aruco_w;
        marker_tf.wx = aruco_wx;
        marker_tf.wy = aruco_wy;
        marker_tf.wz = aruco_wz;

        shelf_marker_tf.x  = shelf_aruco_x;
        shelf_marker_tf.y  = shelf_aruco_y;
        shelf_marker_tf.z  = shelf_aruco_z;
        shelf_marker_tf.w  = shelf_aruco_w;
        shelf_marker_tf.wx = shelf_aruco_wx;
        shelf_marker_tf.wy = shelf_aruco_wy;
        shelf_marker_tf.wz = shelf_aruco_wz;

        pb_broadcast.publish(marker_tf);
        pb_broadcast_shelf.publish(shelf_marker_tf);

        rate.sleep();
    }
}


/* === main ===*/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "motion_director");

    //pthread: create a Pthread
    int threadID = pthread_create(&THREAD_t, NULL, &ThreadBroadCast, NULL);
    if(threadID < 0)
    {
        ROS_ERROR("Create Thread Error..");
    }

    //read file: inventory Table
    FILE *fpTable = NULL;
    fpTable = fopen("/home/rainbow/catkin_ws/src/wrs_fsm/inventory_table.txt", "r");
    if(fpTable == NULL){
        std::cout << ">>> item_table File Open Error..!!" << std::endl;
    }else{
        std::cout << ">>> item_table File Open Success..!!" << std::endl;
        for(int i=0; i<table_size; i++){
            fscanf(fpTable, "%d %d %d %d %c\n", &inventory_table[i].aruco_id, &inventory_table[i].object_id,&inventory_table[i].grasp_case,&inventory_table[i].expire_state,&inventory_table[i].character);
            //std::cout << inventory_table[i].aruco_id << "     " << inventory_table[i].object_id << "     " << inventory_table[i].grasp_case <<  "     " << inventory_table[i].expire_state << "     " << inventory_table[i].character << std::endl;
        }
        fclose(fpTable);
    }

    /* ============== action settings==============  */
    motion_directionAction motion_direction("motion_direction");
    ManipulationAction ac_manipulation;

    ROS_INFO("Starting motion_director module");

    // create the action client
//    actionlib::SimpleActionClient<wrs_fsm::fsm_motionAction> ac_fsm_motion("fsm_motion", true);
//    actionlib::SimpleActionClient<rb5_ros_wrapper::manipulationAction> ac_manipulation("manipulation", true);

    // wait for the action server to start
    ROS_INFO("Waiting for action server to start.");
//    ac_manipulation.waitForServer(); //will wait for infinite time

    // send a goal to the action
    ROS_INFO("Action server started, sending goal.");
//    rb5_ros_wrapper::manipulationGoal goal_manipulation;

    /* ============== aruco_ros 30mm subscriber ==============  */
    ros::Subscriber aruco30_sub, aruco80_sub, robot_state_sub;
    ros::NodeHandle n;
    aruco30_sub = n.subscribe("/aruco_marker_size030/markers", 1, aruco30_callback);
    aruco80_sub = n.subscribe("/aruco_marker_size080/markers", 1, aruco80_callback);
    robot_state_sub = n.subscribe("/robot_states", 1, robot_states_callback);
    //manipulation_result_sub = n.subscribe("/manipulation/result", 1, manipulation_result_callback);


    //sleep rate declaration(can't go before node handler ex) ros::NodeHandle n;)
    ros::Rate loop_rate(10);
    ros::Rate detection_rate(10);

    /* === Subscribing and sending actions === */
    while(ros::ok())
    {
        if(motion_direction.returnServerStatus())
        {
            if(motion_flag == false)
            {
                switch(motion_direction.getGoalCmd())
                {
                case MOTION_NONE:
                {
                    motion_flag = true;
                    motion_result = true;
                    break;
                }
                case MOTION_SHELFOUT:
                {
                    std::cout << "=== MOTION_SHELFOUT ===" << std::endl;

                    int temp_id = 0;

                    //1 Search Pose
                    ac_manipulation.goal_manipulation.motion_cmd    = PULL_SHELF;
                    ac_manipulation.goal_manipulation.sub_cmd       = 0;                //sub_cmd: search motion for PULL_SHELF
                    ac_manipulation.goal_manipulation.shelf_id      = shelf_id;
                    ac_manipulation.goal_manipulation.object_id     = fsm_product_id;

                    ac_manipulation.SendGoalandWait();

                    if(manipulation_result == false){
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;
                        break;
                    }


                    ///wait for the action to return
//                    if(waitForResult(ac_manipulation));
//                    else break;

                    ///sleep a half sec before detecting marker:
                    ros::spinOnce();                                    // Dummy spin
                    usleep(2*1000*1000);                              // sleep for a half second


                    int shelf_cnt = 0;                                  //count only when the desired shelf is detected
                    int shelf_marker_id = (shelf_id+1)*10;              //ex) 1st floor, marker id = 10. shelf_id = 0

                    for(int aruco_i =0 ; aruco_i < 3 && shelf_cnt == 0; aruco_i++)
                    {

                        std::cout << "front shelf marker detection try: " << aruco_i << std::endl;

                        //2 Aruco Marker detection
                        array_size = 0;
                        detection_start_flag = true;
                        shelf_detection_start_flag = true;
                        ros::spinOnce();                                    //aruco marker subscribe

                        ///update ArucoMarker TF
                        for(int i =0; i < array_size; i++)
                        {
                            if(aruco_data[i].id == shelf_marker_id)
                            {
                                shelf_aruco_x = aruco_data[i].cur_x ;
                                shelf_aruco_y = aruco_data[i].cur_y ;
                                shelf_aruco_z = aruco_data[i].cur_z ;
                                shelf_aruco_w = aruco_data[i].cur_w ;
                                shelf_aruco_wx= aruco_data[i].cur_wx;
                                shelf_aruco_wy= aruco_data[i].cur_wy;
                                shelf_aruco_wz= aruco_data[i].cur_wz;

                                temp_id = aruco_data[i].id;

                                shelf_cnt++;
                            }
                        }

                        if(shelf_cnt > 0){
                            if(check_shelf_distance(shelf_id)){
                                std::cout << "shelf in Safe Distance" << std::endl;
                            }
                            else{
                                ROS_ERROR("WRONG DISTANCE!! TOO CLOSE!!");
                                shelf_cnt = 0;
                            }
                        }

                        //if the desired shelf is not detected
                        if(shelf_cnt == 0 || array_size == 0){
                            std::cout << "Desired shelf is not detected" << std::endl;
                            setShelfTf(shelf_id,0);                         //if shelf marker is not found, assigned known values
                            motion_result = false;
                            target_detected_flag = false;
                        }else{
                            motion_result = true;
                            target_detected_flag = true;
                        }

                        detection_rate.sleep();
                    }



                    if(target_detected_flag == false)
                    {
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        target_detected_flag == false;
                        break;
                    }



                    std::cout << "shelf aruco before pull: "    << std::endl;
                    std::cout << "shelf aruco id: " << temp_id<< std::endl;
                    std::cout << "shelf_aruco_x : " << shelf_aruco_x  << std::endl;
                    std::cout << "shelf_aruco_y : " << shelf_aruco_y  << std::endl;
                    std::cout << "shelf_aruco_z : " << shelf_aruco_z  << std::endl;
                    std::cout << "shelf_aruco_w : " << shelf_aruco_w  << std::endl;
                    std::cout << "shelf_aruco_wx: " << shelf_aruco_wx << std::endl;
                    std::cout << "shelf_aruco_wy: " << shelf_aruco_wy << std::endl;
                    std::cout << "shelf_aruco_wz: " << shelf_aruco_wz << std::endl;

                    //3 Pull motion
                    ac_manipulation.goal_manipulation.motion_cmd = PULL_SHELF;
                    ac_manipulation.goal_manipulation.sub_cmd    = 1;                   //sub_cmd: pull motion for PULL_SHELF
                    ac_manipulation.goal_manipulation.shelf_id   = shelf_id;
                    ac_manipulation.goal_manipulation.object_id  = fsm_product_id;

                    ac_manipulation.SendGoalandWait();
//                    /// wait for the action to return
//                    if(waitForResult(ac_manipulation));
//                    else break;

                    if(manipulation_result == false){
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;
                        break;
                    }

                    ///sleep a half sec before detecting marker:
                    ///ros::Duration(0.5).sleep();
                    usleep(5.0*1000*1000);                              // sleep for one and a half second

                    shelf_cnt = 0;
                    //4 shelf Aruco Marker detection & update
                    for(int aruco_i =0 ; aruco_i < 3 && shelf_cnt == 0; aruco_i++)
                    {

                        std::cout << "back shelf marker detection try: " << aruco_i << std::endl;

                        array_size = 0;
                        detection_start_flag = true;
                        shelf_detection_start_flag = true;
                        ros::spinOnce();                                    //for aruco marker subscribe

                        ///update ArucoMarker TF
//                        shelf_marker_id = (shelf_id+1)*10;
                        shelf_marker_id = back_shelf_array[shelf_id];
                        for(int i =0; i < array_size; i++)
                        {
//                            if(aruco_data[i].id == shelf_marker_id + 1)
                            if(aruco_data[i].id == shelf_marker_id)
                            {
                                shelf_aruco_x = shelf_aruco_data[i].cur_x ;
                                shelf_aruco_y = shelf_aruco_data[i].cur_y ;
                                shelf_aruco_z = shelf_aruco_data[i].cur_z ;
                                shelf_aruco_w = shelf_aruco_data[i].cur_w ;
                                shelf_aruco_wx= shelf_aruco_data[i].cur_wx;
                                shelf_aruco_wy= shelf_aruco_data[i].cur_wy;
                                shelf_aruco_wz= shelf_aruco_data[i].cur_wz;


                                temp_id = aruco_data[i].id;

                                shelf_cnt++;
                            }
                        }



//                        if(shelf_cnt > 0){
//                            if(check_shelf_distance(shelf_id)){
//                                std::cout << "shelf in Safe Distance" << std::endl;
//                            }
//                            else{
//                                ROS_ERROR("WRONG DISTANCE!! TOO CLOSE!!");
//                                shelf_cnt = 0;
//                            }
//                        }

                        ///if the desired shelf is not detected
                        if(shelf_cnt == 0 || array_size == 0){
                            std::cout << "Desired shelf is not detected" << std::endl;
                            setShelfTf(shelf_id,1);                         //if shelf marker is not found, assigned known values
                            motion_result = false;
                            target_detected_flag = false;
                        }
                        else
                        {
                            target_detected_flag = true;
                        }


                        detection_rate.sleep();
                    }


                    if(target_detected_flag == false)
                    {
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        target_detected_flag == false;
                        break;
                    }


                    //PRINTING DEBUG
                    std::cout << "shelf aruco id: " << temp_id  << std::endl;
                    std::cout << "shelf_aruco_x : " << shelf_aruco_x  << std::endl;
                    std::cout << "shelf_aruco_y : " << shelf_aruco_y  << std::endl;
                    std::cout << "shelf_aruco_z : " << shelf_aruco_z  << std::endl;
                    std::cout << "shelf_aruco_w : " << shelf_aruco_w  << std::endl;
                    std::cout << "shelf_aruco_wx: " << shelf_aruco_wx << std::endl;
                    std::cout << "shelf_aruco_wy: " << shelf_aruco_wy << std::endl;
                    std::cout << "shelf_aruco_wz: " << shelf_aruco_wz << std::endl;

                    //5 Shelf aruco update
                    ac_manipulation.goal_manipulation.motion_cmd  = DETECT_SHELF;
                    ac_manipulation.goal_manipulation.sub_cmd     = 0;                  //sub_cmd: pulled-Shelf search motion for PULL_SHELF
                    ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                    ac_manipulation.goal_manipulation.object_id   = fsm_product_id;

                    ac_manipulation.SendGoalandWait();

                    if(manipulation_result == false){
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;
                        break;
                    }
//                    /// wait for the action to return
//                    if(waitForResult(ac_manipulation));
//                    else break;


                    motion_result = true;
                    motion_flag = true;
                    break;
                }
                case MOTION_SHELFIN:
                {
                    std::cout << "=== MOTION_SHELFIN ===" << std::endl;

                    //1 Push motion
                    ac_manipulation.goal_manipulation.motion_cmd  = PUSH_SHELF;
                    ac_manipulation.goal_manipulation.sub_cmd     = 0;              //Default
                    ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                    ac_manipulation.goal_manipulation.shelf_id    = shelf_id;

                    ac_manipulation.SendGoalandWait();

                    if(manipulation_result == false){
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;
                        break;
                    }
//                    /// wait for the action to return
//                    if(waitForResult(ac_manipulation));
//                    else break;

                    motion_flag = true;
                    motion_result = true;
                    break;
                }
                case MOTION_DETECT_ONSHELF:
                {
                    std::cout << "=== MOTION_DETECT_ONSHELF ===" << std::endl;

                    // variables
                    int item_cnt = 0;   //count for only the desired items
                    motion_result = true;

                    for(int detect_point = 0; detect_point < 3 && item_cnt == 0; detect_point++)
                    {
                        std::cout << "*** detect point: " << detect_point << std::endl;
                        item_cnt = 0;

                        //1 Detect pose
                        ac_manipulation.goal_manipulation.motion_cmd  = OBJECT_SEARCH;
                        ac_manipulation.goal_manipulation.sub_cmd     = detect_point;//0;
                        ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                        ac_manipulation.goal_manipulation.object_id   = fsm_product_id;

                        ac_manipulation.SendGoalandWait();

                        if(manipulation_result == false){
                            ROS_ERROR("break!");
                            motion_result = false;
                            motion_flag = true;
                            manipulation_result = true;
                            break;
                        }

                        //sleep a half sec before detecting marker:
                        ros::Duration(0.5).sleep();           // sleep for a half second

                        //2 Aruco Marker detection
                        for(int aruco_i =0 ; aruco_i < 3 && item_cnt == 0; aruco_i++)
                        {
                            array_size = 0;
                            detection_start_flag = true;
                            ros::spinOnce();  //for aruco marker subscribe

                            ///Look for EXPIRED items, except LunchBox. Lunchbox looks for expried and old both
                            if(fsm_product_id == LUNCHBOX){
                                for(int i = 0; i < array_size; i++)
                                {
                                    if( aruco_data[i].object_id == fsm_product_id && aruco_data[i].expire_state == EXPIRED)
                                    {
                                        std::cout << "*** EXPIRE FOUND ***" << std::endl;
                                        item[item_cnt].object_id  = aruco_data[i].object_id;
                                        item[item_cnt].grasp_case = aruco_data[i].grasp_case;
                                        item[item_cnt].expire_state= aruco_data[i].expire_state;
                                        item[item_cnt].cur_x      = aruco_data[i].cur_x ;
                                        item[item_cnt].cur_y      = aruco_data[i].cur_y ;
                                        item[item_cnt].cur_z      = aruco_data[i].cur_z ;
                                        item[item_cnt].cur_w      = aruco_data[i].cur_w ;
                                        item[item_cnt].cur_wx     = aruco_data[i].cur_wx;
                                        item[item_cnt].cur_wy     = aruco_data[i].cur_wy;
                                        item[item_cnt].cur_wz     = aruco_data[i].cur_wz;

                                        print_item_coordinates(item_cnt);

                                        item_cnt++;
                                    }else if( aruco_data[i].object_id == fsm_product_id && aruco_data[i].expire_state == OLD)
                                    {
                                        std::cout << "*** OLD FOUND ***" << std::endl;
                                        item[item_cnt].object_id  = aruco_data[i].object_id ;
                                        item[item_cnt].grasp_case = aruco_data[i].grasp_case ;
                                        item[item_cnt].expire_state = aruco_data[i].expire_state;
                                        item[item_cnt].cur_x      = aruco_data[i].cur_x ;
                                        item[item_cnt].cur_y      = aruco_data[i].cur_y ;
                                        item[item_cnt].cur_z      = aruco_data[i].cur_z ;
                                        item[item_cnt].cur_w      = aruco_data[i].cur_w ;
                                        item[item_cnt].cur_wx     = aruco_data[i].cur_wx;
                                        item[item_cnt].cur_wy     = aruco_data[i].cur_wy;
                                        item[item_cnt].cur_wz     = aruco_data[i].cur_wz;

                                        print_item_coordinates(item_cnt);

                                        item_cnt++;
                                    }
                                }
                            }
                            else{
                                for(int i = 0; i < array_size; i++)
                                {
                                    if( aruco_data[i].object_id == fsm_product_id && aruco_data[i].expire_state == EXPIRED)
                                    {
                                        item[item_cnt].object_id  = aruco_data[i].object_id ;
                                        item[item_cnt].grasp_case = aruco_data[i].grasp_case ;
                                        item[item_cnt].expire_state = aruco_data[i].expire_state;
                                        item[item_cnt].cur_x      = aruco_data[i].cur_x ;
                                        item[item_cnt].cur_y      = aruco_data[i].cur_y ;
                                        item[item_cnt].cur_z      = aruco_data[i].cur_z ;
                                        item[item_cnt].cur_w      = aruco_data[i].cur_w ;
                                        item[item_cnt].cur_wx     = aruco_data[i].cur_wx;
                                        item[item_cnt].cur_wy     = aruco_data[i].cur_wy;
                                        item[item_cnt].cur_wz     = aruco_data[i].cur_wz;

                                        print_item_coordinates(item_cnt);

                                        item_cnt++;
                                    }
                                }
                            }


                            detection_rate.sleep();
                        }
                    }

                    if(item_cnt == 0){
                        motion_result = false;      //couldn't find the olddest item
                        ROS_INFO("SEARCH RESULT: Could NOT find EXPIRED product");

                        if(fsm_product_id == 4)
                        {
                            motion_result = FALLDOWN;   //assuming this product is fall down
                        }
                    }
                    else{ // FOUND Desired ITEM
                        motion_result = true;


                        //assign aruco_marker data
                        item_id  = item[0].object_id;
                        grasp_id = item[0].grasp_case;
                        aruco_x  = item[0].cur_x;
                        aruco_y  = item[0].cur_y;
                        aruco_z  = item[0].cur_z;
                        aruco_wx = item[0].cur_wx;
                        aruco_wy = item[0].cur_wy;
                        aruco_wz = item[0].cur_wz;
                        aruco_w  = item[0].cur_w;


                        object_pose();

                        if(faceup_flag){
                            std::cout << "FACE UP STATE" << std::endl;
                            motion_result = true;
                            motion_flag = true;                         //Send fsm done to finish current state and move to the next state.
                        }
                        else{
                            std::cout << "NOT FACE UP STATE" << std::endl;
                            ROS_ERROR("FALLDOWN!");
                            motion_result = FALLDOWN; //false;
                            motion_flag = true;
                            manipulation_result = true;
                            break;
                        }
                    }

                    motion_flag = true;
                    break;
                }
                case MOTION_DISPOSE:
                {
                    std::cout << "=== MOTION_DISPOSE ===" << std::endl;

                    motion_result = true;

                    //assign aruco_marker data
                    item_id  = item[0].object_id;
                    grasp_id = item[0].grasp_case;
                    aruco_x  = item[0].cur_x;
                    aruco_y  = item[0].cur_y;
                    aruco_z  = item[0].cur_z;
                    aruco_wx = item[0].cur_wx;
                    aruco_wy = item[0].cur_wy;
                    aruco_wz = item[0].cur_wz;
                    aruco_w  = item[0].cur_w;


                    //PRINTING DEBUG
                    std::cout << "Before Transform: " << std::endl;
                    print_aruco_coordinates();

                    aruco2grasp_coordinate();

                    //PRINTING DEBUG
                    std::cout << "After Transform: " << std::endl;
                    print_aruco_coordinates();

                    //1 grasp
                    ac_manipulation.goal_manipulation.motion_cmd  = GRASP_OBJECT;
                    ac_manipulation.goal_manipulation.sub_cmd     = EXPIRED;
                    ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                    ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                    ac_manipulation.goal_manipulation.tool_id     = tool_id;

                    ac_manipulation.SendGoalandWait();
                    if(manipulation_result == false){
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;

                        //Send Home pose
                        ac_manipulation.goal_manipulation.motion_cmd  = HOME_POSE;
                        ac_manipulation.goal_manipulation.sub_cmd     = EXPIRED;
                        ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                        ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                        ac_manipulation.goal_manipulation.tool_id     = tool_id;
                        ac_manipulation.SendGoalandWait();

                        break;
                    }
//                    /// wait for the action to return
//                    if(waitForResult(ac_manipulation));
//                    else break;


                    //2 put
                    ac_manipulation.goal_manipulation.motion_cmd  = PUT_OBJECT;
                    ac_manipulation.goal_manipulation.sub_cmd     = 0;
                    ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                    ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                    ac_manipulation.goal_manipulation.tool_id     = tool_id;

                    //Special case: Lunchbox --> different put position
                    if(fsm_product_id == LUNCHBOX)
                    {
                        if(item[0].expire_state == OLD){
                            ac_manipulation.goal_manipulation.sub_cmd     = 4;
                            std::cout << "*** GO TO TEMP PLACE ***" << std::endl;
                        }
                        else if(item[0].expire_state == EXPIRED){
                            ac_manipulation.goal_manipulation.sub_cmd     = 0;
                            std::cout << "*** GO TO DISPOSAL PLACE ***" << std::endl;
                        }
                    }

                    ac_manipulation.SendGoalandWait();
                    if(manipulation_result == false){
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;
                        break;
                    }


                    motion_flag = true;
                    motion_result = true;
                    break;
                }
                case MOTION_ALIGN_OLD:
                {
                    std::cout << "=== MOTION_ALIGN_OLD ===" << std::endl;

                    // variables
                    int item_cnt = 0;   //count for only the desired items
                    old_fail = 0;

                    //I.Detection
                    for(int detect_point = 0; detect_point < 3 && item_cnt == 0; detect_point++)
                    {
                        std::cout << "*** detect point: " << detect_point << std::endl;
                        item_cnt = 0;

                        //1 Detect pose
                        ac_manipulation.goal_manipulation.motion_cmd  = OBJECT_SEARCH;
                        ac_manipulation.goal_manipulation.sub_cmd     = detect_point;            //Search at point 0
                        ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                        ac_manipulation.goal_manipulation.object_id   = fsm_product_id;

                        ac_manipulation.SendGoalandWait();
                        if(manipulation_result == false){
                            ROS_ERROR("break!");
                            motion_result = false;
                            motion_flag = true;
                            manipulation_result = true;
                            break;
                        }
//                        /// wait for the action to return
//                        if(waitForResult(ac_manipulation));
//                        else break;

                        usleep(0.5*1000*1000);                                  // sleep for a half second

                        //2 Aruco Marker detection
                        for(int aruco_i =0 ; aruco_i < 3 && item_cnt == 0; aruco_i++)
                        {
                            array_size = 0;
                            detection_start_flag = true;
                            ros::spinOnce();  //for aruco marker subscribe

                            std::cout << "array_size = " << array_size << std::endl;

                            if(fsm_product_id == LUNCHBOX){
                                for(int i =0; i < array_size; i++)
                                {
                                    if( aruco_data[i].object_id == fsm_product_id && aruco_data[i].expire_state == EXPIRED)
                                    {
                                        std::cout << "*** EXPIRED FOUND ***" << std::endl;
                                        item[item_cnt].object_id  = aruco_data[i].object_id ;
                                        item[item_cnt].grasp_case  = aruco_data[i].grasp_case ;
                                        item[item_cnt].expire_state = aruco_data[i].expire_state;
                                        item[item_cnt].cur_x      = aruco_data[i].cur_x ;
                                        item[item_cnt].cur_y      = aruco_data[i].cur_y ;
                                        item[item_cnt].cur_z      = aruco_data[i].cur_z ;
                                        item[item_cnt].cur_w      = aruco_data[i].cur_w ;
                                        item[item_cnt].cur_wx     = aruco_data[i].cur_wx;
                                        item[item_cnt].cur_wy     = aruco_data[i].cur_wy;
                                        item[item_cnt].cur_wz     = aruco_data[i].cur_wz;

                                        print_item_coordinates(item_cnt);

                                        item_cnt++;
                                    }
                                    else if(aruco_data[i].object_id == fsm_product_id && aruco_data[i].expire_state == OLD)
                                    {
                                        std::cout << "*** OLD FOUND ***" << std::endl;
                                        item[item_cnt].object_id  = aruco_data[i].object_id ;
                                        item[item_cnt].grasp_case  = aruco_data[i].grasp_case ;
                                        item[item_cnt].expire_state = aruco_data[i].expire_state;
                                        item[item_cnt].cur_x      = aruco_data[i].cur_x ;
                                        item[item_cnt].cur_y      = aruco_data[i].cur_y ;
                                        item[item_cnt].cur_z      = aruco_data[i].cur_z ;
                                        item[item_cnt].cur_w      = aruco_data[i].cur_w ;
                                        item[item_cnt].cur_wx     = aruco_data[i].cur_wx;
                                        item[item_cnt].cur_wy     = aruco_data[i].cur_wy;
                                        item[item_cnt].cur_wz     = aruco_data[i].cur_wz;

                                        print_item_coordinates(item_cnt);

                                        item_cnt++;
                                    }
                                }
                            }
                            else{
                                for(int i =0; i < array_size; i++)
                                {
                                    if( aruco_data[i].object_id == fsm_product_id && aruco_data[i].expire_state == OLD)
                                    {
                                        item[item_cnt].object_id  = aruco_data[i].object_id ;
                                        item[item_cnt].grasp_case  = aruco_data[i].grasp_case ;
                                        item[item_cnt].expire_state = aruco_data[i].expire_state;
                                        item[item_cnt].cur_x      = aruco_data[i].cur_x ;
                                        item[item_cnt].cur_y      = aruco_data[i].cur_y ;
                                        item[item_cnt].cur_z      = aruco_data[i].cur_z ;
                                        item[item_cnt].cur_w      = aruco_data[i].cur_w ;
                                        item[item_cnt].cur_wx     = aruco_data[i].cur_wx;
                                        item[item_cnt].cur_wy     = aruco_data[i].cur_wy;
                                        item[item_cnt].cur_wz     = aruco_data[i].cur_wz;

                                        print_item_coordinates(item_cnt);

                                        item_cnt++;
                                    }
                                }
                            }

                            detection_rate.sleep();
                        }

                    }
                    //II.ALIGN OR NOT
                    if(item_cnt == 0){
                        motion_result = false;                                          //couldn't find the olddest item
                        ROS_INFO(" *** SEARCH RESULT: Could NOT find OLD product");

                        old_fail = 1;

                        if(fsm_product_id == 4)
                        {
                            motion_result = FALLDOWN;   //assuming this product is fall down
                            motion_flag = true;
                            break;
                        }
                    }
                    else{

                        // 3-0 tf setting
                        //update aruco coordinate to update tf
                        item_id  = item[0].object_id;
                        grasp_id = item[0].grasp_case;
                        aruco_x  = item[0].cur_x;
                        aruco_y  = item[0].cur_y;
                        aruco_z  = item[0].cur_z;
                        aruco_wx = item[0].cur_wx;
                        aruco_wy = item[0].cur_wy;
                        aruco_wz = item[0].cur_wz;
                        aruco_w  = item[0].cur_w;


                        object_pose();
                        if(faceup_flag){
                            std::cout << "FACE UP STATUS" << std::endl;
                            motion_result = true;


                            //PRINTING DEBUG
                            std::cout << "Before Transform: " << std::endl;
                            print_aruco_coordinates();

                            //Transform
                            aruco2grasp_coordinate();
                            usleep(1000*1000);

                            //PRINTING DEBUG
                            std::cout << "After Transform: " << std::endl;
                            print_aruco_coordinates();

                            //3 grasp
                            ac_manipulation.goal_manipulation.motion_cmd  = GRASP_OBJECT;
                            ac_manipulation.goal_manipulation.sub_cmd     = OLD;          //Default
                            ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                            ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                            ac_manipulation.goal_manipulation.tool_id     = tool_id;

                            ac_manipulation.SendGoalandWait();


                            if(manipulation_result == false){
                                ROS_ERROR("break!");
                                motion_result = false;
                                motion_flag = true;
                                manipulation_result = true;
                                break;
                            }

                            //4 put
                            ac_manipulation.goal_manipulation.motion_cmd  = PUT_OBJECT;
                            ac_manipulation.goal_manipulation.sub_cmd     = 1;          //put position = 1
                            ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                            ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                            ac_manipulation.goal_manipulation.tool_id     = tool_id;


                            //If the object is LunchBOX
                            if(fsm_product_id == LUNCHBOX)
                            {
                                if(item[0].expire_state == OLD){
                                    std::cout << "*** GO TO TEMP PLACE ***" << std::endl;
                                    ac_manipulation.goal_manipulation.sub_cmd     = 4;
                                }
                                else if(item[0].expire_state == EXPIRED){
                                    std::cout << "*** GOTO DISPOSAL PLACE ***" << std::endl;
                                    ac_manipulation.goal_manipulation.sub_cmd     = 0;
                                }

                            }

                            ac_manipulation.SendGoalandWait();
                            if(manipulation_result == false){
                                ROS_ERROR("PUT OLD OBJECT FAIL!");
                                old_fail = 1;
                                motion_result = false;
                                motion_flag = true;
                                manipulation_result = true;
                                break;
                            }
                        }

                        else{
                            std::cout << "NOT FACE UP STATE" << std::endl;
                            ROS_ERROR("FALLDOWN");
                            motion_result = FALLDOWN;//false;
                            motion_flag = true;
                            break;
                        }

                    }

                    motion_flag = true;
                    break;
                }
                case MOTION_SEARCH_NEW:
                {
                    std::cout << "=== MOTION_SEARCH_NEW ===" << std::endl;

                    // variables
                    int item_cnt = 0;   //count for only the desired items

                    for(int detect_point = 0; detect_point < 2 && item_cnt == 0; detect_point++)
                    {
                        std::cout << "***detect point: " << detect_point << std::endl;
                        item_cnt = 0;

                        //1 Detect pose
                        ac_manipulation.goal_manipulation.motion_cmd  = OBJECT_SEARCH;  //DETECT_BASKET;     //search Basket
                        ac_manipulation.goal_manipulation.sub_cmd     = detect_point;   //search at detect point
                        ac_manipulation.goal_manipulation.shelf_id    = 3;              //Search Target = Basket
                        ac_manipulation.goal_manipulation.object_id   = fsm_product_id;

                        ///EXCEPTION: LUNCH BOX --> Set parameters again
                        if(fsm_product_id == LUNCHBOX && put_new == 1)
                        {
                            ac_manipulation.goal_manipulation.motion_cmd  = OBJECT_SEARCH;  //DETECT_BASKET;
                            ac_manipulation.goal_manipulation.sub_cmd     = detect_point;
                            ac_manipulation.goal_manipulation.shelf_id    = 4;              //Search Target = temp. position
                            ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                        }

                        ac_manipulation.SendGoalandWait();
                        if(manipulation_result == false){
                            ROS_ERROR("break!");
                            motion_result = false;
                            motion_flag = true;
                            manipulation_result = true;
                            break;
                        }
//                        /// wait for the action to return
//                        if(waitForResult(ac_manipulation));
//                        else break;

                        //sleep a half sec before detecting marker:
                        usleep(0.5*1000*1000);                              //sleep before grasp motion

                        //2 Aruco Marker detection
                        for(int detect_point = 0; detect_point < 3 && item_cnt == 0; detect_point++)
                        {
                            array_size = 0;
                            detection_start_flag = true;
                            ros::spinOnce();  //for aruco marker subscribe

                            std::cout << "array_size = " << array_size << std::endl;

                            if(fsm_product_id == LUNCHBOX)
                            {
                                if(put_new == 0){
                                    for(int i =0; i < array_size; i++)
                                    {
                                        if(aruco_data[i].object_id == fsm_product_id)
                                        {
                                            if(aruco_data[i].expire_state == NEW)
                                            {
                                                std::cout << "*** NEW FOUND ***" << std::endl;
                                                item[item_cnt].object_id    = aruco_data[i].object_id ;
                                                item[item_cnt].grasp_case   = aruco_data[i].grasp_case ;
                                                item[item_cnt].cur_x        = aruco_data[i].cur_x ;
                                                item[item_cnt].cur_y        = aruco_data[i].cur_y ;
                                                item[item_cnt].cur_z        = aruco_data[i].cur_z ;
                                                item[item_cnt].cur_w        = aruco_data[i].cur_w ;
                                                item[item_cnt].cur_wx       = aruco_data[i].cur_wx;
                                                item[item_cnt].cur_wy       = aruco_data[i].cur_wy;
                                                item[item_cnt].cur_wz       = aruco_data[i].cur_wz;

                                                item_cnt++;
                                            }
                                        }
                                    }
                                }else if(put_new == 1){
                                    for(int i =0; i < array_size; i++)
                                    {
                                        if(aruco_data[i].object_id == fsm_product_id)
                                        {
                                            if(aruco_data[i].expire_state == OLD)
                                            {
                                                std::cout << "*** OLD FOUND ***" << std::endl;
                                                item[item_cnt].object_id    = aruco_data[i].object_id ;
                                                item[item_cnt].grasp_case   = aruco_data[i].grasp_case ;
                                                item[item_cnt].cur_x        = aruco_data[i].cur_x ;
                                                item[item_cnt].cur_y        = aruco_data[i].cur_y ;
                                                item[item_cnt].cur_z        = aruco_data[i].cur_z ;
                                                item[item_cnt].cur_w        = aruco_data[i].cur_w ;
                                                item[item_cnt].cur_wx       = aruco_data[i].cur_wx;
                                                item[item_cnt].cur_wy       = aruco_data[i].cur_wy;
                                                item[item_cnt].cur_wz       = aruco_data[i].cur_wz;

                                                item_cnt++;
                                            }
                                        }
                                    }
                                }
                            }
                            else{
                                for(int i =0; i < array_size; i++)
                                {
                                    if(aruco_data[i].object_id == fsm_product_id)
                                    {
                                        if(aruco_data[i].expire_state == NEW || aruco_data[i].expire_state == NEW2 )
                                        {
                                            item[item_cnt].object_id    = aruco_data[i].object_id ;
                                            item[item_cnt].grasp_case   = aruco_data[i].grasp_case ;
                                            item[item_cnt].cur_x        = aruco_data[i].cur_x ;
                                            item[item_cnt].cur_y        = aruco_data[i].cur_y ;
                                            item[item_cnt].cur_z        = aruco_data[i].cur_z ;
                                            item[item_cnt].cur_w        = aruco_data[i].cur_w ;
                                            item[item_cnt].cur_wx       = aruco_data[i].cur_wx;
                                            item[item_cnt].cur_wy       = aruco_data[i].cur_wy;
                                            item[item_cnt].cur_wz       = aruco_data[i].cur_wz;

                                            item_cnt++;
                                        }
                                    }
                                }

                            }

                            detection_rate.sleep();
                        }

                    }

                    if(item_cnt == 0){
                        motion_result = false;                                          //couldn't find the olddest item
                        ROS_INFO("SEARCH RESULT: Could NOT find NEW product");
                    }
                    else{
                        motion_result = true;
                    }

                    motion_flag = true;
                    break;
                }
                case MOTION_DISPLAY_NEW:
                {
                    std::cout << "=== DISPLAY_NEW ===" << std::endl;

                    item_id  = item[0].object_id;
                    grasp_id = item[0].grasp_case;
                    aruco_x  = item[0].cur_x;
                    aruco_y  = item[0].cur_y;
                    aruco_z  = item[0].cur_z;
                    aruco_wx = item[0].cur_wx;
                    aruco_wy = item[0].cur_wy;
                    aruco_wz = item[0].cur_wz;
                    aruco_w  = item[0].cur_w;


                    object_pose();
                    if(faceup_flag){
                        motion_result = true;

                        //PRINTING DEBUG
                        std::cout << "Before Transform: " << std::endl;
                        print_aruco_coordinates();

                        aruco2grasp_coordinate();
                        usleep(1000*1000);                              //sleep before grasp motion

                        //PRINTING DEBUG
                        std::cout << "After Transform: " << std::endl;
                        print_aruco_coordinates();

                        //1 grasp
                        ac_manipulation.goal_manipulation.motion_cmd  = GRASP_OBJECT;
                        ac_manipulation.goal_manipulation.sub_cmd     = NEW;
                        ac_manipulation.goal_manipulation.shelf_id    = 3;              //Basket
                        ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                        ac_manipulation.goal_manipulation.tool_id     = tool_id;

                        if(put_new ==1)
                            ac_manipulation.goal_manipulation.sub_cmd     = NEW2;


                        ///EXCEPTION: LUNCHBOX
                        if(fsm_product_id == LUNCHBOX && put_new == 1)
                        {
                            ac_manipulation.goal_manipulation.motion_cmd  = GRASP_OBJECT;   //DETECT_BASKET;
                            ac_manipulation.goal_manipulation.sub_cmd     = 0;              //default
                            ac_manipulation.goal_manipulation.shelf_id    = 4;              //Search Target = temp. position
                            ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                        }

                        ac_manipulation.SendGoalandWait();
                        if(manipulation_result == false){
                            ROS_ERROR("break!");
                            motion_result = false;
                            motion_flag = true;
                            manipulation_result = true;
                            break;
                        }
    //                    /// wait for the action to return
    //                    if(waitForResult(ac_manipulation));
    //                    else break;


                        //2 put
                        //debug
    //                    std::cout << "put_new: " << put_new << std::endl;

                        ac_manipulation.goal_manipulation.motion_cmd  = PUT_OBJECT;
                        ac_manipulation.goal_manipulation.sub_cmd     = 2 + put_new - old_fail;
                        ac_manipulation.goal_manipulation.shelf_id    = shelf_id;
                        ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                        ac_manipulation.goal_manipulation.tool_id     = tool_id;

                        if(fsm_product_id == LUNCHBOX)
                            ac_manipulation.goal_manipulation.sub_cmd = 1 + put_new;      //ONLY Lunch box displays the new item in point 1.

                        //PRINTING DEBUG
                        std::cout << "put position: " << ac_manipulation.goal_manipulation.sub_cmd << std::endl;

                        ac_manipulation.SendGoalandWait();
                        if(manipulation_result == false){
                            ROS_ERROR("break!");
                            motion_result = false;
                            motion_flag = true;
                            manipulation_result = true;
                            break;
                        }

                    }
                    else{
                        motion_result = false;
                        motion_flag = true;
                        break;
                    }



                    motion_flag = true;
                    motion_result = true;
                    break;
                }
                case MOTION_HOME_POSE:
                {
                    std::cout << "=== MOTION_HOME_POSE ===" << std::endl;

                    //1 HOME motion
                    ac_manipulation.goal_manipulation.motion_cmd  = HOME_POSE;
                    ac_manipulation.goal_manipulation.object_id   = fsm_product_id;
                    ac_manipulation.goal_manipulation.shelf_id    = shelf_id;

                    ac_manipulation.SendGoalandWait();
                    if(manipulation_result == false){
                        ROS_ERROR("break!");
                        motion_result = false;
                        motion_flag = true;
                        manipulation_result = true;
                        break;
                    }
//                    /// wait for the action to return
//                    if(waitForResult(ac_manipulation));
//                    else break;

                    motion_result = true;
                    motion_flag = true;
                    break;
                }
                }
            }
        }

        ros::spinOnce();

        //loop repeats at desired rate
        loop_rate.sleep();

    }

    return 0;

}

