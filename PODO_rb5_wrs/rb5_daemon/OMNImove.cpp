#include "OMNImove.h"

OMNIMOVE::OMNIMOVE()
{
    OMNIinfo.Vms = in.Vmax_ms;
    OMNIinfo.Vrads = in.Vmax_rads;
    OMNIinfo.Trajm = in.Distance_TrajM_m;
    OMNIinfo.Trajrad = in.Distance_TrajRad;
    OMNIinfo.TrajT = in.Time_OmniTrajectory_sec;

}

void OMNIMOVE::Omni_CalVel_Robot5th()
{
    static int cnt = 0;
    double vmaxX = OMNIinfo.Pin.X*(OMNIinfo.Vms/OMNIinfo.Pin.M);
    double vmaxY = OMNIinfo.Pin.Y*(OMNIinfo.Vms/OMNIinfo.Pin.M);
    double vmaxR;
    if(OMNIinfo.Pin.Theta > 0.)
        vmaxR = OMNIinfo.Vrads;
    else
        vmaxR = -OMNIinfo.Vrads;
    /* Move Rotate First !! */
    if(OMNIinfo.CurSec >= 0. && OMNIinfo.CurSec < OMNIinfo.TrajSecR)
    {
        double t = OMNIinfo.CurSec;
        OMNIinfo.Probot.Theta = Ra3*t*t*t + Ra4*t*t*t*t + Ra5*t*t*t*t*t;
        OMNIinfo.Vrobot.Theta = 3*Ra3*t*t + 4*Ra4*t*t*t + 5*Ra5*t*t*t*t;
    } else if(OMNIinfo.CurSec < OMNIinfo.TrajSecR + OMNIinfo.SatSecR)
    {
        double t = OMNIinfo.TrajSecR;
        double nt = OMNIinfo.CurSec - OMNIinfo.TrajSecR;

        if(nt <= OMNIinfo.SatSecR)
        {
            OMNIinfo.Probot.Theta =     Ra3*t*t*t + Ra4*t*t*t*t + Ra5*t*t*t*t*t + vmaxR*(nt);
            OMNIinfo.Vrobot.Theta =     vmaxR;
        }
    } else if(OMNIinfo.CurSec < OMNIinfo.GoalSecR)
    {
        double t = OMNIinfo.CurSec - OMNIinfo.SatSecR;
        OMNIinfo.Probot.Theta = Ra3*t*t*t + Ra4*t*t*t*t + Ra5*t*t*t*t*t + vmaxR*OMNIinfo.SatSecR;
        OMNIinfo.Vrobot.Theta = 3*Ra3*t*t + 4*Ra4*t*t*t + 5*Ra5*t*t*t*t;
    } else if(OMNIinfo.CurSec < OMNIinfo.GoalSecR + OMNIinfo.TrajSec)
    {
        double t = OMNIinfo.CurSec - OMNIinfo.GoalSecR;
        OMNIinfo.Probot.X =     Xa3*t*t*t + Xa4*t*t*t*t + Xa5*t*t*t*t*t;
        OMNIinfo.Probot.Y =     Ya3*t*t*t + Ya4*t*t*t*t + Ya5*t*t*t*t*t;

        OMNIinfo.Vrobot.X =     3*Xa3*t*t + 4*Xa4*t*t*t + 5*Xa5*t*t*t*t;
        OMNIinfo.Vrobot.Y =     3*Ya3*t*t + 4*Ya4*t*t*t + 5*Ya5*t*t*t*t;
    } else if(OMNIinfo.CurSec < OMNIinfo.GoalSecR + OMNIinfo.TrajSec + OMNIinfo.SatSec)
    {
        double t = OMNIinfo.TrajSec;
        double nt = OMNIinfo.CurSec - OMNIinfo.TrajSec- OMNIinfo.GoalSecR;

        if(nt <= OMNIinfo.SatSec)
        {
            OMNIinfo.Probot.X =     Xa3*t*t*t + Xa4*t*t*t*t + Xa5*t*t*t*t*t + vmaxX*(nt);
            OMNIinfo.Probot.Y =     Ya3*t*t*t + Ya4*t*t*t*t + Ya5*t*t*t*t*t + vmaxY*(nt);
            OMNIinfo.Vrobot.X =     vmaxX;
            OMNIinfo.Vrobot.Y =     vmaxY;
        }
    } else if(OMNIinfo.CurSec < OMNIinfo.GoalSec)
    {
        cnt = 0;
        double t = OMNIinfo.CurSec - OMNIinfo.SatSec - OMNIinfo.GoalSecR;
        OMNIinfo.Probot.X = Xa3*t*t*t + Xa4*t*t*t*t + Xa5*t*t*t*t*t + vmaxX*OMNIinfo.SatSec;
        OMNIinfo.Probot.Y = Ya3*t*t*t + Ya4*t*t*t*t + Ya5*t*t*t*t*t + vmaxY*OMNIinfo.SatSec;

        OMNIinfo.Vrobot.X = 3*Xa3*t*t + 4*Xa4*t*t*t + 5*Xa5*t*t*t*t;
        OMNIinfo.Vrobot.Y = 3*Ya3*t*t + 4*Ya4*t*t*t + 5*Ya5*t*t*t*t;
    } else
    {
        printf("CurSec is - or nan");
    }
}


void OMNIMOVE::Omni_CalVel_Wheel()
{
    double temp1 = sin(Pw.alpha1)*OMNIinfo.Vrobot.X - cos(Pw.alpha1)*OMNIinfo.Vrobot.Y - Pw.RobotRm*OMNIinfo.Vrobot.Theta;
    double temp2 = sin(Pw.alpha2)*OMNIinfo.Vrobot.X - cos(Pw.alpha2)*OMNIinfo.Vrobot.Y - Pw.RobotRm*OMNIinfo.Vrobot.Theta;
    double temp3 = sin(Pw.alpha3)*OMNIinfo.Vrobot.X - cos(Pw.alpha3)*OMNIinfo.Vrobot.Y - Pw.RobotRm*OMNIinfo.Vrobot.Theta;
    double temp4 = sin(Pw.alpha4)*OMNIinfo.Vrobot.X - cos(Pw.alpha4)*OMNIinfo.Vrobot.Y - Pw.RobotRm*OMNIinfo.Vrobot.Theta;

    FLWinfo.WheelVel_ms = temp1/Pw.WheelRm;
    FRWinfo.WheelVel_ms = temp2/Pw.WheelRm;
    BRWinfo.WheelVel_ms = temp3/Pw.WheelRm;
    BLWinfo.WheelVel_ms = temp4/Pw.WheelRm;
}

void OMNIMOVE::Omni_CalRef_Motor()
{
    FLWinfo.MoveDistance_m += FLWinfo.WheelVel_ms*R2D;
    FRWinfo.MoveDistance_m += FRWinfo.WheelVel_ms*R2D;
    BRWinfo.MoveDistance_m += BRWinfo.WheelVel_ms*R2D;
    BLWinfo.MoveDistance_m += BLWinfo.WheelVel_ms*R2D;

    FLWinfo.Reference = FLWinfo.InitRef_Deg + FLWinfo.MoveDistance_m*OMNIinfo.TickSec;
    FRWinfo.Reference = FRWinfo.InitRef_Deg + FRWinfo.MoveDistance_m*OMNIinfo.TickSec;
    BRWinfo.Reference = BRWinfo.InitRef_Deg + BRWinfo.MoveDistance_m*OMNIinfo.TickSec;
    BLWinfo.Reference = BLWinfo.InitRef_Deg + BLWinfo.MoveDistance_m*OMNIinfo.TickSec;

}

doubles OMNIMOVE::Omni_CalVel_Wheel(double _vx, double _vy, double _vr)
{
    double temp1 = sin(Pw.alpha1)*_vx - cos(Pw.alpha1)*_vy + Pw.RobotRm*_vr;
    double temp2 = sin(Pw.alpha2)*_vx - cos(Pw.alpha2)*_vy + Pw.RobotRm*_vr;
    double temp3 = sin(Pw.alpha3)*_vx - cos(Pw.alpha3)*_vy + Pw.RobotRm*_vr;
    double temp4 = sin(Pw.alpha4)*_vx - cos(Pw.alpha4)*_vy + Pw.RobotRm*_vr;

    doubles vwheel(4);

    vwheel[0] = temp1/Pw.WheelRm;
    vwheel[1] = temp2/Pw.WheelRm;
    vwheel[2] = temp3/Pw.WheelRm;
    vwheel[3] = temp4/Pw.WheelRm;

    return vwheel;
}

void OMNIMOVE::OMNIThread()
{
    if(FLAG_OMNImove == true)
    {
        if(!(OMNIinfo.CurSec > OMNIinfo.GoalSec))
        {
            OMNIstate = MOVING;
            Omni_CalVel_Robot5th();
            Omni_CalVel_Wheel();
            Omni_CalRef_Motor();
            OMNIinfo.CurSec += OMNIinfo.TickSec;
        }else
        {
            FLAG_OMNImove = false;
            InitWheelInfo();
            OMNIstate = DONE;
            FILE_LOG(logSUCCESS) << "MOVE DONE!!!";
        }
    }
}

void OMNIMOVE::StartOMNImove(double _x, double _y, double _r)
{
    InitWheelInfo();
    OMNIinfo.Pin.X = _x;
    OMNIinfo.Pin.Y = _y;
    OMNIinfo.Pin.Theta = _r*D2R;
    FLAG_OMNImove = true;
    OMNIinfo.CurSec = 0.;
    OMNIinfo.Probot.X = 0.;
    OMNIinfo.Probot.Y = 0.;
    OMNIinfo.Probot.Theta = 0.;
    /* Set M */
    OMNIinfo.Pin.M = sqrt(OMNIinfo.Pin.X*OMNIinfo.Pin.X + OMNIinfo.Pin.Y*OMNIinfo.Pin.Y);
    OMNIinfo.move_theta = OMNIinfo.Pin.Theta;//acos(fabs(OMNIinfo.Pin.Y)/OMNIinfo.Pin.M);


    if(OMNIinfo.Pin.M < 0.3 && OMNIinfo.Pin.M > 0.0001)
        SetOMNIpara(MOVE_LITTLE);
    else if(OMNIinfo.Pin.M < 0.7 && OMNIinfo.Pin.M > 0.0001)
        SetOMNIpara(MOVE_NORMAL);
    else if(OMNIinfo.Pin.M < 1.2 && OMNIinfo.Pin.M > 0.0001)
        SetOMNIpara(MOVE_FAR);

    if(fabs(OMNIinfo.Pin.Theta) < 20.0*D2R && fabs(OMNIinfo.Pin.Theta) > 0.01*D2R)
        SetOMNIpara(TURN_LITTLE);
    else if(fabs(OMNIinfo.Pin.Theta) < 40.0*D2R && fabs(OMNIinfo.Pin.Theta) > 0.01*D2R)
        SetOMNIpara(TURN_NORMAL);
    else if(fabs(OMNIinfo.Pin.Theta) < 80.0*D2R && fabs(OMNIinfo.Pin.Theta) > 0.01*D2R)
        SetOMNIpara(TURN_FAR);

    printf("Pin M = %f, theta = %f\n",OMNIinfo.Pin.M, OMNIinfo.move_theta);

    if(fix == XFix)
    {
        /* fixed move distance during acc&dec */
        if(fabs(OMNIinfo.Pin.M) < OMNIinfo.Trajm)
            OMNIinfo.Pc.M = OMNIinfo.Pin.M;
        else
            OMNIinfo.Pc.M = OMNIinfo.Trajm;

        if(fabs(OMNIinfo.Pin.Theta) < OMNIinfo.Trajrad)
            OMNIinfo.Pc.Theta = OMNIinfo.Pin.Theta;
        else
            OMNIinfo.Pc.Theta = OMNIinfo.Trajrad;

        /* Calculated move time during acc&dec */
        OMNIinfo.TrajSec = (30./16.)*(OMNIinfo.Pc.M - OMNIinfo.Probot.M)/(OMNIinfo.Vms*2.);
        OMNIinfo.TrajSecR = (30./16.)*fabs(OMNIinfo.Pc.Theta - OMNIinfo.Probot.Theta)/(OMNIinfo.Vrads*2.);

    } else if(fix == TimeFix)
    {
        /* fixed move time during acc&dec */
        if(fabs(OMNIinfo.Pin.M) < OMNIinfo.Trajm)
            OMNIinfo.TrajSec = OMNIinfo.TrajT*fabs(OMNIinfo.Pin.M/(OMNIinfo.Trajm));
        else
            OMNIinfo.TrajSec = OMNIinfo.TrajT;

        if(fabs(OMNIinfo.Pin.Theta) < OMNIinfo.Trajrad)
            OMNIinfo.TrajSecR = (OMNIinfo.TrajT)*fabs(OMNIinfo.Pin.Theta/OMNIinfo.Trajrad);
        else
            OMNIinfo.TrajSecR = OMNIinfo.TrajT;

        /* Calculated move distance during acc&dec */
        OMNIinfo.Pc.M = (16./30.)*OMNIinfo.TrajSec*2*OMNIinfo.Vms + OMNIinfo.Probot.M;
        OMNIinfo.Pc.Theta = (16./30.)*OMNIinfo.TrajSecR*2*OMNIinfo.Vrads + OMNIinfo.Probot.Theta;

    }
    OMNIinfo.Pc.X = OMNIinfo.Pin.X*(OMNIinfo.Pc.M/OMNIinfo.Pin.M);
    OMNIinfo.Pc.Y = OMNIinfo.Pin.Y*(OMNIinfo.Pc.M/OMNIinfo.Pin.M);

    if(OMNIinfo.Pin.Theta <= -OMNIinfo.Trajrad)  OMNIinfo.Pc.Theta = -OMNIinfo.Pc.Theta;


    printf("PcM = %f, PcX = %f, PcY = %f, max = %f\n",OMNIinfo.Pc.M, OMNIinfo.Pc.X, OMNIinfo.Pc.Y, OMNIinfo.Vms);
    /* Calculated move time during constant vel */
    if(OMNIinfo.Pin.M < OMNIinfo.Probot.M)
    {
        OMNIinfo.SatSec = 0.;
        OMNIinfo.Probot.M = OMNIinfo.Pin.M;
    } else
    {
        OMNIinfo.SatSec = (OMNIinfo.Pin.M - OMNIinfo.Pc.M)/OMNIinfo.Vms;
    }

    if(fabs(OMNIinfo.Pin.Theta) < OMNIinfo.Probot.Theta)
    {
        OMNIinfo.SatSecR = 0.;
        OMNIinfo.Probot.Theta = OMNIinfo.Pin.Theta;
    } else
    {
        OMNIinfo.SatSecR = fabs(OMNIinfo.Pin.Theta - OMNIinfo.Pc.Theta)/OMNIinfo.Vrads;
    }
    printf("pin = %f, pc = %f\n",OMNIinfo.Pin.Theta*R2D, OMNIinfo.Pc.Theta*R2D);
    /* Calculated move time */
    OMNIinfo.GoalSec = OMNIinfo.TrajSec*2. + OMNIinfo.TrajSecR*2. + OMNIinfo.SatSec + OMNIinfo.SatSecR;
    OMNIinfo.GoalSecR = OMNIinfo.TrajSecR*2. + OMNIinfo.SatSecR;

    double dx = OMNIinfo.Pc.X - OMNIinfo.Probot.X;
    double dy = OMNIinfo.Pc.Y - OMNIinfo.Probot.Y;
    double dr = OMNIinfo.Pc.Theta - OMNIinfo.Probot.Theta;
    double t = OMNIinfo.TrajSec*2.;
    double tr = OMNIinfo.TrajSecR*2.;
    printf("Probot = %f, %f, %f\n",OMNIinfo.Probot.X, OMNIinfo.Probot.Y, OMNIinfo.Probot.Theta*R2D);
    printf("dx = %f, %f, %f, t=%f, %f\n",dx,dy,dr*R2D,t,tr);
    printf("GoalSec = %f, GoalSecR = %f, T = %f, Tr = %f, S = %f, Sr = %f\n",OMNIinfo.GoalSec, OMNIinfo.GoalSecR, OMNIinfo.TrajSec, OMNIinfo.TrajSecR, OMNIinfo.SatSec, OMNIinfo.SatSecR);

    Xa3 = 10.*dx/(t*t*t);
    Xa4 = -15.*dx/(t*t*t*t);
    Xa5 = 6.*dx/(t*t*t*t*t);

    Ya3 = 10.*dy/(t*t*t);
    Ya4 = -15.*dy/(t*t*t*t);
    Ya5 = 6.*dy/(t*t*t*t*t);

    Ra3 = 10.*dr/(tr*tr*tr);
    Ra4 = -15.*dr/(tr*tr*tr*tr);
    Ra5 = 6.*dr/(tr*tr*tr*tr*tr);


}

void OMNIMOVE::InitWheelInfo()
{
    FLWinfo.InitRef_Deg = curFLW;
    FRWinfo.InitRef_Deg = curFRW;
    BRWinfo.InitRef_Deg = curBRW;
    BLWinfo.InitRef_Deg = curBLW;

    SetOMNIpara(INIT);

    FLWinfo.MoveDistance_m = 0.;
    FLWinfo.WheelVel_ms = 0.;

    FRWinfo.MoveDistance_m = 0.;
    FRWinfo.WheelVel_ms = 0.;

    BRWinfo.MoveDistance_m = 0.;
    BRWinfo.WheelVel_ms = 0.;

    BLWinfo.MoveDistance_m = 0.;
    BLWinfo.WheelVel_ms = 0.;

    OMNIinfo.Pin.X = 0.0;
    OMNIinfo.Pin.Y = 0.0;
    OMNIinfo.Pin.Theta = 0.0;

    OMNIinfo.Probot.X = 0.0;
    OMNIinfo.Probot.Y = 0.0;
    OMNIinfo.Probot.Theta = 0.0;

    OMNIinfo.Vrobot.X = 0.0;
    OMNIinfo.Vrobot.Y = 0.0;
    OMNIinfo.Vrobot.Theta = 0.0;

    OMNIinfo.CurSec = 0.0;
    OMNIinfo.GoalSec = 0.0;
    OMNIinfo.GoalSecR = 0.0;
    OMNIinfo.SatSec = 0.0;
    OMNIinfo.SatSecR = 0.0;
    OMNIinfo.TrajSec = 0.0;
    OMNIinfo.TrajSecR = 0.0;

    FLAG_JOYmove = false;
}

void OMNIMOVE::SetOMNIpara(int _mode)
{
    switch(_mode)
    {
    case INIT:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara INIT";
        fix = XFix;
        OMNIinfo.Vms =      in.Vmax_ms;//0.6ms
        OMNIinfo.Vrads =    in.Vmax_rads;//70.0degs
        OMNIinfo.Trajm =    in.Distance_TrajM_m;//0.98m
        OMNIinfo.Trajrad =  in.Distance_TrajRad;//20.0deg
        OMNIinfo.TrajT =    in.Time_OmniTrajectory_sec;//1.5sec
        break;
    }
    case SHORT:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara SHORT";
        //maybe short than 20cm, 10deg
        fix = TimeFix;
        OMNIinfo.Vms = 0.1;
        OMNIinfo.Vrads = 20.0*D2R;
        OMNIinfo.Trajm = 0.2;
        OMNIinfo.Trajrad = 10.0*D2R;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case TURN_LITTLE:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara TURN_LITTLE";
//        fix = TimeFix;
        OMNIinfo.Vrads = 15.0*D2R;
        OMNIinfo.Trajrad = 10.0*D2R;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case MOVE_LITTLE:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara MOVE_LITTLE";
//        fix = TimeFix;
        OMNIinfo.Vms = 0.1;
        OMNIinfo.Trajm = 0.1;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case TURN_NORMAL:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara TURN_NORMAL";
//        fix = TimeFix;
        OMNIinfo.Vrads = 30.0*D2R;
        OMNIinfo.Trajrad = 15.0*D2R;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case MOVE_NORMAL:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara MOVE_NORMAL";
//        fix = TimeFix;
        OMNIinfo.Vms = 0.2;
        OMNIinfo.Trajm = 0.1;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case TURN_FAR:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara TURN_FAR";
//        fix = TimeFix;
        OMNIinfo.Vrads = 60.0*D2R;
        OMNIinfo.Trajrad = 20.0*D2R;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case MOVE_FAR:
    {
        FILE_LOG(logWARNING) << "Set OMNIpara MOVE_FAR";
        fix = XFix;
        OMNIinfo.Vms =      in.Vmax_ms;//0.6ms
        OMNIinfo.Vrads =    in.Vmax_rads;//70.0degs
        OMNIinfo.Trajm =    in.Distance_TrajM_m;//0.98m
        OMNIinfo.Trajrad =  in.Distance_TrajRad;//20.0deg
        OMNIinfo.TrajT =    in.Time_OmniTrajectory_sec;//1.5sec
        break;
    }
    case MIDDLE:
    {
        //maybe short than 1m, 30deg
        fix = TimeFix;
        OMNIinfo.Vms = 0.4;
        OMNIinfo.Vrads = 30.0*D2R;
        OMNIinfo.Trajm = 0.4;
        OMNIinfo.Trajrad = 20.0*D2R;
        OMNIinfo.TrajT = 1.0;
        break;
    }
    case FAR:
    {
        fix = XFix;
        OMNIinfo.Vms =      in.Vmax_ms;//0.6ms
        OMNIinfo.Vrads =    in.Vmax_rads;//70.0degs
        OMNIinfo.Trajm =    in.Distance_TrajM_m;//0.98m
        OMNIinfo.Trajrad =  in.Distance_TrajRad;//20.0deg
        OMNIinfo.TrajT =    in.Time_OmniTrajectory_sec;//1.5sec
        break;
    }
    }
}

void OMNIMOVE::SetOMNIstate(int _state)
{
    OMNIstate = _state;
}

int OMNIMOVE::IsMovingWheel()
{
    return OMNIstate;
}

void OMNIMOVE::SetCurReference(const double _curRef[4])
{
    curFLW = _curRef[0];
    curFRW = _curRef[1];
    curBRW = _curRef[2];
    curBLW = _curRef[3];
}

void OMNIMOVE::JoyStickmove(int _onoff)
{
    if(_onoff == false)
        FLAG_JOYmove = false;
    else
        FLAG_JOYmove = true;
}
