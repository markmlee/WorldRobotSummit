#ifndef CANCONNECT_H
#define CANCONNECT_H

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "BaseLAN/RBLog.h"
#include "../../share/Headers/RBSharedMemory.h"
#include "../../share/Headers/JointInformation.h"

class CANconnect
{
public:
    CANconnect();
    ~CANconnect();
    pRBCORE_SHM_COMMAND     sharedCMD;
    pRBCORE_SHM_REFERENCE   sharedREF;
    pRBCORE_SHM_SENSOR      sharedSEN;

    int CreateSharedMemory();
    int isConnectCAN();
    void WheelMovewithGoalPos(double _x, double _y, double _r);
    void SetMaxSpeed(double _spd);
    void WheelMoveStart();
    void VelModeInputPush(double _vx, double _vy, double _vr);
    void WheelMoveStop();
private:
    int CANFlag = 0;
};

#endif // CANCONNECT_H
