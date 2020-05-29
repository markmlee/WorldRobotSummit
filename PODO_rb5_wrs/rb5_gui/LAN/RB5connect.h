#ifndef RB5CONNECT_H
#define RB5CONNECT_H

#include <sys/mman.h>
#include <fcntl.h>
#include <QSettings>
#include <QThread>
#include <QByteArray>
#include <QTimer>
#include <QDataStream>

#include "LAN/BaseLAN/RBTCPClient.h"
#include "DataStructure/rb5data.h"



extern int      _NO_OF_MC;
extern systemSTAT   *sys_status;
extern systemCONFIG *sys_config;
extern systemPOPUP  *sys_popup;

class CMDClient;
class DATAClient;

class RB5connect : public QObject
{
    Q_OBJECT
public:
    RB5connect();
    ~RB5connect();

    CMDClient   *clientCMD;
    DATAClient  *clientDATA;

    systemSTAT      systemStat;
    systemCONFIG    systemConfig;
    systemPOPUP     systemPopup;

    int cmdConfirmFlag;
    int moveSendFlag;
    int initFlag;

    int pir_detected[4];

    void CMDconnect(QString _addr, int _port);
    void DATAconnect(QString _addr, int _port);
    void CMDdisconnect();
    void DATAdisconnect();

    void RB5_init();
    void MoveJoint(float joint1, float joint2, float joint3, float joint4, float joint5, float joint6, float spd = -1, float acc = -1);
    void MoveJoint(float coordinate[6], float spd = -1, float acc = -1);
    void MoveTCP(float x, float y, float z, float rx, float ry, float rz, float spd = -1, float acc = -1);
    void MoveTCP(float coordinate[6], float spd = -1, float acc = -1);
    void MoveCircle_ThreePoint(int type, float x1, float y1, float z1, float rx1, float ry1, float rz1, float x2, float y2, float z2, float rx2, float ry2, float rz2, float spd = -1, float acc = -1);
    void MoveCircle_Axis(int type, float cx, float cy, float cz, float ax, float ay, float az, float rot_angle, float spd, float acc);
    void MoveJointBlend_Clear();
    void MoveJointBlend_AddPoint(float joint1, float joint2, float joint3, float joint4, float joint5, float joint6, float spd = -1, float acc = -1);
    void MoveJointBlend_AddPoint(float coordinate[6], float spd, float acc);
    void MoveJointBlend_MovePoint();
    void MoveTCPBlend_Clear();
    void MoveTCPBlend_AddPoint(float radius, float x, float y, float z, float rx, float ry, float rz, float spd = -1, float acc = -1);
    void MoveTCPBlend_AddPoint(float radius, float coordinate[6], float spd, float acc);
    void MoveTCPBlend_MovePoint(int mode);
    void ControlBoxDigitalOut(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, int d9, int d10, int d11, int d12, int d13, int d14, int d15);
    void ControlBoxAnalogOut(float a0, float a1, float a2, float a3);
    void ToolOut(int volt, int d0, int d1);
    void ProgramMode_Real();
    void ProgramMode_Simulation();
    void BaseSpeedChange(float spd);
    void MotionPause();
    void MotionHalt();
    void MotionResume();
    void CollisionResume();
    int IsMotionIdle();
    void Suction(int mode);
    int checkPir();


signals:
    void RB5_CMD_Connected();
    void RB5_CMD_Disconnected();
    void RB5_DATA_Connected();
    void RB5_DATA_Disconnected();

protected slots:
    void CMDConnected();
    void CMDDisconnected();
    void DATAConnected();
    void DATADisconnected();
    void CMDread();
    void DATAread();
    void DATArequest();
    void onReadyCmdRead();

private:
    QTimer  *requestTimer;
};

class CMDClient : public RBTCPClient
{
    Q_OBJECT
public:
    CMDClient();
    ~CMDClient();

    QByteArray     dataReceived;

signals:
    void CMD_READED();
protected slots:
    virtual void ReadData();

private:
    int     dataSize;
};

class DATAClient : public RBTCPClient
{
    Q_OBJECT
public:
    DATAClient();
    ~DATAClient();

    QByteArray     dataReceived;

signals:
    void DATA_READED();
protected slots:
    virtual void ReadData();

private:
    int     dataSize;
};

#endif // RB5CONNECT_H
