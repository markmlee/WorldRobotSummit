#ifndef OMNIMOVE_H
#define OMNIMOVE_H
#include <stdio.h>
#include <cmath>
#include "BasicHeader/BasicMatrix.h"
#include "BasicHeader/RBLog.h"

#ifndef PI
#define PI			3.141592653589793
#endif
#ifndef D2R
#define D2R			1.745329251994330e-2
#endif
#ifndef R2D
#define R2D			5.729577951308232e1
#endif

enum{
    INIT = 0, SHORT, MIDDLE, FAR, TURN_LITTLE, MOVE_LITTLE, TURN_NORMAL, MOVE_NORMAL, TURN_FAR, MOVE_FAR
};
enum{
    TimeFix = 0, XFix
};
enum{
    BREAK = 0,
    MOVING,
    DONE
};

typedef struct _USER_INPUT_PARAMETER{
    double Distance_OmniTrajectoryX_m = 1.0;
    double Distance_OmniTrajectoryY_m = 1.0;
    double Distance_OmniTrajectoryR_Rad = 30.0*D2R;
    double Time_OmniTrajectory_sec = 1.0;

    /* OmniWheel profile */
    double Vmax_ms = 0.3;//0.57;//1.85;
    double Vmax_rads = 30.0*D2R;

    double Distance_TrajM_m = 0.98;
    double Distance_TrajRad = 30.0*D2R;
} USER_INPUT_PARA;



typedef struct _INFO_ROBOTMOVE{
    double X;
    double Y;
    double M;
    double Theta;
}Robot_move;

typedef struct _WHEEL_PARAMETER{
    double alpha1 =  47.93 *D2R;
    double alpha2 = -47.93 *D2R;
    double alpha3 = -132.07*D2R;
    double alpha4 =  132.07*D2R;

    double WheelRm = 0.125/2;
//    double RobotRm = 0.85467/2;
    double RobotRm = 0.81536/2;
}WHEEL_PARAMETER;

typedef struct _OMNI_PARAMETER{
    Robot_move Pin;
    Robot_move Probot;
    Robot_move Vrobot;
    double CurSec;
    double GoalSec;
    double GoalSecR;
    double TrajSec;
    double TrajSecR;
    double SatSecR;
    double SatSec;
    double TickSec = 0.005;
    double RatioTraj = 0.2;
    double move_theta = 0.;

    /* Profile by HD */
    Robot_move Pc;  //move distance during acc&dec
    double Vms;
    double Vrads;
    double Trajm;
    double Trajrad;
    double TrajT;
}OMNI_PARAMETER;

typedef struct _WHEEL_INFO{
    double InitRef_Deg;
    double WheelVel_ms;
    double MoveDistance_m;
    double Reference;
}WheelInfos;

typedef struct _ROBOT_STATE
{
    Robot_move SLAMPos;
    Robot_move GoalPos;
    Robot_move DiffPos;
    Robot_move CurrPos;
}ROBOT_STATE;

class OMNIMOVE
{
public:
    OMNIMOVE();

    WheelInfos FLWinfo;
    WheelInfos FRWinfo;
    WheelInfos BRWinfo;
    WheelInfos BLWinfo;
    OMNI_PARAMETER OMNIinfo;

    int OMNIstate = BREAK;
private:
    int FLAG_OMNImove;
    int FLAG_JOYmove;
    USER_INPUT_PARA in;
    WHEEL_PARAMETER Pw;
    ROBOT_STATE    ROBOTinfo;
    double curFLW, curFRW, curBRW, curBLW;
    double Xa3,Xa4,Xa5,Ya3,Ya4,Ya5,Ra3,Ra4,Ra5;
    int fix = XFix;
signals:
    void WHEEL_MOVE_DONE();
public:
    void Omni_CalVel_Robot5th();
    void Omni_CalRef_Motor();
    void Omni_CalVel_Wheel();
    doubles Omni_CalVel_Wheel(double _vx, double _vy, double _vr);

    void OMNIThread();
    void StartOMNImove(double _x, double _y, double _r);
    void JoyStickmove(int _onoff);
    void InitWheelInfo();
    void SetOMNIpara(int _mode);
    void SetOMNIstate(int _state);
    int IsMovingWheel();
    void SetCurReference(const double _curRef[4]);
};

#endif // OMNIMOVE_H
