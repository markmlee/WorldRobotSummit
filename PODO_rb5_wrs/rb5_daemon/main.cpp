#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <QSettings>

#include "RBCAN.h"
#include "OMNImove.h"

#include "BasicHeader/RBRawLAN.h"
#include "BasicHeader/JointInformation.h"

#include "device/RBDataBase.h"
#include "device/RBMotorController.h"
#include "device/RBFTSensor.h"
#include "device/JoyStick/joystickclass.h"
#include "device/JoyStick/joystickvariable.h"

using namespace std;
QString     settingFile;
inline void pushData(doubles &tar, double var){
    tar.push_back(var);
    tar.pop_front();
}
// Basic --------
int     IS_WORKING = false;
int     IS_CHILD = false;
int     IS_CAN_OK = false;

int FLAG_Debug = false;
pRBCORE_SHM_COMMAND     sharedCMD;
pRBCORE_SHM_REFERENCE   sharedREF;
pRBCORE_SHM_SENSOR      sharedSEN;
pRBCAN                  canHandler;
pRBLAN                  lanHandler;

// Initialize --------
int     RBCore_Initialize();
int     RBCore_SMInitialize();
int     RBCore_DBInitialize();
int     RBCore_CANInitialize();
int     RBCore_ThreadInitialize();
int     RBCore_Termination();
void    *RBCore_NRTThreadCon(void *);
void    *JoystickThread(void *);
ulong   nrtTaskCon;
ulong   nrtJoyCon;

// Database --------
DB_GENERAL      RBDataBase::_DB_GENERAL;
DB_MC           RBDataBase::_DB_MC[MAX_MC];
DB_FT           RBDataBase::_DB_FT[MAX_FT];

int     _VERSION;
int     _NO_OF_COMM_CH;
int     _NO_OF_MC;
int     _NO_OF_FT;

// Devices --------
RBMotorController   _DEV_MC[MAX_MC];
RBFTSensor          _DEV_FT[MAX_FT];

int _CANOUT_ENABLED = false;
int _ENCODER_ENABLED = false;
int _SENSOR_ENABLED = false;
int _REFERENCE_ENABLED = false;

long _ThreadCnt = 0;

bool StatusReadFlag[MAX_MC] = {0,};
bool ErrorClearStart = false;

// Thread Functions
void    THREAD_ReadSensor();
void    THREAD_ReadEncoder();
void    THREAD_ReadTemperature();
void    THREAD_ReadVoltage();
void    THREAD_ReadHomeError();

void    THREAD_RequestSensor();
void    THREAD_RequestEncoder();
void    THREAD_RequestTemperature();
void    THREAD_RequestVotypedefltage();

// Command Functions
void    RBCMD_InitCheckDevice();
void    RBCMD_InitWheel();
void    RBCMD_InitFindHome();
void    RBCMD_InitFetOnOff();
void    RBCMD_InitControlOnOff();
void    RBCMD_InitSetFingerModifier();

void    RBCMD_AttrSwitchingMode();
void    RBCMD_AttrFrictionCompensation();
void    RBCMD_AttrControlMode();

void    RBCMD_SensorEncoderReset();
void    RBCMD_SensorEncoderOnOff();
void    RBCMD_SensorSensorOnOff();
void    RBCMD_SensorFTNull();

void    RBCMD_MotionRefOnOff();
void    RBCMD_MotionMove();
void    RBCMD_MotionGainOverride();
void    RBCMD_MotionErrorClear();

void    RBCMD_CANEnableDisable();

//OmniWheel
enum{
    MANUAL_STOP = 0, MANUAL_WHEEL
};
OMNIMOVE    OMNI;
RBJoystick  *joy;
void THREAD_JOYSTICK();
void JOYreset();

int FLAG_JOYStart = false;
int FLAG_VELStart = false;
bool _isFirst = true;
bool Joystick_flag = false;
doubles FLWList(120);
doubles FRWList(120);
doubles BRWList(120);
doubles BLWList(120);

float Move_X = 0.;
float Move_Y = 0.;
float Move_R = 0.;
float MotorSpeed[4] = {0.0, };
const double zero[4] = {0.0,};

/* Debug data */
FILE *fp;
#define ROW 50000
#define COL 100
int     Save_Index;
double  Save_Data[COL][ROW];
void save();


void CatchSignals(int _signal)
{
    switch(_signal)
    {
    case SIGHUP:     // shell termination
    case SIGINT:     // Ctrl-c
    case SIGTERM:    // "kill" from shell
    case SIGKILL:
    case SIGSEGV:
        canHandler->Finish();
        usleep(1000*1000);
        IS_WORKING = false;
        break;
    }
}

int main(int argc, char *argv[])
{
    // Copyright
    cout << endl;
    cout << " \033[31m##############################################################################\n";
    cout << " #                                                                            #\n";
    cout << " #  Version for WRS competition                                               #\n";
    cout << " #  Copyright 2019 HuboLab                                                    #\n";
    cout << " #  developer: Yujin Heo                                                      #\n";
    cout << " #  E-mail: blike4@kaist.ac.kr                                                #\n";
    cout << " #                                                                            #\n";
    cout << " #  PODO Version 2.2                                                          #\n";
    cout << " #  Copyright 2016 Rainbow Robotics Co.                                       #\n";
    cout << " #  developer: Jeongsoo Lim                                                   #\n";
    cout << " #  E-mail: yjs0497@kaist.ac.kr                                               #\n";
    cout << " #                                                                            #\n";
    cout << " #  We touch the core!                                                        #\n";
    cout << " #                                                                            #\n";
    cout << " ##############################################################################\n";

    // Termination signal
    signal(SIGTERM, CatchSignals);       // "kill" from shell
    signal(SIGINT,  CatchSignals);       // Ctrl-c
    signal(SIGHUP,  CatchSignals);       // shell termination
    signal(SIGKILL, CatchSignals);
    signal(SIGSEGV, CatchSignals);

    // Block memory swapping
    mlockall(MCL_CURRENT|MCL_FUTURE);

    if(RBCore_Initialize() == false)
    {
        FILE_LOG(logERROR) << "Core Initialization Failed..";
        return 0;
    }

    while(IS_WORKING){
        usleep(100*1000);
        switch(sharedCMD->COMMAND.USER_COMMAND){
        case DAEMON_INIT_CHECK_DEVICE:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_INIT_CHECK_DEVICE";
            if(IS_CAN_OK)   {RBCMD_InitCheckDevice();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_INIT_FIND_HOME:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_INIT_FIND_HOME";
            if(IS_CAN_OK)   {RBCMD_InitFindHome();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_INIT_FET_ONOFF:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_INIT_FET_ONOFF";
            if(IS_CAN_OK)   {RBCMD_InitFetOnOff();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_INIT_CONTROL_ONOFF:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_INIT_CONTROL_ONOFF";
            if(IS_CAN_OK)   {RBCMD_InitControlOnOff();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_SENSOR_ENCODER_RESET:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_SENSOR_ENCODER_RESET";

            if(IS_CAN_OK)   {RBCMD_SensorEncoderReset();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_SENSOR_ENCODER_ONOFF:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_SENSOR_ENCODER_ONOFF";

            if(IS_CAN_OK)   {RBCMD_SensorEncoderOnOff();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_SENSOR_SENSOR_ONOFF:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_SENSOR_SENSOR_ONOFF";

            if(IS_CAN_OK)   {RBCMD_SensorSensorOnOff();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_SENSOR_FT_NULL:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_SENSOR_FT_NULL";

            if(IS_CAN_OK)   {RBCMD_SensorFTNull();}
            else            {FILE_LOG(logWARNING) << "CAN device not set";}

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_MOTION_REF_ONOFF:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_MOTION_REF_ONOFF";
            RBCMD_MotionRefOnOff();

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_MOTION_MOVE:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_MOTION_MOVE";
            RBCMD_MotionMove();

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_MOTION_GAIN_OVERRIDE:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_MOTION_GAIN_OVERRIDE";
            RBCMD_MotionGainOverride();

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_MOTION_ERROR_CLEAR:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_MOTION_ERROR_CLEAR";
                if(IS_CAN_OK)   {RBCMD_MotionErrorClear();}
                else            {FILE_LOG(logWARNING) << "CAN device not set";}

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_CAN_ENABLE_DISABLE:
        {
            FILE_LOG(logINFO) << "CMD: DAEMON_CAN_ENABLE_DISABLE";

                if(IS_CAN_OK)   {RBCMD_CANEnableDisable();}
                else            {FILE_LOG(logWARNING) << "CAN device not set";}

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case OMNIWHEEL_JOY_ON:
        {
            if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == JOY_CONNECT)
            {
                if(joy->ConnectJoy() == 1)
                    sharedSEN->JOY_Enabled = true;
                else
                    sharedSEN->JOY_Enabled = false;
            }
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case OMNIWHEEL_JOY_OFF:
        {
            FILE_LOG(logINFO) << "CMD: JOYSTICK THREAD STOP";
            JOYreset();
            FLAG_JOYStart = false;
            sharedSEN->WHEEL_STATE = OMNI_BREAK;
            Joystick_flag = false;

            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case OMNIWHEEL_JOY_START:
        {
            if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == JOY_ON)
            {
                FILE_LOG(logINFO) << "CMD: JOYSTICK THREAD START";
                JOYreset();
                Joystick_flag = true;
            }else
            {
                FILE_LOG(logINFO) << "CMD: JOYSTICK THREAD STOP";
                Joystick_flag = false;
            }
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case OMNIWHEEL_JOY_MOVE:
        {
            if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == JOY_START)
            {
                FILE_LOG(logINFO) << "CMD: JOYSTICK MOVE START";
                OMNI.InitWheelInfo();
                JOYreset();
                usleep(20*1000);
                FLAG_JOYStart = true;
                sharedSEN->WHEEL_STATE = OMNI_JOY_ON;
            }
            else if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == JOY_STOP)
            {
                FILE_LOG(logINFO) << "CMD: JOYSTICK MOVE STOP";
                JOYreset();
                FLAG_JOYStart = false;
                sharedSEN->WHEEL_STATE = OMNI_BREAK;
            }
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case OMNIWHEEL_MOVE_TO_GOAL:
        {
            FILE_LOG(logINFO) << "CMD: START WHEEL MOVE";
            OMNI.InitWheelInfo();
            usleep(20*1000);
            sharedSEN->WHEEL_STATE = OMNI_MOVING;
            OMNI.StartOMNImove(sharedCMD->omni_xm, sharedCMD->omni_ym, sharedCMD->omni_yawdeg);
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case OMNIWHEEL_VEL_MODE_START:
        {
            if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == 1)
            {
                FILE_LOG(logINFO) << "CMD: VEL MODE START";
                OMNI.InitWheelInfo();
                usleep(20*1000);
                FLAG_VELStart = true;
                sharedSEN->WHEEL_STATE = OMNI_VEL_ON;
            }
            else if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == 0)
            {
                FILE_LOG(logINFO) << "CMD: VEL MODE STOP";
                FLAG_VELStart = false;
                sharedSEN->WHEEL_STATE = OMNI_BREAK;
            }
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        case DAEMON_DATA_SAVE:
        {
            if(sharedCMD->COMMAND.USER_PARA_CHAR[0] == 0)
            {
                FILE_LOG(logSUCCESS) << "NEW COMMAND :: DATA RESET";
                Save_Index = 0;
                memset(&Save_Data,0,sizeof(double)*COL*ROW);
            }
            else
            {
                FILE_LOG(logSUCCESS) << "NEW COMMAND :: DATA SAVE";
                fp = fopen("dataDAEMON.txt","w");
                for(int i=0;i<Save_Index;i++)
                {
                    for(int j=0;j<COL;j++)fprintf(fp,"%g\t", Save_Data[j][i]);
                    fprintf(fp,"\n");
                }
                fclose(fp);
                FILE_LOG(logSUCCESS) << "Data Save Complete";
            }
            sharedCMD->COMMAND.USER_COMMAND = NO_ACT;
            break;
        }
        }
    }

    RBCore_Termination();
    usleep(1000*1000);
    return 0;
}

void *RBCore_NRTThreadCon(void *)
{
    while(IS_WORKING)
    {
        usleep(5*1000);
        _ThreadCnt++;
        double curRef[MAX_JOINT];

        /*======================= Read CAN ===========================*/
        if(canHandler->IsWorking())
        {
            THREAD_ReadSensor();
            THREAD_ReadEncoder();
            THREAD_ReadTemperature();
            THREAD_ReadHomeError();

            sharedSEN->CAN_Enabled = _CANOUT_ENABLED;
            sharedSEN->ENC_Enabled = _ENCODER_ENABLED;
            sharedSEN->SEN_Enabled = _SENSOR_ENABLED;
            sharedSEN->REF_Enabled = _REFERENCE_ENABLED;
        }


        /*======================= Write CAN ==========================*/
        int cnt=0;
        for(int i=0; i<_NO_OF_MC; i++)
        {
            if(_DEV_MC[i].CAN_CHANNEL == -1)
                continue;

            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++)
            {
                _DEV_MC[i].RBJoint_MoveJoint(j);
                sharedREF->JointReference[i][j] = _DEV_MC[i].MoveJoints[j].RefAngleCurrent;
                if(FLAG_Debug == true)
                {
                    printf("TH REF = %f    CurrentRef = %f  MCRef =%f\n",sharedREF->JointReference[1][0], sharedSEN->ENCODER[MC_GetID(BRW)][MC_GetCH(BRW)].CurrentReference, _DEV_MC[1].MoveJoints[0].RefAngleCurrent);
//                    FLAG_Debug = false;
                }
                _DEV_MC[i].Joints[j].Reference = sharedREF->JointReference[i][j];
                if(FLAG_Debug == true)
                {
                    printf("TH REF = %f    CurrentRef = %f  MCRef =%f\n",sharedREF->JointReference[1][0], sharedSEN->ENCODER[MC_GetID(BRW)][MC_GetCH(BRW)].CurrentReference, _DEV_MC[1].MoveJoints[0].RefAngleCurrent);
//                    FLAG_Debug = false;
                }

                sharedSEN->ENCODER[i][j].CurrentReference = sharedREF->JointReference[i][j];

                if(FLAG_Debug == true)
                {
                    printf("TH REF = %f    CurrentRef = %f  MCRef =%f\n",sharedREF->JointReference[1][0], sharedSEN->ENCODER[MC_GetID(BRW)][MC_GetCH(BRW)].CurrentReference, _DEV_MC[1].MoveJoints[0].RefAngleCurrent);
                    FLAG_Debug = false;
                }
                curRef[cnt] = sharedREF->JointReference[i][j];
                cnt++;
            }
            if(canHandler->IsWorking() && _CANOUT_ENABLED)// && _REFERENCE_ENABLED)
            { // CANOUT
                _DEV_MC[i].RBBoard_SendReference();
            }
        }


        /*====================== Manual CAN ==========================*/
        if(canHandler->IsWorking())
        {
            for(int i=0; i<MAX_MANUAL_CAN; i++)
            {
                if(sharedCMD->ManualCAN[i].status == MANUALCAN_NEW)
                {
                    sharedCMD->ManualCAN[i].status = MANUALCAN_WRITING;
                    RBCAN_MB mb;
                    mb.channel = sharedCMD->ManualCAN[i].channel;
                    mb.id = sharedCMD->ManualCAN[i].id;
                    mb.dlc = sharedCMD->ManualCAN[i].dlc;

                    if(mb.channel < 0)
                        continue;

                    for(int j=0; j<mb.dlc; j++)
                        mb.data[j] = sharedCMD->ManualCAN[i].data[j];

                    //0x6F = gain override
                    if(mb.data[0] != 0x6F || _CANOUT_ENABLED)  // CANOUT
                        canHandler->RBCAN_WriteData(mb);

                    sharedCMD->ManualCAN[i].status = MANUALCAN_EMPTY;
                }
            }
        }


        /*====================== Request CAN =========================*/
        if(canHandler->IsWorking())
        {
            THREAD_RequestSensor();
            THREAD_RequestEncoder();
            THREAD_RequestTemperature();
        }


        /*=================== OmniWheel Control ======================*/

        OMNI.SetCurReference(curRef);
        OMNI.OMNIThread();
        THREAD_JOYSTICK();
        if(OMNI.IsMovingWheel() == MOVING)
        {
            sharedSEN->WHEEL_STATE = OMNI_MOVING;
            _DEV_MC[MC_GetID(FLW)].MoveJoints[MC_GetCH(FLW)].RefAngleCurrent = OMNI.FLWinfo.Reference;
            _DEV_MC[MC_GetID(FRW)].MoveJoints[MC_GetCH(FRW)].RefAngleCurrent = OMNI.FRWinfo.Reference;
            _DEV_MC[MC_GetID(BRW)].MoveJoints[MC_GetCH(BRW)].RefAngleCurrent = OMNI.BRWinfo.Reference;
            _DEV_MC[MC_GetID(BLW)].MoveJoints[MC_GetCH(BLW)].RefAngleCurrent = OMNI.BLWinfo.Reference;
        }else if(OMNI.IsMovingWheel() == DONE)
        {
            sharedSEN->WHEEL_STATE = OMNI_MOVE_DONE;
            OMNI.SetOMNIstate(BREAK);
            printf("Daemon : omni done\n");
        }/*else
        {
            if(FLAG_JOYStart == false)
                sharedSEN->WHEEL_STATE = OMNI_BREAK;
        }*/
        save();
    }
}

void *JoystickThread(void *)
{
    while(1)
    {
        if(Joystick_flag == true)
        {
            if(joy->isConnected() == false)
            {
                printf("JoyStick connection failure...!!!\n");
                Joystick_flag = false;
                continue;
            }

            // AXIS Data
            sharedSEN->JOY_LJOG_RL = joy->JoyAxis[0];
            if(sharedSEN->JOY_LJOG_RL > 30000) sharedSEN->JOY_LJOG_RL = 32767;
            else if(sharedSEN->JOY_LJOG_RL < -30000) sharedSEN->JOY_LJOG_RL = -32767;
            else if(sharedSEN->JOY_LJOG_RL > -3000 && sharedSEN->JOY_LJOG_RL < 3000) sharedSEN->JOY_LJOG_RL = 0;

            sharedSEN->JOY_LJOG_UD = -joy->JoyAxis[1];
            if(sharedSEN->JOY_LJOG_UD > 30000) sharedSEN->JOY_LJOG_UD = 32767;
            else if(sharedSEN->JOY_LJOG_UD < -30000) sharedSEN->JOY_LJOG_UD = -32767;
            else if(sharedSEN->JOY_LJOG_UD > -3000 && sharedSEN->JOY_LJOG_UD < 3000) sharedSEN->JOY_LJOG_UD = 0;

            sharedSEN->JOY_RJOG_RL = joy->JoyAxis[3];
            if(sharedSEN->JOY_RJOG_RL > 30000) sharedSEN->JOY_RJOG_RL = 32767;
            else if(sharedSEN->JOY_RJOG_RL < -30000) sharedSEN->JOY_RJOG_RL = -32767;
            else if(sharedSEN->JOY_RJOG_RL > -3000 && sharedSEN->JOY_RJOG_RL < 3000) sharedSEN->JOY_RJOG_RL = 0;
        }
        else
        {
            sharedSEN->JOY_LJOG_RL = 0;
            sharedSEN->JOY_LJOG_UD = 0;
            sharedSEN->JOY_RJOG_RL = 0;
        }
        usleep(20*1000);
    }
}

void RBCMD_InitCheckDevice()
{
    for(int i=0; i<_NO_OF_MC; i++)
    {
        if(_DEV_MC[i].CAN_CHANNEL < 0)
            continue;
        _DEV_MC[i].RBBoard_CANCheck(RT_TIMER_PERIOD_MS);
        for(int j=0; j<MOTOR_2CH; j++)
        {
            sharedSEN->ENCODER[i][j].BoardConnection = _DEV_MC[i].ConnectionStatus;
        }
    }
    for(int i=0; i<_NO_OF_FT; i++)
    {
        if(_DEV_FT[i].CAN_CHANNEL < 0)
            continue;
        _DEV_FT[i].RBBoard_CANCheck(RT_TIMER_PERIOD_MS);
        sharedSEN->FT[i].BoardConnection = _DEV_FT[i].ConnectionStatus;
    }
}

void RBCMD_InitFindHome()
{
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];

    if(id == -1)
    { // All
        for(int i=0; i<_NO_OF_MC; i++)
        {
            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++)
            {
                _DEV_MC[i].RBJoint_ResetEncoder(j+1);

                _DEV_MC[i].MoveJoints[j].RefAngleCurrent = 0.0;
                sharedREF->JointReference[i][j] = 0.0;
                _DEV_MC[i].Joints[j].Reference = 0.0;
                OMNI.SetCurReference(zero);
                OMNI.InitWheelInfo();

                _DEV_MC[i].RBJoint_EnableFETDriver(j+1, 1);
                usleep(100);
                _DEV_MC[i].RBJoint_FindHome(j+1);
            }
        }
    }else
    {  // Each
        _DEV_MC[id].RBJoint_ResetEncoder(ch+1);

        _DEV_MC[id].MoveJoints[ch].RefAngleCurrent = 0.0;
        sharedREF->JointReference[id][ch] = 0.0;
        _DEV_MC[id].Joints[ch].Reference = 0.0;
        OMNI.SetCurReference(zero);
        OMNI.InitWheelInfo();

        _DEV_MC[id].RBJoint_EnableFETDriver(ch+1, 1);
        usleep(100);
        _DEV_MC[id].RBJoint_FindHome(ch+1);
    }

}



void RBCMD_InitFetOnOff(){
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];
    int onoff = sharedCMD->COMMAND.USER_PARA_CHAR[2];   // 0->off
    if(onoff != 0) onoff = 1;

    if(id == -1){ // All
        for(int i=0; i<_NO_OF_MC; i++){
            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
                _DEV_MC[i].RBJoint_EnableFETDriver(j+1, onoff);
            }
        }
    }else{  // Each
        _DEV_MC[id].RBJoint_EnableFETDriver(ch+1, onoff);
    }
}

void RBCMD_InitControlOnOff(){
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];
    int onoff = sharedCMD->COMMAND.USER_PARA_CHAR[2];   // 0->off
    if(onoff != 0) onoff = 1;

    if(id == -1){ // All
        for(int i=0; i<_NO_OF_MC; i++){
            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
                _DEV_MC[i].RBJoint_EnableFeedbackControl(j+1, onoff);
            }
        }
    }else{  // Each
        _DEV_MC[id].RBJoint_EnableFeedbackControl(ch+1, onoff);
    }
}

void RBCMD_SensorEncoderReset()
{
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];

    if(id == -1)
    { // All
        for(int i=0; i<_NO_OF_MC; i++)
        {
            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++)
            {
                _DEV_MC[i].RBJoint_ResetEncoder(j+1);

                sharedREF->JointReference[i][j] = 0.0;
                _DEV_MC[i].Joints[j].Reference = 0.0;
                _DEV_MC[i].MoveJoints[j].RefAngleCurrent = 0.0;

                OMNI.SetCurReference(zero);
                OMNI.InitWheelInfo();
            }
        }
    }else
    {  // Each
        _DEV_MC[id].RBJoint_ResetEncoder(ch+1);
        sharedREF->JointReference[id][ch] = 0.0;
        _DEV_MC[id].Joints[ch].Reference = 0.0;
        _DEV_MC[id].MoveJoints[ch].RefAngleCurrent = 0.0;
        OMNI.SetCurReference(zero);
        OMNI.InitWheelInfo();
    }
}

void RBCMD_SensorEncoderOnOff(){
    int onoff = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    if(onoff != 0) onoff = 1;

    if(onoff == 1)
    { //on
        for(int i=0; i<_NO_OF_MC; i++)
        {
            _DEV_MC[i].RBBoard_RequestEncoder(1);   //continuous
            printf("encoder continuous\n");
            _ENCODER_ENABLED = true;
        }
    }else{  //off
        for(int i=0; i<_NO_OF_MC; i++){
            _DEV_MC[i].RBBoard_RequestEncoder(0);   //oneshot
            printf("encoder oneshot\n");
            _ENCODER_ENABLED = false;
        }
    }
}

void RBCMD_SensorSensorOnOff(){
    int onoff = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    if(onoff != 0) onoff = 1;

    if(onoff == 1){ //on
        _SENSOR_ENABLED = true;
    }else{  //off
        _SENSOR_ENABLED = false;
    }
}

void RBCMD_SensorFTNull(){
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];

    if(id == -1){   //All
        for(int i=0; i<_NO_OF_FT; i++){
            _DEV_FT[i].RBFT_Nulling(0);
        }
    }else{  //Each
        _DEV_FT[id].RBFT_Nulling(0);
    }
}

void RBCMD_MotionRefOnOff()
{
    int onoff = sharedCMD->COMMAND.USER_PARA_CHAR[0];


    for(int i=0; i<_NO_OF_MC; i++)
    {
        for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++)
        {
           // _DEV_MC[i].MoveJoints[j].RefAngleCurrent = sharedSEN->ENCODER[MC_GetID(i)][MC_GetCH(i)].CurrentPosition;
        }
        _DEV_MC[i].RBBoard_ReferenceOutEnable(onoff);
    }
    usleep(20*1000);
    _REFERENCE_ENABLED = onoff;
}

void RBCMD_MotionMove()
{
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];
    int mode = sharedCMD->COMMAND.USER_PARA_CHAR[2];
    float time = sharedCMD->COMMAND.USER_PARA_FLOAT[0];
    float ang = sharedCMD->COMMAND.USER_PARA_FLOAT[1];

    _DEV_MC[id].MoveJoints[ch].MoveFlag = false;
    _DEV_MC[id].MoveJoints[ch].RefAngleCurrent = sharedREF->JointReference[id][ch] = sharedSEN->ENCODER[id][ch].CurrentReference;
    _DEV_MC[id].RBJoint_SetMoveJoint(ch, ang, time, mode);
}

void RBCMD_MotionGainOverride()
{
    if(!_CANOUT_ENABLED)
        return;

    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];
    float time = sharedCMD->COMMAND.USER_PARA_FLOAT[0];
    float gain = sharedCMD->COMMAND.USER_PARA_FLOAT[1];

    _DEV_MC[id].RBJoint_GainOverride(ch+1, gain, time);
}

void RBCMD_MotionErrorClear()
{
    int id = sharedCMD->COMMAND.USER_PARA_CHAR[0];
    int ch = sharedCMD->COMMAND.USER_PARA_CHAR[1];
    int mode = sharedCMD->COMMAND.USER_PARA_CHAR[2];
    if(mode != 0) mode = 1;


    ErrorClearStart = true;
    if(mode == 0){  // just error clear
        if(id == -1){ // All
            for(int i=0; i<_NO_OF_MC; i++){
                for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
                    _DEV_MC[i].RBJoint_ClearErrorFlag(j+1);
                }
            }
        }else{  // Each
            _DEV_MC[id].RBJoint_ClearErrorFlag(ch+1);
        }
    }else{          // error clear + joint recovery
        // reference out disable & get motion ownership for all joints
        for(int i=0; i<_NO_OF_MC; i++){
            _DEV_MC[i].RBBoard_ReferenceOutEnable(false);
        }

        // encoder enable
        for(int i=0; i<_NO_OF_MC; i++){
            _DEV_MC[i].RBBoard_RequestEncoder(1);
        }

        // sleep for encoder read
        usleep(30*1000);

        for(int i=0; i<_NO_OF_MC; i++){
            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
                // update reference with encoder (exception RWH LWH RHAND LHAND)
                if(i == 4 || i == 10){
                    _DEV_MC[i].MoveJoints[j].RefAngleCurrent = 0.0;
                }else{
                    _DEV_MC[i].MoveJoints[j].RefAngleCurrent = sharedSEN->ENCODER[i][j].CurrentPosition;
                }
            }
        }

        if(id == -1){ // All
            for(int i=0; i<_NO_OF_MC; i++){
                for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
                    // error clear
                    _DEV_MC[i].RBJoint_ClearErrorFlag(j+1);

                    // FET on & CTRL on
                    _DEV_MC[i].RBJoint_EnableFETDriver(j+1, true);
                    _DEV_MC[i].RBJoint_EnableFeedbackControl(j+1, true);
                }
            }
        }else{  // Each
            // error clear
            _DEV_MC[id].RBJoint_ClearErrorFlag(ch+1);

            // FET on & CTRL on
            _DEV_MC[id].RBJoint_EnableFETDriver(ch+1, true);
            _DEV_MC[id].RBJoint_EnableFeedbackControl(ch+1, true);
        }

        // wait for settling
        usleep(10*1000);

        // reference out enable
        for(int i=0; i<_NO_OF_MC; i++){
            for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
                _DEV_MC[i].RBBoard_ReferenceOutEnable(true);
            }
        }
    }
    ErrorClearStart = false;
}

void RBCMD_CANEnableDisable()
{
    _CANOUT_ENABLED = sharedCMD->COMMAND.USER_PARA_CHAR[0];
}

//==============================================================================
int RBCore_Initialize(void)
{
    cout << endl;
    FILE_LOG(logERROR) << "==========Initializing===========";

    IS_WORKING = true;

    // Shared Memory initialize
    if(RBCore_SMInitialize() == false)
        return false;

    // Load RBCore configuration file
    if(RBCore_DBInitialize() == false)
        return false;

    // CAN & LAN Communcation initialize
    RBCore_CANInitialize();

    // Real-time thread initialize
    if(RBCore_ThreadInitialize() == false)
        return false;

    joy = new RBJoystick();
    if(joy->ConnectJoy() == 1)
    {
        sharedSEN->JOY_Enabled = true;
    }else
    {
        sharedSEN->JOY_Enabled = false;
    }

    FILE_LOG(logERROR) << "=================================";
    cout << endl;

    IS_WORKING = true;
    return true;
}

//---------------
int RBCore_SMInitialize(){
    shm_unlink(RBCORE_SHM_NAME_REFERENCE);
    shm_unlink(RBCORE_SHM_NAME_SENSOR);
    shm_unlink(RBCORE_SHM_NAME_COMMAND);

    int shmFD;
    // Core Shared Memory Creation [Reference]==================================
    shmFD = shm_open(RBCORE_SHM_NAME_REFERENCE, O_CREAT|O_RDWR, 0666);
    if(shmFD == -1){
        FILE_LOG(logERROR) << "Fail to open core shared memory [Reference]";
        return -1;
    }else{
        if(ftruncate(shmFD, sizeof(RBCORE_SHM_REFERENCE)) == -1){
            FILE_LOG(logERROR) << "Fail to truncate core shared memory [Reference]";
            return -1;
        }else{
            sharedREF = (pRBCORE_SHM_REFERENCE)mmap(0, sizeof(RBCORE_SHM_REFERENCE), PROT_READ|PROT_WRITE, MAP_SHARED, shmFD, 0);
            if(sharedREF == (void*)-1){
                FILE_LOG(logERROR) << "Fail to mapping core shared memory [Reference]";
                return -1;
            }
        }
    }
    FILE_LOG(logSUCCESS) << "Core shared memory creation = OK [Reference]";
    // =========================================================================

    // Core Shared Memory Creation [Sensor]=====================================
    shmFD = shm_open(RBCORE_SHM_NAME_SENSOR, O_CREAT|O_RDWR, 0666);
    if(shmFD == -1){
        FILE_LOG(logERROR) << "Fail to open core shared memory [Sensor]";
        return -1;
    }else{
        if(ftruncate(shmFD, sizeof(RBCORE_SHM_SENSOR)) == -1){
            FILE_LOG(logERROR) << "Fail to truncate core shared memory [Sensor]";
            return -1;
        }else{
            sharedSEN = (pRBCORE_SHM_SENSOR)mmap(0, sizeof(RBCORE_SHM_SENSOR), PROT_READ|PROT_WRITE, MAP_SHARED, shmFD, 0);
            if(sharedSEN == (void*)-1){
                FILE_LOG(logERROR) << "Fail to mapping core shared memory [Sensor]";
                return -1;
            }
        }
    }
    FILE_LOG(logSUCCESS) << "Core shared memory creation = OK [Sensor]";
    // =========================================================================

    // Core Shared Memory Creation [Command]====================================
    shmFD = shm_open(RBCORE_SHM_NAME_COMMAND, O_CREAT|O_RDWR, 0666);
    if(shmFD == -1){
        FILE_LOG(logERROR) << "Fail to open core shared memory [Command]";
        return -1;
    }else{
        if(ftruncate(shmFD, sizeof(RBCORE_SHM_COMMAND)) == -1){
            FILE_LOG(logERROR) << "Fail to truncate core shared memory [Command]";
            return -1;
        }else{
            sharedCMD = (pRBCORE_SHM_COMMAND)mmap(0, sizeof(RBCORE_SHM_COMMAND), PROT_READ|PROT_WRITE, MAP_SHARED, shmFD, 0);
            if(sharedCMD == (void*)-1){
                FILE_LOG(logERROR) << "Fail to mapping core shared memory [Command]";
                return -1;
            }
        }
    }
    FILE_LOG(logSUCCESS) << "Core shared memory creation = OK [Command]";
    // =========================================================================

    return true;
}
//---------------
int RBCore_DBInitialize(){
    RBDataBase DB;
    DB.SetFilename("Core_Config.db");
    if(DB.OpenDB() == false){
        FILE_LOG(logERROR) << "Fail to load database file";
        return false;
    }

    _VERSION        = RBDataBase::_DB_GENERAL.VERSION;
    _NO_OF_COMM_CH  = RBDataBase::_DB_GENERAL.NO_OF_COMM_CH;
    _NO_OF_MC       = RBDataBase::_DB_GENERAL.NO_OF_MC;
    _NO_OF_FT       = RBDataBase::_DB_GENERAL.NO_OF_FT;
    FILE_LOG(logSUCCESS) << "Core load database = OK";

    std::cout << "----------------------------------" << std::endl;
    std::cout << "     VERSION       : " << _VERSION << std::endl;
    std::cout << "     NO_OF_COMM_CH : " << _NO_OF_COMM_CH << std::endl;
    std::cout << "     NO_OF_MC      : " << _NO_OF_MC << std::endl;
    std::cout << "     NO_OF_FT      : " << _NO_OF_FT << std::endl;
    std::cout << "----------------------------------" << std::endl;

    for(int i=0; i<_NO_OF_MC; i++)   _DEV_MC[i].RBBoard_GetDBData(RBDataBase::_DB_MC[i]);
    for(int i=0; i<_NO_OF_FT; i++)   _DEV_FT[i].RBBoard_GetDBData(RBDataBase::_DB_FT[i]);

    return true;
}
//---------------
int RBCore_CANInitialize(){
    canHandler = new RBCAN(_NO_OF_COMM_CH);

    if(canHandler->IsWorking() == false){
        IS_CAN_OK = false;
        return false;
    }else
    {
        _CANOUT_ENABLED = true;
        for(int i=0; i<_NO_OF_MC; i++)   _DEV_MC[i].RBMC_AddCANMailBox();
        for(int i=0; i<_NO_OF_FT; i++)   _DEV_FT[i].RBFT_AddCANMailBox();
        IS_CAN_OK = true;
        return true;
    }
}
//---------------
int RBCore_ThreadInitialize(){
    int theadID = pthread_create(&nrtTaskCon, NULL, &RBCore_NRTThreadCon, NULL);
    if(theadID < 0){
        FILE_LOG(logERROR) << "Fail to create core non real-time thread";
        return false;
    }
    FILE_LOG(logSUCCESS) << "Core non real-time thread start = OK";

    theadID = pthread_create(&nrtJoyCon, NULL, &JoystickThread, NULL);
    if(theadID < 0){
        FILE_LOG(logERROR) << "Fail to create core Joystick thread";
        return false;
    }
    FILE_LOG(logSUCCESS) << "Core Joystick thread start = OK";
    return true;
}

//---------------
int RBCore_Termination(){
    if(IS_CHILD)
        return true;

    shm_unlink(RBCORE_SHM_NAME_REFERENCE);
    shm_unlink(RBCORE_SHM_NAME_SENSOR);
    shm_unlink(RBCORE_SHM_NAME_COMMAND);
    FILE_LOG(logERROR) << "RBCore will be terminated..";
    return true;
}
//==============================================================================


void THREAD_ReadSensor(){
    if(_SENSOR_ENABLED == false)
        return;

    for(int i=0; i<_NO_OF_FT; i++){
        _DEV_FT[i].RBFT_ReadData();
        sharedSEN->FT[i].Fx     = _DEV_FT[i].FX;
        sharedSEN->FT[i].Fy     = _DEV_FT[i].FY;
        sharedSEN->FT[i].Fz     = _DEV_FT[i].FZ;
        sharedSEN->FT[i].Mx     = _DEV_FT[i].MX;
        sharedSEN->FT[i].My     = _DEV_FT[i].MY;
        sharedSEN->FT[i].Mz     = _DEV_FT[i].MZ;

        if(i == 0){
            sharedSEN->FT[i].Fz = _DEV_FT[i].FZ * 8.0/7.0;
        }

        if(_DEV_FT[i].SENSOR_TYPE == 0){
            sharedSEN->FT[i].Roll       = _DEV_FT[i].ROLL;
            sharedSEN->FT[i].RollVel    = _DEV_FT[i].VelRoll;
            sharedSEN->FT[i].Pitch      = _DEV_FT[i].PITCH;
            sharedSEN->FT[i].PitchVel   = _DEV_FT[i].VelPitch;
        }
    }
}

void THREAD_RequestSensor(){
    if(_SENSOR_ENABLED){
        for(int i=0; i<_NO_OF_FT; i++){
            if(_DEV_FT[i].SENSOR_TYPE == 0x00){
                _DEV_FT[i].RBFT_RequestData(0x04);
                _DEV_FT[i].RBFT_RequestData(0x00);
            }else{
                _DEV_FT[i].RBFT_RequestData(0x00);
            }
        }
    }
}

void THREAD_ReadEncoder(){
    if(_ENCODER_ENABLED == false)
        return;

    for(int i=0; i<_NO_OF_MC; i++){
        _DEV_MC[i].RBBoard_ReadEncoderData();
        for(int j=0; j<_DEV_MC[i].MOTOR_CHANNEL; j++){
            sharedSEN->ENCODER[i][j].CurrentPosition = _DEV_MC[i].Joints[j].CurrentPosition;
            sharedSEN->ENCODER[i][j].CurrentVelocity = _DEV_MC[i].Joints[j].CurrentVelocity;
        }
    }
}

void THREAD_RequestEncoder(){
    if(_ENCODER_ENABLED){
        ;
    }
}

void THREAD_ReadTemperature(){
    if(_SENSOR_ENABLED){
        int mc = _ThreadCnt%_NO_OF_MC;
        _DEV_MC[mc].RBBoard_ReadTemperature();
        sharedSEN->MCTemperature[mc] = _DEV_MC[mc].BoardTemperature;
        for(int i=0; i<_DEV_MC[mc].MOTOR_CHANNEL; i++){
            sharedSEN->MotorTemperature[mc][i] = _DEV_MC[mc].Joints[i].Temperature;
        }
    }
}

void THREAD_RequestTemperature(){
    int mc = _ThreadCnt%_NO_OF_MC;

    // temperature
    if(_SENSOR_ENABLED){
        _DEV_MC[mc].RBBoard_RequestTemperature();
    }

    // home & error
    if(_DEV_MC[mc].ConnectionStatus == true){
        StatusReadFlag[mc] = false;
        _DEV_MC[mc].RBBoard_RequestStatus();
    }
}



void THREAD_ReadHomeError()
{
    static unsigned int StatusErrorCnt[MAX_MC] = {0,};

    //if(_SENSOR_ENABLED){
    int mc = _ThreadCnt%_NO_OF_MC;

    if(_DEV_MC[mc].ConnectionStatus == true){
        if(_DEV_MC[mc].RBBoard_GetStatus() == true)

        {
            StatusErrorCnt[mc] = 0;
            StatusReadFlag[mc] = true;

            for(int j=0; j<_DEV_MC[mc].MOTOR_CHANNEL; j++)
            {
                sharedSEN->MCStatus[mc][j] = _DEV_MC[mc].Joints[j].CurrentStatus;
                sharedSEN->MCStatus[mc][j].b.CUR = 0;
                if(sharedSEN->MCStatus[mc][j].b.BIG == 1 || sharedSEN->MCStatus[mc][j].b.INP == 1 ||
                        sharedSEN->MCStatus[mc][j].b.ENC == 1 || sharedSEN->MCStatus[mc][j].b.JAM == 1 )
                {
                    if(ErrorClearStart == false)
                    {
                        //FILE_LOG(logERROR) << "Need Recover Activated.. Reason: Servo[" << mc << ", " << j << "] Off with Error(" << (int)(sharedSEN->MCStatus[mc][j].B[1]) << ")";
                        //NeedJointRecover = true;
                    }
                }
            }
        }else if(_SENSOR_ENABLED && StatusReadFlag[mc] == false)
        {
            StatusErrorCnt[mc]++;
            if(StatusErrorCnt[mc] > 3){
                if(ErrorClearStart == false)
                {
                    for(int j=0; j<_DEV_MC[mc].MOTOR_CHANNEL; j++){
                        sharedSEN->MCStatus[mc][j].b.CUR = 1;
                    }
                    //FILE_LOG(logERROR) << "Need Recover Activated.. Reason: No Status Return";
                    //NeedCANRecover = true;
                }
            }

            if(StatusErrorCnt[mc]%3 == 0)
                FILE_LOG(logERROR) << "Status Get Error from Board " << mc << " Cnt : " << StatusErrorCnt[mc];

        }
    }
}

void THREAD_JOYSTICK()
{
    if(FLAG_JOYStart == true)
    {
        if(fabs(sharedSEN->JOY_RJOG_RL) >35000) sharedSEN->JOY_RJOG_RL = 0;
        if(fabs(sharedSEN->JOY_LJOG_RL) >35000) sharedSEN->JOY_LJOG_RL = 0;
        if(fabs(sharedSEN->JOY_LJOG_UD) >35000) sharedSEN->JOY_LJOG_UD = 0;

        static int tempcnt = 0;
        if(sharedSEN->JOY_LJOG_RL == 0 && sharedSEN->JOY_LJOG_UD == 0 && sharedSEN->JOY_RJOG_RL == 0 && tempcnt > 20){
            tempcnt++;
            pushData(FLWList, 0.0);
            pushData(FRWList, 0.0);
            pushData(BRWList, 0.0);
            pushData(BLWList, 0.0);
        } else
        {
            float Kspeed = 0.1;//2.5;
            tempcnt = 0;

            Move_X = (-(float)sharedSEN->JOY_LJOG_UD/-32767.f)*Kspeed;
            Move_Y = (-(float)sharedSEN->JOY_LJOG_RL/32767.f)*Kspeed;
            Move_R = ((float)sharedSEN->JOY_RJOG_RL/32767.f)*Kspeed;

            doubles vwheel(4);
            vwheel = OMNI.Omni_CalVel_Wheel(Move_X, Move_Y, Move_R);

            MotorSpeed[0] = vwheel[0];
            MotorSpeed[1] = vwheel[1];
            MotorSpeed[2] = vwheel[2];
            MotorSpeed[3] = vwheel[3];

            pushData(FLWList, MotorSpeed[0]);
            pushData(FRWList, MotorSpeed[1]);
            pushData(BRWList, MotorSpeed[2]);
            pushData(BLWList, MotorSpeed[3]);

            if(Move_X != 0 || Move_Y != 0 || Move_R != 0)
            {
                printf("X = %f, Y = %f, Move_R = %f\n",Move_X, Move_Y, Move_R);
            }
        }

        double temp1 = 0.0;
        double temp2 = 0.0;
        double temp3 = 0.0;
        double temp4 = 0.0;
        for(int i=0; i<120; i++)
        {
            temp1 += FLWList[i];
            temp2 += FRWList[i];
            temp3 += BRWList[i];
            temp4 += BLWList[i];
        }
        OMNI.FLWinfo.WheelVel_ms = temp1/120.0;
        OMNI.FRWinfo.WheelVel_ms = temp2/120.0;
        OMNI.BRWinfo.WheelVel_ms = temp3/120.0;
        OMNI.BLWinfo.WheelVel_ms = temp4/120.0;

        OMNI.FLWinfo.MoveDistance_m += OMNI.FLWinfo.WheelVel_ms;
        OMNI.FRWinfo.MoveDistance_m += OMNI.FRWinfo.WheelVel_ms;
        OMNI.BRWinfo.MoveDistance_m += OMNI.BRWinfo.WheelVel_ms;
        OMNI.BLWinfo.MoveDistance_m += OMNI.BLWinfo.WheelVel_ms;

        _DEV_MC[MC_GetID(FLW)].MoveJoints[MC_GetCH(FLW)].RefAngleCurrent = OMNI.FLWinfo.InitRef_Deg + OMNI.FLWinfo.MoveDistance_m;
        _DEV_MC[MC_GetID(FRW)].MoveJoints[MC_GetCH(FRW)].RefAngleCurrent = OMNI.FRWinfo.InitRef_Deg + OMNI.FRWinfo.MoveDistance_m;
        _DEV_MC[MC_GetID(BRW)].MoveJoints[MC_GetCH(BRW)].RefAngleCurrent = OMNI.BRWinfo.InitRef_Deg + OMNI.BRWinfo.MoveDistance_m;
        _DEV_MC[MC_GetID(BLW)].MoveJoints[MC_GetCH(BLW)].RefAngleCurrent = OMNI.BLWinfo.InitRef_Deg + OMNI.BLWinfo.MoveDistance_m;

    }

    if(FLAG_VELStart == true)
    {

        float Kspeed = 0.1;

        Move_X = (float)sharedCMD->vel_x*Kspeed;
        Move_Y = (float)sharedCMD->vel_y*Kspeed;
        Move_R = (float)sharedCMD->vel_r*Kspeed;

        doubles vwheel(4);
        vwheel = OMNI.Omni_CalVel_Wheel(Move_X, Move_Y, Move_R);

        MotorSpeed[0] = vwheel[0];
        MotorSpeed[1] = vwheel[1];
        MotorSpeed[2] = vwheel[2];
        MotorSpeed[3] = vwheel[3];

        pushData(FLWList, MotorSpeed[0]);
        pushData(FRWList, MotorSpeed[1]);
        pushData(BRWList, MotorSpeed[2]);
        pushData(BLWList, MotorSpeed[3]);

        if(Move_X != 0 || Move_Y != 0 || Move_R != 0)
        {
            printf("X = %f, Y = %f, Move_R = %f\n",Move_X, Move_Y, Move_R);
        }


        double temp1 = 0.0;
        double temp2 = 0.0;
        double temp3 = 0.0;
        double temp4 = 0.0;
        for(int i=0; i<120; i++)
        {
            temp1 += FLWList[i];
            temp2 += FRWList[i];
            temp3 += BRWList[i];
            temp4 += BLWList[i];
        }
        OMNI.FLWinfo.WheelVel_ms = temp1/120.0;
        OMNI.FRWinfo.WheelVel_ms = temp2/120.0;
        OMNI.BRWinfo.WheelVel_ms = temp3/120.0;
        OMNI.BLWinfo.WheelVel_ms = temp4/120.0;

        OMNI.FLWinfo.MoveDistance_m += OMNI.FLWinfo.WheelVel_ms;
        OMNI.FRWinfo.MoveDistance_m += OMNI.FRWinfo.WheelVel_ms;
        OMNI.BRWinfo.MoveDistance_m += OMNI.BRWinfo.WheelVel_ms;
        OMNI.BLWinfo.MoveDistance_m += OMNI.BLWinfo.WheelVel_ms;

        _DEV_MC[MC_GetID(FLW)].MoveJoints[MC_GetCH(FLW)].RefAngleCurrent = OMNI.FLWinfo.InitRef_Deg + OMNI.FLWinfo.MoveDistance_m;
        _DEV_MC[MC_GetID(FRW)].MoveJoints[MC_GetCH(FRW)].RefAngleCurrent = OMNI.FRWinfo.InitRef_Deg + OMNI.FRWinfo.MoveDistance_m;
        _DEV_MC[MC_GetID(BRW)].MoveJoints[MC_GetCH(BRW)].RefAngleCurrent = OMNI.BRWinfo.InitRef_Deg + OMNI.BRWinfo.MoveDistance_m;
        _DEV_MC[MC_GetID(BLW)].MoveJoints[MC_GetCH(BLW)].RefAngleCurrent = OMNI.BLWinfo.InitRef_Deg + OMNI.BLWinfo.MoveDistance_m;
    }

}

void JOYreset()
{
    sharedSEN->JOY_LJOG_RL = 0;
    sharedSEN->JOY_RJOG_RL = 0;
    sharedSEN->JOY_LJOG_UD = 0;

    for(int i=0; i<120; i++)
    {
        pushData(FLWList, 0.0);
        pushData(FRWList, 0.0);
        pushData(BRWList, 0.0);
        pushData(BLWList, 0.0);
    }
}

void save()
{
    if(Save_Index < ROW)
    {
        Save_Data[0][Save_Index] = OMNI.OMNIinfo.Probot.X;
        Save_Data[1][Save_Index] = OMNI.OMNIinfo.Probot.Y;
        Save_Data[2][Save_Index] = OMNI.OMNIinfo.Probot.Theta;

        Save_Data[3][Save_Index] = OMNI.OMNIinfo.Pin.X;
        Save_Data[4][Save_Index] = OMNI.OMNIinfo.Pin.Y;
        Save_Data[5][Save_Index] = OMNI.OMNIinfo.Pin.Theta;

        Save_Data[6][Save_Index] = OMNI.OMNIinfo.Vrobot.X;
        Save_Data[7][Save_Index] = OMNI.OMNIinfo.Vrobot.Y;
        Save_Data[8][Save_Index] = OMNI.OMNIinfo.Vrobot.Theta;

        Save_Data[9][Save_Index] =  OMNI.OMNIinfo.CurSec;
        Save_Data[10][Save_Index] = OMNI.OMNIinfo.GoalSec;
        Save_Data[11][Save_Index] = OMNI.OMNIinfo.GoalSecR;

        Save_Data[12][Save_Index] = OMNI.FLWinfo.WheelVel_ms;
        Save_Data[13][Save_Index] = OMNI.FRWinfo.WheelVel_ms;
        Save_Data[14][Save_Index] = OMNI.BRWinfo.WheelVel_ms;
        Save_Data[15][Save_Index] = OMNI.BLWinfo.WheelVel_ms;

        Save_Data[16][Save_Index] = OMNI.FLWinfo.MoveDistance_m;
        Save_Data[17][Save_Index] = OMNI.FRWinfo.MoveDistance_m;
        Save_Data[18][Save_Index] = OMNI.BRWinfo.MoveDistance_m;
        Save_Data[19][Save_Index] = OMNI.BLWinfo.MoveDistance_m;

        Save_Data[20][Save_Index] = OMNI.FLWinfo.Reference;
        Save_Data[21][Save_Index] = OMNI.FRWinfo.Reference;
        Save_Data[22][Save_Index] = OMNI.BRWinfo.Reference;
        Save_Data[23][Save_Index] = OMNI.BLWinfo.Reference;

        Save_Data[24][Save_Index] = sharedSEN->ENCODER[MC_GetID(FLW)][MC_GetCH(FLW)].CurrentPosition;
        Save_Data[25][Save_Index] = sharedSEN->ENCODER[MC_GetID(FRW)][MC_GetCH(FRW)].CurrentPosition;
        Save_Data[26][Save_Index] = sharedSEN->ENCODER[MC_GetID(BRW)][MC_GetCH(BRW)].CurrentPosition;
        Save_Data[27][Save_Index] = sharedSEN->ENCODER[MC_GetID(BLW)][MC_GetCH(BLW)].CurrentPosition;


        Save_Index++;
        if(Save_Index >= ROW) Save_Index = 0;
    }
}
