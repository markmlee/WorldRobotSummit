#include "CANconnect.h"

CANconnect::CANconnect()
{
    CANFlag = CreateSharedMemory();
}

CANconnect::~CANconnect()
{
    printf("delete CANconnect\n");
    shm_unlink(RBCORE_SHM_NAME_REFERENCE);
    shm_unlink(RBCORE_SHM_NAME_SENSOR);
    shm_unlink(RBCORE_SHM_NAME_COMMAND);
}

int CANconnect::isConnectCAN()
{
    return CANFlag;
}

int CANconnect::CreateSharedMemory()
{
    mlockall(MCL_CURRENT|MCL_FUTURE);

    int ret = 1;

    int shmFD;
    // Core Shared Memory Creation [Reference]==================================
    shmFD = shm_open(RBCORE_SHM_NAME_REFERENCE, O_RDWR, 0666);
    if(shmFD == -1){
        FILE_LOG(logERROR) << "Fail to open core shared memory [Reference]";
        ret = -1;
    }else{
        if(ftruncate(shmFD, sizeof(RBCORE_SHM_REFERENCE)) == -1){
            FILE_LOG(logERROR) << "Fail to truncate core shared memory [Reference]";
            ret = -1;
        }else{
            sharedREF = (pRBCORE_SHM_REFERENCE)mmap(0, sizeof(RBCORE_SHM_REFERENCE), PROT_READ, MAP_SHARED, shmFD, 0);
            if(sharedREF == (void*)-1)
            {
                FILE_LOG(logERROR) << "Fail to mapping core shared memory [Reference]";
                ret = -1;
            }
            FILE_LOG(logSUCCESS) << "Core shared memory creation = OK [Reference]";
        }
    }
    // =========================================================================

    // Core Shared Memory Creation [Sensor]=====================================
    shmFD = shm_open(RBCORE_SHM_NAME_SENSOR, O_RDWR, 0666);
    if(shmFD == -1){
        FILE_LOG(logERROR) << "Fail to open core shared memory [Sensor]";
        ret = -1;
    }else{
        if(ftruncate(shmFD, sizeof(RBCORE_SHM_SENSOR)) == -1){
            FILE_LOG(logERROR) << "Fail to truncate core shared memory [Sensor]";
            ret = -1;
        }else{
            sharedSEN = (pRBCORE_SHM_SENSOR)mmap(0, sizeof(RBCORE_SHM_SENSOR), PROT_READ|PROT_WRITE, MAP_SHARED, shmFD, 0);
            if(sharedSEN == (void*)-1)
            {
                FILE_LOG(logERROR) << "Fail to mapping core shared memory [Sensor]";
                ret = -1;
            }
            FILE_LOG(logSUCCESS) << "Core shared memory creation = OK [Sensor]";
        }
    }
    // =========================================================================

    // Core Shared Memory Creation [Command]====================================
    shmFD = shm_open(RBCORE_SHM_NAME_COMMAND, O_RDWR, 0666);
    if(shmFD == -1){
        FILE_LOG(logERROR) << "Fail to open core shared memory [Command]";
        ret = -1;
    }else{
        if(ftruncate(shmFD, sizeof(RBCORE_SHM_COMMAND)) == -1){
            FILE_LOG(logERROR) << "Fail to truncate core shared memory [Command]";
            ret = -1;
        }else{
            sharedCMD = (pRBCORE_SHM_COMMAND)mmap(0, sizeof(RBCORE_SHM_COMMAND), PROT_READ|PROT_WRITE, MAP_SHARED, shmFD, 0);
            if(sharedCMD == (void*)-1)
            {
                FILE_LOG(logERROR) << "Fail to mapping core shared memory [Command]";
                ret = -1;
            }
            FILE_LOG(logSUCCESS) << "Core shared memory creation = OK [Command]";
        }
    }
    // =========================================================================
    return ret;
}

void CANconnect::WheelMovewithGoalPos(double _x, double _y, double _r)
{
    sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_MOVE_TO_GOAL;
    sharedCMD->omni_xm = _x;
    sharedCMD->omni_ym = _y;
    sharedCMD->omni_yawdeg = _r;
}

void CANconnect::SetMaxSpeed(double _spd)
{
    sharedCMD->max_ms = _spd;
}

void CANconnect::WheelMoveStart()
{
    sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_VEL_MODE_START;
    sharedCMD->COMMAND.USER_PARA_CHAR[0] = 1;
}

void CANconnect::VelModeInputPush(double _vx, double _vy, double _vr)
{
    sharedCMD->vel_x = _vx;
    sharedCMD->vel_y = _vy;
    sharedCMD->vel_r = _vr;
}

void CANconnect::WheelMoveStop()
{
    sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_VEL_MODE_START;
    sharedCMD->COMMAND.USER_PARA_CHAR[0] = 0;
}
