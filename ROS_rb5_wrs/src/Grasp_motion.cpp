#include <ros/ros.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <string.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <rb5_ros_wrapper/MotionAction.h>
#include <rb5_ros_wrapper/manipulationAction.h>
#include <actionlib/client/terminal_state.h>
#include "rb5_ros_wrapper/update.h"
#include <geometry_msgs/Point.h>
#include <std_msgs/Int32.h>
#include "lanros2podo.h"
#include "wrs_parameter.h"
#include "wrs_motion.h"
#include "tf/transform_listener.h"
#include "tf/transform_broadcaster.h"
#include "rb5_ros_wrapper/update.h"

#define D2R             0.0174533
#define R2D             57.2958


/*-------
 * 1. kimbap 2 via point
 * 2.
 *
 *
 *
 *
 * -------*/


ros::Subscriber robot_state_sub;
rb5_ros_wrapper::update robotstate;





typedef actionlib::SimpleActionClient<rb5_ros_wrapper::MotionAction> Client;

Motion cur_motion;
Motion *motion;

class GraspAction
{
public:
    GraspAction() : ac_("motion", true)
    {
        ROS_INFO("Waiting for action server to start.");
        ac_.waitForServer();
        ROS_INFO("Action server started.");
    }

    tf::StampedTransform transform_shelf;
    tf::StampedTransform transform;
    int MotionDone = IDLE;
    int shelf1_backline = false;
    double temp_yaw = 0.;
    int check_result = false;
    int _result = IDLE;

    void doInit()
    {
        //need to initialize the robot before sending commands
        ROS_INFO("Initializing.");
        Sendcommand(CMD_Initialize,0,0,0,0,0, 0, 0, 0, 0,0,0);

        //set mode
        ROS_INFO("Setting to real mode.");
        Sendcommand(CMD_ChangeMode,0,0,1,0,0, 0, 0, 0, 0,0,0);

        //change speed 50%
        ChangeSpeed(0.85);

        //joy move stop
//        Sendcommand(CMD_WheelMove,0,0,0,0,0,0,0,0,0,0,0);

        //home position
        goHome(IDLEPOSE);
    }

    void doTest()
    {
        ROS_INFO("Send TCP move");
//        Sendcommand(CMD_MoveJoint,0,0,0,-245.103, -138.91, 89.064, -132, -20, 0.21, 0.5,0.3);

        StateStart();

        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);


        StateStart();

        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);


        StateStart();

        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);


        StateStart();

        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,100,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600, 200,300,0,0,90,spd_60);
        Sendcommand(CMD_MoveTCP,0,0,0,600,-200,300,0,0,90,spd_60);


        ROS_INFO("TCP move done");
    }

    void BlendJointClear()
    {
        Sendcommand(CMD_MoveBlend,0,0,3,0,0,0,0,0,0,0,0);
    }
    void BlendJointAddPoint(Pose _pose, const float spd[])
    {
        rb5_goal.type = CMD_MoveBlend;
        rb5_goal.d0 = 0;
        rb5_goal.d1 = 0;
        rb5_goal.data = 4;
        rb5_goal.coordinate[0] = _pose.cdn[0];
        rb5_goal.coordinate[1] = _pose.cdn[1];
        rb5_goal.coordinate[2] = _pose.cdn[2];
        rb5_goal.coordinate[3] = _pose.cdn[3];
        rb5_goal.coordinate[4] = _pose.cdn[4];
        rb5_goal.coordinate[5] = _pose.cdn[5];
        rb5_goal.spd = spd[0];
        rb5_goal.acc = spd[1];
        SendGoalandWait();
    }
    void BlendJointAddPoint(float coordinate0, float coordinate1, float coordinate2, float coordinate3, float coordinate4, float coordinate5, const float spd[])
    {
        rb5_goal.type = CMD_MoveBlend;
        rb5_goal.d0 = 0;
        rb5_goal.d1 = 0;
        rb5_goal.data = 4;
        rb5_goal.coordinate[0] = coordinate0;
        rb5_goal.coordinate[1] = coordinate1;
        rb5_goal.coordinate[2] = coordinate2;
        rb5_goal.coordinate[3] = coordinate3;
        rb5_goal.coordinate[4] = coordinate4;
        rb5_goal.coordinate[5] = coordinate5;
        rb5_goal.spd = spd[0];
        rb5_goal.acc = spd[1];
        SendGoalandWait();
    }

    void BlendJointMove()
    {
        Sendcommand(CMD_MoveBlend,0,0,5,0,0,0,0,0,0,0,0);
    }
    void doneCb(const actionlib::SimpleClientGoalState& state,
                const rb5_ros_wrapper::MotionResultConstPtr& result)
    {
        ROS_INFO("Done CB");
        switch(result->rb5result)
        {
        case ACCEPT:
        case DONE:
            ROS_INFO("DONE");
            break;
        case STATE_ERROR:
        case INPUT_ERROR:
        case ERROR_STOP:
            ROS_ERROR("Action Server send rb5 ERROR");
            if(check_result == true)
            {
                _result = result->rb5result;
                check_result = false;
            }else
            {
                MotionDone = result->rb5result;
            }
            break;
        case EXT_COLLISION:
            ROS_ERROR("EXT COLLISION!!!!");
            MotionDone = result->rb5result;
            break;
        case HUMAN_DETECTED:
            ROS_ERROR("HUMAN_DETECTED!!!");
//            MotionDone = result->rb5result;
            break;
        }

        switch(result->wheelresult)
        {
        case ACCEPT:
        case DONE:
            break;
        case STATE_ERROR:
        case INPUT_ERROR:
        case ERROR_STOP:
            ROS_ERROR("Action Server send wheel ERROR");
            MotionDone = result->wheelresult;
            break;
        }
    }
    void activeCb()
    {
    }
    void feedbackCb(const rb5_ros_wrapper::MotionFeedbackConstPtr& feedback)
    {
    }

    void StateStart()
    {
        ROS_INFO("StateStart");
        Sendcommand(CMD_StateStart,0,0,0,0,0,0,0,0,0,0,0);
    }

    void Sendcommand(char type, int d0, int d1, float data, float coordinate0, float coordinate1, float coordinate2, float coordinate3, float coordinate4, float coordinate5, float spd, float acc)
    {
        rb5_goal.type = type;
        rb5_goal.d0 = d0;
        rb5_goal.d1 = d1;
        rb5_goal.data = data;
        rb5_goal.coordinate[0] = coordinate0;
        rb5_goal.coordinate[1] = coordinate1;
        rb5_goal.coordinate[2] = coordinate2;
        rb5_goal.coordinate[3] = coordinate3;
        rb5_goal.coordinate[4] = coordinate4;
        rb5_goal.coordinate[5] = coordinate5;
        rb5_goal.spd = spd;
        rb5_goal.acc = acc;
        SendGoalandWait();
    }
    void Sendcommand(char type, int d0, int d1, float data, float coordinate0, float coordinate1, float coordinate2, float coordinate3, float coordinate4, float coordinate5, const float spd[])
    {
        rb5_goal.type = type;
        rb5_goal.d0 = d0;
        rb5_goal.d1 = d1;
        rb5_goal.data = data;
        rb5_goal.coordinate[0] = coordinate0;
        rb5_goal.coordinate[1] = coordinate1;
        rb5_goal.coordinate[2] = coordinate2;
        rb5_goal.coordinate[3] = coordinate3;
        rb5_goal.coordinate[4] = coordinate4;
        rb5_goal.coordinate[5] = coordinate5;
        rb5_goal.spd = spd[0];
        rb5_goal.acc = spd[1];
        SendGoalandWait();
    }
    void Sendcommand(char type, Pose _pose,  const float spd[])
    {
        rb5_goal.type = type;
        rb5_goal.d0   = 0;
        rb5_goal.d1   = 0;
        rb5_goal.data = 0;
        rb5_goal.coordinate[0] = _pose.cdn[0];
        rb5_goal.coordinate[1] = _pose.cdn[1];
        rb5_goal.coordinate[2] = _pose.cdn[2];
        rb5_goal.coordinate[3] = _pose.cdn[3];
        rb5_goal.coordinate[4] = _pose.cdn[4];
        rb5_goal.coordinate[5] = _pose.cdn[5];
        rb5_goal.spd = spd[0];
        rb5_goal.acc = spd[1];
        SendGoalandWait();
    }
    void Sendcommandwheel(char type, int d0, int d1, float x_m, float y_m, float yaw_deg)
    {
        rb5_goal.type = type;
        rb5_goal.d0 = d0;
        rb5_goal.d1 = d1;
        rb5_goal.wheel[0] = x_m;
        rb5_goal.wheel[1] = y_m;
        rb5_goal.wheel[2] = yaw_deg;
        SendGoalandWait();
    }
    void SendGoalandWait()
    {
        if(MotionDone == ACCEPT || MotionDone == IDLE || MotionDone == EXT_COLLISION)
        {
            ac_.sendGoal(rb5_goal, boost::bind(&GraspAction::doneCb, this, _1, _2), boost::bind(&GraspAction::activeCb, this), boost::bind(&GraspAction::feedbackCb, this, _1));
            ac_.waitForResult();
        }else
        {
            ROS_ERROR("Disregard command");
        }
    }
    int sendMotion(Motion _curMotion);

    void goHome(int _curPose);
    void goSearch(const rb5_ros_wrapper::manipulationGoalConstPtr _goal);
    void goGrasp(const rb5_ros_wrapper::manipulationGoalConstPtr _goal);
    void goPut(const rb5_ros_wrapper::manipulationGoalConstPtr _goal, Pose _pose);

    void pullshelf(const rb5_ros_wrapper::manipulationGoalConstPtr _goal);
    void pushshelf(const rb5_ros_wrapper::manipulationGoalConstPtr _goal);

    void TCPtoRB5(int _mode);
    void TCPtoRB5(Pose _pose, int _mode);

    void PrintCoordinate(char *_name, tf::Vector3 _p, tf::Matrix3x3 _o)
    {
        double rr,pp,yy;
        _o.getEulerYPR(yy,pp,rr);
        printf("%s Pose = [%f, %f, %f], [%f, %f, %f]\n", _name, _p.x()*1000, _p.y()*1000, _p.z()*1000, rr*R2D, pp*R2D, yy*R2D);
    }
    void PrintCoordinate(char *_name, float c0, float c1, float c2, float c3, float c4, float c5)
    {
        printf("%s Pose = [%f, %f, %f], [%f, %f, %f]\n", _name, c0,c1,c2,c3,c4,c5);
    }
    void PrintGoal(const rb5_ros_wrapper::manipulationGoalConstPtr _goal)
    {
        int _shelf = _goal->shelf_id;
        int _id = _goal->object_id;
        int _sub = _goal->sub_cmd;
        char *cmd, *sub, *object;
        sub = "";
        switch(_goal->motion_cmd)
        {
        case HOME_POSE:
        {
            cmd = "Home_Pose";
            break;
        }
        case OBJECT_SEARCH:
        {
            if(_shelf == BASKET)
                cmd = "Object_Search (Basket)";
            else if(_shelf == TEMP)
                cmd = "Object_Search (Lunchbox)";
            else
                cmd = "Object_Search (Shelf)";

            switch(_sub)
            {
            case 0:
                sub = "Motion 1";
                break;
            case 1:
                sub = "Motion 2";
                break;
            case 2:
                sub = "Motion 3";
                break;
            case 3:
                sub = "Motion 4";
                break;
            default:
                sub = "Wrong_Subcmd";
                break;
            }
            break;
        }
        case GRASP_OBJECT:
        {
            cmd = "Object_Grasp";
            switch(_shelf)
            {
            case SHELF1:
                sub = "at Shelf1";
                break;
            case SHELF2:
                sub = "at Shelf2";
                break;
            case SHELF3:
                sub = "at Shelf3";
                break;
            case BASKET:
                sub = "at Basket";
                break;
            case TEMP:
                sub = "at TempPlace";
                break;
            default:
                sub = "Wrong Shelf num";
                break;
            }
            break;
        }
        case PUT_OBJECT:
        {
            cmd = "Object_Put";
            switch(_sub)
            {
            case DISPOSE:
                sub = "Dispose";
                break;
            case LINE1:
                sub = "Align to Line 1";
                break;
            case LINE2:
                sub = "Align to Line 2";
                break;
            case LINE3:
                sub = "Align to Line 3";
                break;
            case LUNCHBOX_PUT:
                sub = "Put temp lunchbox";
                break;
            default:
                sub = "Wrong_Subcmd";
                break;
            }
            break;
        }
        case PULL_SHELF:
        {
            if(_goal->sub_cmd == SEARCH)
            {
                cmd = "Shelf_Pull (Search)";
            }else
            {
                cmd = "Shelf_Pull (Pull)";
            }
            if(_shelf == SHELF1)
                sub = "1";
            else if(_shelf == SHELF2)
                sub = "2";
            else if(_shelf == SHELF3)
                sub = "3";
            else
                sub = "-1";
            break;
        }
        case PUSH_SHELF:
        {
            cmd = "Shelf_Push";
            if(_shelf == SHELF1)
                sub = "1";
            else if(_shelf == SHELF2)
                sub = "2";
            else if(_shelf == SHELF3)
                sub = "3";
            else
                sub = "-1";
            break;
        }
        case DETECT_SHELF:
        {
            cmd = "Shelf_Search";
            if(_shelf == SHELF1)
                sub = "1";
            else if(_shelf == SHELF2)
                sub = "2";
            else if(_shelf == SHELF3)
                sub = "3";
            else
                sub = "-1";
            break;
        }
        default:
            cmd = "Wrong_Command";
            break;
        }

        switch(_goal->object_id)
        {
        case KIMBAP1:
            object = "Kimbap1";
            break;
        case KIMBAP2:
            object = "Kimbap2";
            break;
        case SANDWICH:
            object = "Sandwich";
            break;
        case HAMBUG:
            object = "Hambug";
            break;
        case COFFEE:
            object = "Coffee";
            break;
        case LUNCHBOX:
            object = "Lunchbox";
            break;
        default:
            object = "Wrong_object";
            break;
        }

        ROS_INFO("\n");
        ROS_INFO("-------------------------------------");
        ROS_INFO("New Goal received : %s %s",cmd,sub);
        ROS_INFO("object : %s",object);
        ROS_INFO("-------------------------------------");
        ROS_INFO("\n");
    }

    void ChangeSpeed(float _spd)
    {
        if(_spd > 1.0) _spd = 1.0;
        else if(_spd < 0.0) _spd = 0.0;

        //change speed to _spd*100%
        Sendcommand(CMD_ChangeSpeed,0,0,_spd,0,0, 0, 0, 0, 0,0,0);
        ROS_INFO("Changing speed : %f",_spd);
    }

    void getShelfPose(const rb5_ros_wrapper::manipulationGoalConstPtr _goal)
    {
        //----------------- target pose set ----------------------//
        usleep(1000*1000);
        int _shelf = _goal->shelf_id;
        try
        {
            ros::Time now = ros::Time::now();
            listener.lookupTransform("/rb5/base", "/shelf", ros::Time(0), transform_shelf);
        }
        catch(tf::TransformException ex)
        {
            ROS_ERROR("%s",ex.what());
            return;
        }
        tf::Vector3 temp;
        temp = transform_shelf.getOrigin();
        ROS_INFO("\n");
        ROS_INFO("-----------------------------------------------");
        ROS_INFO("Back Shelf %d detected : %f, %f, %f", _shelf, temp.x(), temp.y(), temp.z());

        float new_x = temp.x();
        float new_y = temp.y();
        float new_z = temp.z()+marker_z_offset;
        switch(_shelf)
        {
        case SHELF1:
            if(fabs(temp.y()-shelf_marker[BACK1].y) > limit_marker_offset_b)
            {
//                ROS_ERROR("Marker y is changed %f to %f",temp.y(),shelf_marker[BACK1].y);
//                new_y = shelf_marker[BACK1].y;
            }
            if(fabs(temp.z()-shelf_marker[BACK1].z) > limit_marker_offset_z)
            {
                ROS_ERROR("Marker z is changed %f to %f",temp.z(),shelf_marker[BACK1].z);
                new_z = shelf_marker[BACK1].z;
            }
            Sendcommand(CMD_MoveTCP,0,0,0,0,-350,100,90,0,0,spd_60);
            break;
        case SHELF2:
            if(fabs(temp.y()-shelf_marker[BACK2].y) > limit_marker_offset_b)
            {
//                ROS_ERROR("Marker y is changed %f to %f",temp.y(),shelf_marker[BACK2].y);
//                new_y = shelf_marker[BACK2].y;
            }
            if(fabs(temp.z()-shelf_marker[BACK2].z) > limit_marker_offset_z)
            {
                ROS_ERROR("Marker z is changed %f to %f",temp.z(),shelf_marker[BACK2].z);
                new_z = shelf_marker[BACK2].z;
            }
            break;
        case SHELF3:
            if(fabs(temp.y()-shelf_marker[BACK3].y) > limit_marker_offset_b)
            {
//                ROS_ERROR("Marker y is changed %f to %f",temp.y(),shelf_marker[BACK3].y);
//                new_y = shelf_marker[BACK3].y;
            }
            if(fabs(temp.z()-shelf_marker[BACK3].z) > limit_marker_offset_z)
            {
                ROS_ERROR("Marker z is changed %f to %f",temp.z(),shelf_marker[BACK3].z);
                new_z = shelf_marker[BACK3].z;
            }
            break;
        default:
            ROS_ERROR("Wrong Shelf number!!!! %d",_shelf);

            break;
        }

        Pshelf_back.setValue(new_x, new_y, new_z);
        transform_shelf.setOrigin(Pshelf_back);
//        Rshelf = transform_shelf.getBasis();

        ROS_INFO("-----------------------------------------------");
        ROS_INFO("\n");

        Sendcommand(CMD_MoveJoint,home_pose[JDEFAULTHOME],spd_80);
        if(MotionDone == ACCEPT)
        {
            MotionDone = DONE;
        }
    }

protected:
    rb5_ros_wrapper::MotionGoal rb5_goal;
    ros::NodeHandle n;
    rb5_ros_wrapper::manipulationFeedback feedback_;
    rb5_ros_wrapper::manipulationResult result_;
    tf::TransformListener listener;
    Client ac_;

    float offset_approach_z = 200.;
    float x_real, y_real, z_real, r_real, p_real, oy_real;

    tf::Matrix3x3 Rtarget, Robject, Rshelf_front, Rshelf_back;
    tf::Vector3   Ptarget, Pobject, Pshelf_front, Pshelf_back;

    void openGripper(int _msec=max_gripper_cnt);
    void closeGripper(int _msec=max_gripper_cnt);
    void pause(float time);
    void resume();
    void suction(int _mode)
    {
        if(_mode == 0)
        {
            ROS_INFO("Suction reset");
            Sendcommand(CMD_Suction,0,0,0,0,0,0,0,0,0,0,0);
        }else if(_mode == 1)
        {
            ROS_INFO("Suction suction");
            Sendcommand(CMD_Suction,0,0,1,0,0,0,0,0,0,0,0);
        }else if(_mode == 2)
        {
            ROS_INFO("Suction release");
            Sendcommand(CMD_Suction,0,0,2,0,0,0,0,0,0,0,0);
        }

        usleep(500*1000);
    }
};

int GraspAction::sendMotion(Motion _curMotion)
{
    switch(_curMotion.type)
    {
    case 'J':
        ROS_INFO("Move Joint : %f, %f, %f, %f, %f, %f, %f, %f",_curMotion.coordinate[0],_curMotion.coordinate[1],_curMotion.coordinate[2],_curMotion.coordinate[3],_curMotion.coordinate[4],_curMotion.coordinate[5],_curMotion.spd,_curMotion.acc);
        Sendcommand(CMD_MoveJoint, 0, 0, 0, _curMotion.coordinate[0],_curMotion.coordinate[1],_curMotion.coordinate[2],_curMotion.coordinate[3],_curMotion.coordinate[4],_curMotion.coordinate[5],_curMotion.spd, _curMotion.acc);
        break;
    case 'T':
        ROS_INFO("Move TCP : %f, %f, %f, %f, %f, %f, %f, %f",_curMotion.coordinate[0],_curMotion.coordinate[1],_curMotion.coordinate[2],_curMotion.coordinate[3],_curMotion.coordinate[4],_curMotion.coordinate[5],_curMotion.spd,_curMotion.acc);
        Sendcommand(CMD_MoveTCP, 0, 0, 0, _curMotion.coordinate[0],_curMotion.coordinate[1],_curMotion.coordinate[2],_curMotion.coordinate[3],_curMotion.coordinate[4],_curMotion.coordinate[5],_curMotion.spd, _curMotion.acc);
        break;
    case 'E':
        return 0;
    default:
        return -1;
    }
    return 1;
}

void GraspAction::openGripper(int _msec)
{
    if(_msec < 0 || _msec > max_gripper_cnt)
        _msec = max_gripper_cnt;

    //'E' command opens on 0,1, closes on 1,0
    Sendcommand(CMD_MoveGripper,0,1,_msec,0,0, 0, 0, 0, 0,0,0);

    usleep(500*1000);
}

void GraspAction::closeGripper(int _msec)
{
    if(_msec < 0 || _msec > max_gripper_cnt)
        _msec = max_gripper_cnt;

    //'E' command opens on 0,1, closes on 1,0
    Sendcommand(CMD_MoveGripper,1,0,_msec,0,0, 0, 0, 0, 0, 0, 0);

    usleep(500*1000);
}

void GraspAction::goHome(int _curPose)
{
    switch(_curPose)
    {
    case SHELF1:
        ROS_INFO("Going home : SHELF1");
        Sendcommand(CMD_MoveJoint,home_pose[TSHELF1HOME],spd_80);
        closeGripper();
        break;
    case BASKET:
        ROS_INFO("Going home : BASKET");
        Sendcommand(CMD_MoveJoint,home_pose[JBASKETHOME],spd_80);
        closeGripper();
        break;
    default:
        ROS_INFO("Going home : DEFAULT");
        Sendcommand(CMD_MoveJoint,home_pose[JDEFAULTHOME],spd_80);
        closeGripper();

        break;
    }
    if(MotionDone == ACCEPT)
    {
        MotionDone = DONE;
    }
}

void GraspAction::goSearch(const rb5_ros_wrapper::manipulationGoalConstPtr _goal)
{
    if(_goal->motion_cmd != OBJECT_SEARCH)
        return;

    int _shelf = _goal->shelf_id;
    int _id = _goal->object_id;
    Sendcommand(CMD_MoveJoint,home_pose[JDEFAULTHOME],spd_80);

    char *object_name;
    switch(_shelf)
    {
    case SHELF1:
    {
        if(_id == COFFEE)
        {
            object_name = "coffee";
            switch(_goal->sub_cmd)
            {
            case 0:
                Sendcommand(CMD_MoveJoint,0,0,0,-46.19, 43.27, 125.94, -79.22, 90, -43.82, spd_80);
                break;
            case 1:
                Sendcommand(CMD_MoveJoint,0,0,0,-50.35, 26.47, 112.92, -49.39, 90.10, -39.66, spd_80);
//                BlendJointClear();
//                BlendJointAddPoint(-41.78, 27.85, 130.84, -74.98, 81.95, 40.15, spd_80);
//                BlendJointAddPoint(-52.95, 65.54, 94.32, -67.92, 89.8, -38.8, spd_80);
//                BlendJointMove();
//                Sendcommand(CMD_MoveJoint,0,0,0,-58.36, 59.98, 82.72, -49.21, 91.23, -35.33, spd_80);
                break;
            case 2:
                Sendcommand(CMD_MoveJoint,0,0,0,-50.35, 26.47, 112.92, -49.39, 90.10, -39.66, spd_80);
                break;
            default:
                printf("no search pose %d\n",_goal->sub_cmd);
                MotionDone = INPUT_ERROR;
                return;
            }
        }else if(_id == LUNCHBOX)
        {
            object_name = "lunchbox";
            switch(_goal->sub_cmd)
            {
            case 0:
                Sendcommand(CMD_MoveJoint,0,0,0,-119.1, 45.89, 115.47, -71.37, 90, 29.08, spd_80);
                break;
            case 1:
                Sendcommand(CMD_MoveJoint,0,0,0,-119.1, 60.14, 110.79, -80.92, 90., 29.08, spd_80);
                break;
            case 2:
                Sendcommand(CMD_MoveJoint,0,0,0,-119.1, 45.89, 115.47, -71.37, 90, 29.08, spd_80);
                break;
            default:
                printf("no search pose %d\n",_goal->sub_cmd);
                MotionDone = INPUT_ERROR;
                return;
            }
        }
        break;
    }
    case SHELF2:
    {
        if(_id == SANDWICH)
        {
            object_name = "sandwich";
            switch(_goal->sub_cmd)
            {
            case 0:
                Sendcommand(CMD_MoveJoint,0,0,0,24.31, 10.22, 145.1, -14.9, 51.71, -152.88, spd_80);
                break;
            case 1:
                Sendcommand(CMD_MoveJoint,0,0,0,-108.45, 33., 137.11, -76.68, 158.87, -80, spd_80);
                break;
            case 2:
                Sendcommand(CMD_MoveJoint,0,0,0,-17.7, 42.29, 79.6, 25.15, 50.99, 22.21, spd_80);
                break;
            default:
                printf("no search pose %d\n",_goal->sub_cmd);
                MotionDone = INPUT_ERROR;
                return;
            }
        }else if(_id == HAMBUG)
        {
            object_name = "hambug";
            switch(_goal->sub_cmd)
            {
            case 0:
                Sendcommand(CMD_MoveJoint,0,0,0,-167.77, 29.13, 109.63, -35.03, 138.58, 108.05, spd_80);
                break;
            case 1:
                Sendcommand(CMD_MoveJoint,0,0,0,-50.82, 7.45, 130.28, -18.08, 35.13, 55.17, spd_80);
                break;
            case 2:
                Sendcommand(CMD_MoveJoint,0,0,0,-120.75, 9.03, 88.47, -9.97, 90.93, 29.48, spd_80);
                break;
            default:
                printf("no search pose %d\n",_goal->sub_cmd);
                MotionDone = INPUT_ERROR;
                return;
            }
        }
        break;
    }
    case SHELF3:
    {
        if(_id == KIMBAP1)
        {
            object_name = "kimbap1";
            switch(_goal->sub_cmd)
            {
            case 0:
                Sendcommand(CMD_MoveJoint,0,0,0,-102.71, -23.19, 112.81, -21.36, 134.97, 70.01, spd_80);
                break;
            case 1:
                Sendcommand(CMD_MoveJoint,0,0,0,-109.5, -16.23, 128.47,-41.32, 148.34,76.08, spd_80);
                break;
            case 2:
                Sendcommand(CMD_MoveJoint,0,0,0, 21.16, -46.38, 125.86, 42.52, 36.13, -127.74, spd_80);
                break;
            default:
                printf("no search pose %d\n",_goal->sub_cmd);
                MotionDone = INPUT_ERROR;
                return;
            }
        }else if(_id == KIMBAP2)
        {
            object_name = "kimbap2";
            switch(_goal->sub_cmd)
            {
            case 0:
                BlendJointClear();
                BlendJointAddPoint(-138.2, 8.32, 66.76, 19.86, 117.67, 108.38, spd_80);
                BlendJointAddPoint(-171.78, 23.57, 53.91, 30.48, 138.9, 128.07, spd_80);
                BlendJointMove();
                break;
            case 1:
                BlendJointClear();
                BlendJointAddPoint(-138.2, 8.32, 66.76, 19.86, 117.67, 108.38, spd_80);
                BlendJointAddPoint(-142.65, 45.16, 28.69, 30.8, 138.38, 109.26, spd_80);
                BlendJointMove();
                break;
            case 2:
                Sendcommand(CMD_MoveJoint,0,0,0,-33.56, -24.07, 117.18, 47.59, 40.48, -148.09, spd_80);
                break;
            default:
                printf("no search pose %d\n",_goal->sub_cmd);
                MotionDone = INPUT_ERROR;
                return;
            }
        }
        break;
    }
    case BASKET:
    {
        Sendcommand(CMD_MoveJoint,home_pose[JDEFAULTHOME],spd_80);

        switch(_goal->object_id)
        {
        case KIMBAP1:
            object_name = "kimbap1 (basket)";
            Sendcommand(CMD_MoveJoint, 0, 0, 0, -213.88, 44.03, 81.54, 19.62, 61.15, -161.44, spd_80);
            break;
        case KIMBAP2:
            object_name = "kimbap2 (basket)";
            Sendcommand(CMD_MoveJoint, 0, 0, 0, -213.88, 44.03, 81.54, 19.62, 61.15, -161.44, spd_80);
            break;
        case SANDWICH:
            object_name = "sandwich (basket)";
            Sendcommand(CMD_MoveJoint, 0, 0, 0, -211.24, 49.43, 70.97, 25.6, 63.32, -163.12, spd_80);
            break;
        case HAMBUG:
            object_name = "hambug (basket)";
            Sendcommand(CMD_MoveJoint, 0, 0, 0, -211.24, 49.43, 70.97, 25.6, 63.32, -163.12, spd_80);
            break;
        case COFFEE:
            object_name = "coffee (basket)";
            Sendcommand(CMD_MoveJoint, 0, 0, 0, 82.87, 18.91, 106.7, -35.61, 90.01, 7.12, spd_80);
            break;
        case LUNCHBOX:
            object_name = "lunchbox (basket)";
            Sendcommand(CMD_MoveJoint, 0, 0, 0, 80.34, 1.63, 128.19, -39.81, 90.01, 9.65, spd_80);
            break;
        default:
            return;
        }
        break;
    }
    case TEMP:
    {
        object_name = "old lunchbox";
        Sendcommand(CMD_MoveJoint , search_pose[TEMP_LUNCHBOX], spd_80);
        break;
    }
    }

    ROS_INFO("Going Search :: %s, %d", object_name, _goal->sub_cmd);

//    usleep(500*1000);
    if(MotionDone == ACCEPT)
    {
        MotionDone = DONE;
    }
}

void GraspAction::goGrasp(const rb5_ros_wrapper::manipulationGoalConstPtr _goal)
{
    if(_goal->motion_cmd != GRASP_OBJECT)
        return;

    int tool = _goal->tool_id;

    usleep(500*1000);
    /*---------------------transform base to marker--------------------------*/
    try
    {
        ros::Time now = ros::Time::now();
        listener.lookupTransform("/rb5/base", "/marker1", ros::Time(0), transform);
    }
    catch(tf::TransformException ex)
    {
        ROS_ERROR("%s",ex.what());
        return;
    }

    tf::Vector3 temp;
    temp = transform.getOrigin();
    Robject = transform.getBasis();
    double rr,pp,yy;
    Robject.getEulerYPR(yy,pp,rr);

    int _id = _goal->object_id;
    int _shelf = _goal->shelf_id;
    float desired_z = shelf_marker[_shelf+5].z + Info_Object[_id].height;

    ROS_INFO("\n");
    ROS_INFO("-----------------------------------------------");
    ROS_INFO("Object %d detected : %f, %f, %f", _id, temp.x(), temp.y(), temp.z());
    ROS_INFO("RPY : %f, %f, %f", rr,pp,yy);
    ROS_INFO("desired Z : %f (coffee = -0.469)",desired_z);

    float new_x = temp.x();
    float new_y = temp.y();
    float new_z = temp.z()+marker_z_offset;

    //-0.592 + 0.123 = -0.469

    if(_goal->object_id == KIMBAP1 && _goal->shelf_id == SHELF3)
    {
        ROS_INFO("KIMBAP1 GRASP READY POSE");
        Sendcommand(CMD_MoveJoint,via_point[KIMBAP1_GRASP],spd_80);
    }else if(_goal->object_id == KIMBAP2 && _goal->shelf_id == SHELF3)
    {
        ROS_INFO("KIMBAP2 GRASP READY POSE");
        Sendcommand(CMD_MoveJoint,via_point[KIMBAP2_GRASP],spd_80);
    }else if(_goal->object_id == SANDWICH && _goal->shelf_id == SHELF2)
    {
        ROS_INFO("SANDWICH GRASP READY POSE");
        Sendcommand(CMD_MoveJoint,via_point[SANDWICH_GRASP],spd_80);

    }else if(_goal->object_id == HAMBUG && _goal->shelf_id == SHELF2)
    {
        ROS_INFO("HAMBUG GRASP READY POSE");
        Sendcommand(CMD_MoveJoint,via_point[HAMBUG_GRASP],spd_80);

    }

    if(_id == LUNCHBOX && fabs(temp.z()-desired_z) > Info_Object[LUNCHBOX].height - limit_marker_offset_z)
    {
        ROS_ERROR("Marker z is changed %f to %f",temp.z(),desired_z);
        new_z = desired_z + Info_Object[LUNCHBOX].height;
    }else if(fabs(temp.z()-desired_z) > limit_marker_z)
    {
        ROS_ERROR("Marker z is changed %f to %f",temp.z(),desired_z);
        //new_z = desired_z;
    }

    Pobject.setValue(new_x, new_y, new_z);
    transform.setOrigin(Pobject);
    ROS_INFO("-----------------------------------------------");
    ROS_INFO("\n");


    /*---------------------------- check object pose -------------------------------*/
    switch(_shelf)
    {
    case SHELF1:
    case SHELF2:
    {
        if(Pobject.y() < I_S[_goal->shelf_id].limit)
        {
            ROS_ERROR("RB5 can't grasp this object [y]");
            MotionDone = INPUT_ERROR;
            return;
        }else if(_shelf == SHELF1 && Pobject.y() < I_S[_goal->shelf_id].half_limit)
        {
            ROS_ERROR("RB5 can grasp object ?");
            shelf1_backline = true;
            temp_yaw = yy;
        }
        break;
    }
    case SHELF3:
    {
        float length = sqrt(Pobject.x()*Pobject.x() + Pobject.y()*Pobject.y());
        if(length > I_S[_shelf].limit)
        {
            ROS_ERROR("RB5 can't grasp this object [length = %f]",length);
            MotionDone = INPUT_ERROR;
            return;
        }
        break;
    }
    default:
        break;
    }
    ROS_INFO("RB5 can grasp this object..maybe..");

//    tool = Info_Object[_id].default_toolmode;
    float basket_z_lift = 0.0;
    float basket_z_approach = 0.0;
    float z_approach = z_grasp_approach;
    if(_shelf == BASKET)
    {
        basket_z_lift = 0.1;
        basket_z_approach = 0.05;
    }else if(_shelf == TEMP)
    {
        z_approach = 0.05;
    }

    //-------------------------------  Approach  -----------------------------------//
    if(tool == TOOLMODE_GRIPPER)
    {
        //approach to target + 5cm(z_approach) + 10cm(only basket)
        if(_goal->object_id == KIMBAP1 || _goal->object_id ==KIMBAP2)
        {

            Rtarget.setEulerYPR(yy, 0, 90*D2R);
            z_approach += 0.05;
        }else
        {
            Rtarget.setEulerYPR(yy-90*D2R, 0, 90*D2R);
        }

        Ptarget.setValue(Pobject.x(), Pobject.y(), Pobject.z() + z_approach + basket_z_approach);

        PrintCoordinate("Gripper Target",Ptarget,Rtarget);
        TCPtoRB5(TOOLMODE_GRIPPER);

        ROS_INFO("GRASP::Open Gripper");
        if(_goal->sub_cmd == LINE1)
        {
            openGripper(GRIPPER_SECOND);
        }else
        {
            openGripper(GRIPPER_FIRST);
        }

    }else if(tool == TOOLMODE_SUCTION)
    {
        Ptarget.setValue(Pobject.x(), Pobject.y(), Pobject.z() + z_approach + basket_z_approach);

        if(shelf1_backline == true)
        {
            Rtarget.setEulerYPR(180*D2R, 0, 90*D2R);
        }else if(_goal->sub_cmd == FALL_DOWN && _goal->object_id == SANDWICH)
        {
            Rtarget.setEulerYPR(pp, 0, 90*D2R);
        }else{
            Rtarget.setEulerYPR(yy, 0, 90*D2R);
        }
        PrintCoordinate("Suction Target",Ptarget,Rtarget);
        TCPtoRB5(TOOLMODE_SUCTION);
    }
    //---------------------------------Approach----------------------------------------//
    ROS_INFO("GRASP::Approach pose1");
    PrintCoordinate("Approach",x_real,y_real,z_real,r_real,p_real,oy_real);

    Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_60);


    ROS_INFO("GRASP::Approach pose2");
    z_real -= (z_approach + z_grasp_offset[tool] + basket_z_approach)*1000;
    PrintCoordinate("Approach2",x_real,y_real,z_real,r_real,p_real,oy_real);

    //--------------------------------------Suction-------------------------------------//
    if(tool == TOOLMODE_GRIPPER)
    {
        Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_approach);
        ROS_INFO("GRASP::Close Gripper");
        closeGripper();
    }else if(tool == TOOLMODE_SUCTION)
    {
        Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_10);
        ROS_INFO("GRASP::Suction");
        suction(0);
        suction(1);
    }

    //-------------------------------------------Lift-----------------------------------//
    ROS_INFO("GRASP::Lift");
    z_real += (Info_Object[_id].z_lift + basket_z_lift)*1000;

    PrintCoordinate("Lift",x_real,y_real,z_real,r_real,p_real,oy_real);
    check_result = true;
    Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

    if(_result == ERROR_STOP)
    {
        ROS_ERROR("-----------RB5 ERROR STOP------------");
        ROS_ERROR("Motion Halt");
        Sendcommand(CMD_MotionHalt,0,0,0,0,0,0,0,0,0,0,0);

        ROS_ERROR("Move Joint 2,3");
        Sendcommand(CMD_MoveJoint,0,0,0,robotstate.joint_angles[0],robotstate.joint_angles[1]-10,robotstate.joint_angles[2]+10,robotstate.joint_angles[3],robotstate.joint_angles[4],robotstate.joint_angles[5],spd_10[0],spd_10[1]);
    }else
    {
        MotionDone = _result;
    }


    switch(_shelf)
    {
    case SHELF1:
        break;
    case SHELF2:
        break;
    case SHELF3:
        break;
    case BASKET:
        switch(_id)
        {
        case KIMBAP1:
        case KIMBAP2:
            Sendcommand(CMD_MoveJoint,via_point[JBASKETTO3],spd_60);
            break;
        case SANDWICH:
        case HAMBUG:
        case COFFEE:
        case LUNCHBOX:
            Sendcommand(CMD_MoveJoint,via_point[JBASKETTO12],spd_60);
            break;
        }
        break;
    case TEMP:
        Sendcommand(CMD_MoveJoint,via_point[JTEMPTOSHELF],spd_80);
        break;
    }


    //if fall_down

    if(_goal->sub_cmd == FALL_DOWN && tool == TOOLMODE_GRIPPER)
    {
        if(_goal->object_id == KIMBAP1)
        {
            x_real = 200;
            y_real = -300;
            z_real = 500;
            r_real = 0.;
            p_real = 0.;
            oy_real = 0.;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            z_real = 300;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            openGripper();
            z_real = 400;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            goHome(SHELF3);
        }else if(_goal->object_id == KIMBAP2)
        {
            x_real = -200;
            y_real = -300;
            z_real = 500;
            r_real = 0.;
            p_real = 0.;
            oy_real = 0.;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            z_real = 300;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            openGripper();
            z_real = 400;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            goHome(SHELF3);

        }else if(_goal->object_id == SANDWICH)
        {
            x_real = 250;
            y_real = -300;
            z_real = 150;
            r_real = 0.;
            p_real = 0.;
            oy_real = 0.;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            z_real = -55;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            openGripper();
            z_real = 150;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_lift);

            goHome(SHELF2);
        }
    }else if(_goal->sub_cmd == FALL_DOWN && tool == TOOLMODE_SUCTION)
    {
        Sendcommand(CMD_MoveJoint, via_point[JHAMBUG_FALLDOWN],spd_lift);

        Sendcommand(CMD_MoveTCP, via_point[THAMBUG_PUT],spd_10);

        suction(2);

        Sendcommand(CMD_MoveTCP, via_point[THAMBUG_FALLDOWN],spd_lift);

        goHome(SHELF2);
    }


    ROS_INFO("GRASP::Completed ;D\n");
    if(MotionDone == ACCEPT)
    {
        MotionDone = DONE;
    }
}

void GraspAction::goPut(const rb5_ros_wrapper::manipulationGoalConstPtr _goal, Pose _pose)
{
    int tool = _goal->tool_id;
    int _shelf = _goal->shelf_id;
    int _mode = _goal->sub_cmd;
    int _id = _goal->object_id;

    //need to erase this for adding suction picking mode
//    tool = Info_Object[_goal->object_id].default_toolmode;

    switch(_mode)
    {
    case DISPOSE:
    {
        if(_shelf == SHELF1)
        {
            ROS_INFO("PUT::Lift");
            Sendcommand(CMD_MoveTCP,via_point[TLUNCHBOX_VIA1],spd_80);
            ROS_INFO("PUT::VIA1");
            BlendJointClear();
            BlendJointAddPoint(via_point[JDISPOSE_VIA1],spd_80);
//            Sendcommand(CMD_MoveJoint,via_point[JDISPOSE_VIA1],spd_80);
            ROS_INFO("PUT::Dispose");
            BlendJointAddPoint(via_point[JDISPOSE],spd_80);
            BlendJointMove();
//            Sendcommand(CMD_MoveJoint,via_point[JDISPOSE],spd_80);
        }else if(_shelf == SHELF3)
        {
            ROS_INFO("PUT::Lift");
            Sendcommand(CMD_MoveTCP, via_point[TGRASP_SHELF3],spd_60);
            ROS_INFO("PUT::Dispose");
            Sendcommand(CMD_MoveJoint,via_point[JDISPOSE],spd_80);
        }else if(_shelf == SHELF2)
        {
            ROS_INFO("PUT::Lift");
            Sendcommand(CMD_MoveJoint,via_point[JDISPOSE_VIA1],spd_80);
            ROS_INFO("PUT::Dispose");
            Sendcommand(CMD_MoveJoint,via_point[JDISPOSE],spd_80);
        }
        break;
    }
    case LUNCHBOX_PUT:
    {
        if(_goal->object_id == LUNCHBOX)
        {
            Sendcommand(CMD_MoveTCP,via_point[TLUNCHBOX_VIA1],spd_80);
        }
//        Sendcommand(CMD_MoveJoint,via_point[JLUNCHBOX_VIA2],spd_80);
        Sendcommand(CMD_MoveJoint,via_point[JLUNCHBOX_PUT],spd_80);

        break;
    }
    case LINE1:
    case LINE2:
    case LINE3:
    {
        /* pos_x, pos_y - 0.03, pos_z + 0.1~0.2 */
        _pose.cdn[1] -= y_put_object*1000;
        _pose.cdn[2] += Info_Object[_goal->object_id].z_lift*1000;
        TCPtoRB5(_pose, tool);
        PrintCoordinate("Put 1",x_real,y_real,z_real,r_real,p_real,oy_real);
        Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_approach);


        /* pos_x, pos_y - 0.03, pos_z + 0.02(only suction) */
        _pose.cdn[2] -= (Info_Object[_goal->object_id].z_lift - z_put_offset[tool])*1000;


        //shelf tunning
//        if(_mode == LINE1)
//        {
//            if(_id == HAMBUG)
//            {
//                ROS_INFO("Hambug,Coffee(Line1) -20z");
//                _pose.cdn[2] -= 10;
//            }else if(_id == SANDWICH)
//            {
//                _pose.cdn[2] += 10;
//            }else
//            {
//                ROS_INFO("Line1 -15z");
//                _pose.cdn[2] -= 15;

//            }
//        }else if(_mode == LINE2)
//        {
//            if(_id == HAMBUG)
//            {
//                ROS_INFO("Hambug(Line2) +20z");
//                _pose.cdn[2] += 20;
//            }else if(_id == SANDWICH)
//            {
//                _pose.cdn[2] += 10;
//            }else if(_id == KIMBAP1 || _id == KIMBAP2)
//            {
//                ROS_INFO("Kimbap(Line1) -10z");
//                _pose.cdn[2] -= 15;
//            }
//        }else if(_mode == LINE3)
//        {
//            if(_id == KIMBAP1 || _id == KIMBAP2)
//            {
//                ROS_INFO("Kimbap(Line1) -5z");
//                _pose.cdn[2] -= 15;
//            }
//        }

        TCPtoRB5(_pose, tool);
        PrintCoordinate("Put 2",x_real,y_real,z_real,r_real,p_real,oy_real);
        Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_approach);


        if(_goal->object_id == LUNCHBOX || _goal->sub_cmd == LINE1)
        {//no over pull
            y_real += y_put_object*1000;
            PrintCoordinate("Put 3",x_real,y_real,z_real,r_real,p_real,oy_real);
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_10);
        }else
        {
            /* pull object over 1cm */
            y_real += y_pull_object*1000;
            PrintCoordinate("Put 3",x_real,y_real,z_real,r_real,p_real,oy_real);
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_10);
        }
        break;
    }
    }

    usleep(500*1000);

    if(tool == TOOLMODE_GRIPPER)
    {
        openGripper(GRIPPER_PUT);
    }else
    {
        suction(2);
    }

    switch(_goal->sub_cmd)
    {
    case DISPOSE:
        break;
    case LUNCHBOX_PUT:
        Sendcommand(CMD_MoveTCP,via_point[TLUNCHBOX_PUT_LIFT], spd_lift);
        break;
    default:
        z_real += Info_Object[_goal->object_id].z_lift*1000;
        Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_lift);

        if(_shelf == SHELF1)
        {
            Sendcommand(CMD_MoveJoint,via_point[JSHELF1TOIDLE],spd_80);
        }
        break;
    }

    ROS_INFO("GRASP::Put Completed ;D\n\n");
    goHome(IDLEPOSE);
}

void GraspAction::pullshelf(const rb5_ros_wrapper::manipulationGoalConstPtr _goal)
{
    if(_goal->motion_cmd != PULL_SHELF)
        return;

    int _shelf = _goal->shelf_id;
    int _id = _goal->object_id;
    switch(_goal->sub_cmd)
    {
    case SEARCH:
    {
        //-----------------search pose set------------------------//
        int seq = 0;
        switch(_shelf)
        {
        case SHELF1:
            ROS_INFO("PULL SHELF::Search pose1");
            motion = Shelf_Search1;
            break;
        case SHELF2:
            ROS_INFO("PULL SHELF::Search pose2");
            motion = Shelf_Search2;
            break;
        case SHELF3:
            ROS_INFO("PULL SHELF::Search pose3");
            motion = Shelf_Search3;
            break;
        default:
            break;
        }

        cur_motion = motion[seq];

        //---------------------motion send------------------------//
        while(sendMotion(cur_motion) == 1)
        {
            seq++;
            cur_motion = motion[seq];
        }
        ROS_INFO("PULL SHELF::Search pose done");
        break;
    }
    case PULL:
    {
        //----------------- listen tf shelf ----------------------//
        usleep(1000*1000);
        try
        {
            ros::Time now = ros::Time::now();
            listener.lookupTransform("/rb5/base", "/shelf", ros::Time(0), transform_shelf);
        }
        catch(tf::TransformException ex)
        {
            ROS_ERROR("%s",ex.what());
            return;
        }


        tf::Vector3 temp;
        temp = transform_shelf.getOrigin();
        ROS_INFO("\n");
        ROS_INFO("-----------------------------------------------");
        ROS_INFO("Front Shelf %d detected : %f, %f, %f",_goal->shelf_id, temp.x(), temp.y(), temp.z());

        float new_x = temp.x();
        float new_y = temp.y();
        float new_z = temp.z()+marker_z_offset;
        /*
        switch(_shelf)
        {
        case SHELF1:
            if(fabs(temp.y()-shelf_marker[FRONT1].y) > 0.1)//limit_marker_offset_f)
            {
                ROS_ERROR("Marker y is changed %f to %f",temp.y(),shelf_marker[FRONT1].y);
                new_y = shelf_marker[FRONT1].y;
            }
            if(fabs(temp.z()-shelf_marker[FRONT1].z) > 0.1)//limit_marker_offset_z)
            {
                ROS_ERROR("Marker z is changed %f to %f",temp.z(),shelf_marker[FRONT1].z);
//                new_z = shelf_marker[FRONT1].z;
            }
            break;
        case SHELF2:
            if(fabs(temp.y()-shelf_marker[FRONT2].y) > limit_marker_offset_f)
            {
//                ROS_ERROR("Marker y is changed %f to %f",temp.y(),shelf_marker[FRONT2].y);
//                new_y = shelf_marker[FRONT2].y;
            }
            if(fabs(temp.z()-shelf_marker[FRONT2].z) > limit_marker_offset_z)
            {
//                ROS_ERROR("Marker z is changed %f to %f",temp.z(),shelf_marker[FRONT2].z);
//                new_z = shelf_marker[FRONT2].z;
            }
            break;
        case SHELF3:
            if(fabs(temp.y()-shelf_marker[FRONT3].y) > limit_marker_offset_f)
            {
//                ROS_ERROR("Marker y is changed %f to %f",temp.y(),shelf_marker[FRONT3].y);
//                new_y = shelf_marker[FRONT3].y;
            }
            if(fabs(temp.z()-shelf_marker[FRONT3].z) > limit_marker_offset_z)
            {
//                ROS_ERROR("Marker z is changed %f to %f",temp.z(),shelf_marker[FRONT3].z);
//                new_z = shelf_marker[FRONT3].z;
            }
            break;
        default:
            ROS_ERROR("Wrong Shelf number!!!!");
            break;
        }
*/
        Pshelf_front.setValue(new_x, new_y, new_z);
        transform_shelf.setOrigin(Pshelf_front);
        ROS_INFO("-----------------------------------------------");
        ROS_INFO("\n");



        //------------------------- set target pos --------------------------//
//        Rshelf = transform_shelf.getBasis();
        Ptarget = transform_shelf.getOrigin();
        Rtarget.setEulerYPR(0, 0, 0);

        TCPtoRB5(TOOLMODE_SUCTION);

        switch(_shelf)
        {
        case SHELF1:
        {
            openGripper();
            TCPtoRB5(TOOLMODE_GALGORI);
            Sendcommand(CMD_MoveJoint,0,0,0,-60, 77.8, 120.82, -108.59, 89.94, -30.24, spd_80);

            Sendcommandwheel('W', 0, 1, 0.3, 0, 0);

            Sendcommand(CMD_MoveTCP,0,0,0,0,-550, -330, 90, 0, 90, spd_80);

            z_real += 35;
            y_real -= 10;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_approach);

            ROS_INFO("MotionDone = %d",MotionDone);
            if(MotionDone == EXT_COLLISION)
            {
                ROS_INFO("EXT_COLLISION1");
                z_real += 20;
                y_real += 10;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_10);

                y_real -= 10;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_10);
            }

            z_real -= 45;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_10);

            ROS_INFO("MotionDone = %d",MotionDone);
            if(MotionDone == EXT_COLLISION)
            {
                ROS_INFO("EXT_COLLISION2");
                z_real += 5;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_10);

//                ROS_INFO("MotionDone = %d",MotionDone);
//                y_real -= 10;
//                z_real += 20;
//                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_10);

//                z_real -= 20;
//                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,90,0,90,spd_10);
            }

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real+300,z_real,90,0,90,spd_10);

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real+300,z_real+110,90,0,90,spd_approach);

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real+100,z_real+110,90,0,90,spd_approach);

            closeGripper();

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real+100,z_real+30,90,0,90,spd_approach);

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real+250,z_real+30,90,0,90,spd_turtle);

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real+200,z_real+110,90,0,90,spd_10);

            Sendcommand(CMD_MoveTCP,0,0,0,x_real,-550,-230,80,0,0,spd_60);

            closeGripper();
            break;
        }
        case SHELF2:
        case SHELF3:
        {
            //-----------------shelf approach---------------------//
            ROS_INFO("PULL SHELF::Approach pose1");
            y_real+=50;
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_approach);


            //--------------------Suction-------------------------//
            ROS_INFO("PULL SHELF::Suction");
            suction(0); //reset

            suction(1); //suction

            y_real-=50;
            ROS_INFO("PULL SHELF::Approach pose2");
            Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real, spd_10);

            //--------------------Pull shelf------------------------//
            ROS_INFO("PULL SHELF::Pull");
            if(_shelf == SHELF2)
            {
                printf("shelf2\n");
                y_real += 400;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_10);
            }else if(_shelf == SHELF3)
            {
                printf("shelf3\n");
                y_real += 400;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_10);
            }

            //--------------------release suction -----------------------//
            ROS_INFO("PULL SHELF::Release");
            suction(2); //release


            //----------------------Go to idle pose---------------------//
            if(_shelf == SHELF2)
            {
                ROS_INFO("PULL_SHELF::Final pose2");
                z_real += 100;
                y_real += 20;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_60);


                ROS_INFO("PULL_SHELF::Shelf search pose2");
                Sendcommand(CMD_MoveJoint,0,0,0,-77.34, 17.82, 122.3, -59.88, 87.82, -12.48, spd_80);


                ROS_INFO("Wheel move +0.3x");
                Sendcommandwheel('W', 0, 1, 0.3, 0, 0);
            }else if(_shelf == SHELF3)
            {
                ROS_INFO("PULL_SHELF::Final pose3");
                z_real += 100;
                y_real += 20;
                Sendcommand(CMD_MoveTCP,0,0,0,x_real,y_real,z_real,r_real,p_real,oy_real,spd_60);

                ROS_INFO("PULL_SHELF::Shelf search pose3");
                Sendcommand(CMD_MoveJoint,search_pose[JSHELF3_BACK],spd_80);
                Sendcommand(CMD_MoveJoint,0,0,0,-77.34, -0.37, 78.9, 1.71, 87.83, -12.47, spd_80);

                ROS_INFO("Wheel move +0.3x");
                Sendcommandwheel('W', 0, 1, 0.3, 0, 0);
            }
            break;
        }
        default:
        {
            MotionDone = INPUT_ERROR;
            return;
        }
        }
        break;
    }
    default:
        break;
    }
    if(MotionDone == ACCEPT || MotionDone == EXT_COLLISION)
    {
        MotionDone = DONE;
    }
    return;
}

void GraspAction::pushshelf(const rb5_ros_wrapper::manipulationGoalConstPtr _goal)
{
    int _shelf = _goal->shelf_id;
    //-----------------Init pose set------------------------//
    int seq = 0;
    switch(_shelf)
    {
    case SHELF1:
        ROS_INFO("Wheel move -0.1x");
        Sendcommandwheel('W', 0, 1, -0.1, 0, 0);
        ROS_INFO("PUSH SHELF::Init pose1");
        motion = Shelf_Push1;
        break;
    case SHELF2:
        ROS_INFO("Wheel move -0.3x");
        Sendcommandwheel('W', 0, 1, -0.3, 0, 0);
        ROS_INFO("PUSH SHELF::Init pose2");
        motion = Shelf_Push2;
        break;
    case SHELF3:
        ROS_INFO("Wheel move -0.3x");
        Sendcommandwheel('W', 0, 1, -0.3, 0, 0);
        ROS_INFO("PUSH SHELF::Init pose3");
        motion = Shelf_Push3;
        break;
    default:
        break;
    }

    cur_motion = motion[seq];

    //---------------------motion send------------------------//
    while(sendMotion(cur_motion) == 1)
    {
        seq++;
        cur_motion = motion[seq];
    }

    if(_shelf == SHELF1)
    {
        ROS_INFO("Wheel move -0.3x");
        Sendcommandwheel('W', 0, 1, -0.2, 0, 0);
        goHome(SHELF1);
    }else
    {
        goHome(IDLEPOSE);
    }

    ROS_INFO("PUSH SHELF::Search pose done");
    if(MotionDone == ACCEPT)
    {
        MotionDone = DONE;
    }
}

void GraspAction::pause(float time)
{
    Sendcommand(CMD_MotionPause,0,0,0,0,0, 0, 0, 0, 0,0,0);
    ROS_INFO("Waiting ", time, " seconds.");
    ac_.sendGoalAndWait(rb5_goal, ros::Duration(10.0));

    ros::Time beginTime = ros::Time::now();
    ros::Time curTime;
    ros::Duration durTime;
    int continueFlag = 0;

    while(continueFlag == 0)
    {
        curTime = ros::Time::now();
        durTime = curTime - beginTime;

        if(durTime.toSec() > time)
            continueFlag = 1;
    }
    resume();
}

void GraspAction::resume()
{
    Sendcommand(CMD_MotionResume,0,0,0,0,0, 0, 0, 0, 0,0,0);
    ROS_INFO("Starting again.");
    ac_.sendGoalAndWait(rb5_goal, ros::Duration(10.0));
}

void GraspAction::TCPtoRB5(int _mode)
{
    double rr,pp,yy;
    Rtarget.getEulerYPR(yy,pp,rr);
    printf("Target Pose : %f, %f, %f, %f, %f, %f\n",Ptarget.x()*1000, Ptarget.y()*1000, Ptarget.z()*1000, rr*R2D, pp*R2D, yy*R2D);

    tf::Matrix3x3 Rtool;

    tf::Vector3 Ptool, Poffset;

    if(_mode == TOOLMODE_GRIPPER)
        Poffset.setValue(-0.00035, 0.23122, 0.);
    else
        Poffset.setValue(-0.0235, 0.244, 0.);

    Rtool = Rtarget;
    Ptool = Ptarget + Rtool*Poffset;

    Rtool.getEulerYPR(yy,pp,rr);

    x_real = Ptool.x()*1000;
    y_real = Ptool.y()*1000;
    z_real = Ptool.z()*1000;
    r_real = (float)rr*R2D;
    p_real = (float)pp*R2D;
    oy_real= (float)yy*R2D;

    if(_mode == TOOLMODE_GALGORI)
    {
        printf("mode is galgori\n");
        x_real = Ptarget.x()*1000;
        y_real = Ptarget.y()*1000+ 410;
        z_real = Ptarget.z()*1000+ 210;
        r_real = 90;
        p_real = 0;
        oy_real = 90;
    }

    printf("TCPtoRB5 : %f, %f, %f, %f, %f, %f\n",x_real, y_real, z_real, r_real, p_real, oy_real);
}

void GraspAction::TCPtoRB5(Pose _pose,int _mode)
{
    tf::Matrix3x3 Rtcp;
    tf::Vector3 Ptcp;

    Ptcp.setValue(_pose.cdn[0]/1000.,_pose.cdn[1]/1000.,_pose.cdn[2]/1000.);
    Rtcp.setEulerYPR(_pose.cdn[5]*D2R, _pose.cdn[4]*D2R, _pose.cdn[3]*D2R);

    double rr,pp,yy;
    Rtcp.getEulerYPR(yy,pp,rr);
    printf("Target Pose : %f, %f, %f, %f, %f, %f\n",Ptcp.x()*1000, Ptcp.y()*1000, Ptcp.z()*1000, rr*R2D, pp*R2D, yy*R2D);

    tf::Matrix3x3 Rtool;

    tf::Vector3 Ptool, Poffset;

    if(_mode == TOOLMODE_GRIPPER)
        Poffset.setValue(-0.00035, 0.22122, 0.);
    else
        Poffset.setValue(-0.0235, 0.24, 0.);

    Rtool = Rtcp;
    Ptool = Ptcp + Rtool*Poffset;

    Rtool.getEulerYPR(yy,pp,rr);

    x_real = Ptool.x()*1000;
    y_real = Ptool.y()*1000;
    z_real = Ptool.z()*1000;
    r_real = (float)rr*R2D;
    p_real = (float)pp*R2D;
    oy_real= (float)yy*R2D;
    printf("TCPtoRB5 : %f, %f, %f, %f, %f, %f\n",x_real, y_real, z_real, r_real, p_real, oy_real);
}

//=========================================================================//

class manipulationAction
{
protected:
    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<rb5_ros_wrapper::manipulationAction> as_;
    std::string action_name_;

    rb5_ros_wrapper::manipulationFeedback feedback_;
    rb5_ros_wrapper::manipulationResult result_;
    rb5_ros_wrapper::manipulationGoalConstPtr goal;
    GraspAction newGrasp;

    Pose Tput_pose;
    Pose Jput_pose;
public:

    manipulationAction(std::string name) :
        as_(nh_, name, boost::bind(&manipulationAction::executeCB, this, _1), false),
        action_name_(name)
    {
        as_.start();
        newGrasp.doInit();
    }

    ~manipulationAction(void)
    {
    }

    //only called when client requests goal
    void executeCB(const rb5_ros_wrapper::manipulationGoalConstPtr &_goal)
    {
        goal = _goal;

        newGrasp.PrintGoal(goal);
        newGrasp.MotionDone = ACCEPT;
        newGrasp.StateStart();

        switch(goal->motion_cmd)
        {
        case HOME_POSE:
            newGrasp.goHome(IDLEPOSE);
            break;
        case OBJECT_SEARCH:
            newGrasp.goSearch(goal);
            break;
        case GRASP_OBJECT:
            newGrasp.goGrasp(goal);
            break;
        case PUT_OBJECT:
            goPut();
            break;
        case PULL_SHELF:
            newGrasp.pullshelf(goal);
            break;
        case PUSH_SHELF:
            newGrasp.pushshelf(goal);
            break;
        case DETECT_SHELF:
            newGrasp.getShelfPose(goal);
            break;
        default:
            break;
        }

        while(newGrasp.MotionDone == ACCEPT || newGrasp.MotionDone == EXT_COLLISION)
        {
            if(as_.isPreemptRequested() || !ros::ok())
            {
                ROS_INFO("%s: Preempted", action_name_.c_str());
                as_.setPreempted();
                break;
            }
            if(as_.isActive())
            {
                feedback_.dummy = 0;
                as_.publishFeedback(feedback_);
            }
        }

        result_.result_flag = newGrasp.MotionDone;
        ROS_INFO("MotionDone : %d",newGrasp.MotionDone);
        sendResult(newGrasp.MotionDone);

        newGrasp.MotionDone = IDLE;
    }

    void sendResult(int flag)
    {
        if(flag == DONE)
        {
            result_.result_flag = flag;
            ROS_INFO("Succeeded");
            as_.setSucceeded(result_);
        }else
        {
            result_.result_flag = flag;
            ROS_INFO("Aborted %d",flag);
            as_.setAborted(result_);
        }
    }

    //motion function call
    int goPut();
    void calPutoffset();
};

int manipulationAction::goPut()
{
    if(goal->motion_cmd != PUT_OBJECT)
        return STATE_ERROR;

    switch(goal->sub_cmd)
    {
    case DISPOSE:
        newGrasp.goPut(goal, Jput_pose);
        break;
    case LINE1:
        calPutoffset();
        newGrasp.goPut(goal, Tput_pose);
        break;
    case LINE2:
        calPutoffset();
        newGrasp.goPut(goal, Tput_pose);
        break;
    case LINE3:
        calPutoffset();
        newGrasp.goPut(goal, Tput_pose);
        break;
    case LUNCHBOX_PUT:
        calPutoffset();
        newGrasp.goPut(goal, Tput_pose);
        break;
    default:
        break;
    }
}

void manipulationAction::calPutoffset()
{
    float depth =       Info_Object[goal->object_id].depth;
    float first_depth = Info_Object[goal->object_id].first_depth;
    float height =      Info_Object[goal->object_id].height;
    float line_offset = (Info_Object[goal->object_id].line1 + Info_Object[goal->object_id].line2)/2.;


    tf::Vector3 Pshelf_back;
    Pshelf_back = newGrasp.transform_shelf.getOrigin();
    printf("shelf origin = %f, %f, %f\n", Pshelf_back.x(), Pshelf_back.y(), Pshelf_back.z());

    float lunchbox_z_offset = 0.;
    if(goal->object_id == LUNCHBOX)
    {
        lunchbox_z_offset = height*(goal->sub_cmd - 1);
    }

    float x_offset = line_offset;
    float y_offset = shelf_y_offset - first_depth - depth*(goal->sub_cmd - 1);
    float z_offset = height + lunchbox_z_offset;

    Tput_pose.cdn[0] = (Pshelf_back.x() + x_offset)*1000.;  //center of line x
    Tput_pose.cdn[1] = (Pshelf_back.y() + y_offset)*1000.;  //shelf y + object y...
    Tput_pose.cdn[2] = (Pshelf_back.z() + z_offset)*1000.;  //shelf z + object z
    Tput_pose.cdn[3] = 90.;
    Tput_pose.cdn[4] = 0.;

    if(newGrasp.shelf1_backline == true)
    {
        Tput_pose.cdn[5] = newGrasp.temp_yaw;
        newGrasp.shelf1_backline = false;
    }if(goal->object_id == SANDWICH || goal->object_id == HAMBUG)
        Tput_pose.cdn[5] = -90.;
    else
        Tput_pose.cdn[5] = 0.;

    printf("Offset = %f, %f, %f\n", x_offset, y_offset, z_offset);
    printf("Tput_pose = %f, %f, %f\n", Tput_pose.cdn[0], Tput_pose.cdn[1], Tput_pose.cdn[2]);
}

void robotstateCallback(const rb5_ros_wrapper::updatePtr& msg)
{
    for(int i=0;i<6;i++)
    {
        robotstate.joint_angles[i] = msg->joint_angles[i];
    }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "grasp_motion");

    manipulationAction manipulation("manipulation");

    ros::NodeHandle nh;

    robot_state_sub = nh.subscribe("/robot_states",1, &robotstateCallback);

//    while(1)
//    {

//        ros::spinOnce();
//    }
    ros::spin();
    return 0;
}
