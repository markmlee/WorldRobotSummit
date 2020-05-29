#ifndef ROSCONNECT_H
#define ROSCONNECT_H

#include <QByteArray>
#include <QTimer>
#include <QDataStream>
#include "LAN/BaseLAN/RBTCPServer.h"
#include "DataStructure/rosdata.h"

class ROSServer;
class RSTServer;

class ROSconnect : public QObject
{
    Q_OBJECT
public:
    ROSconnect();
    ~ROSconnect();

    void Connect(int _rosport, int _rstport);
    void Disconnect();
    ROSServer    *serverROS;
    RSTServer    *serverRST;
    Update message;
    command data;
    Result result;
    RB5toROS rb5toros;

    QTimer *sendTimer;
    int pir_detected;
    int pir_cnt = 0;
//    void setRB5RESULT(int rb5result)
//    {
//        rb5toros.result.rb5_result = rb5result;
//    }
//    void setWHEELRESULT(int wheelresult)
//    {
//        rb5toros.result.wheel_result = wheelresult;
//    }

    void sendRB5RESULT(int rst, int mode = false);
    void sendWHEELRESULT(int rst);
    void RESULTreset();

signals:
    void ROS_Connected();
    void ROS_Disconnected();
    void RST_Connected();
    void RST_Disconnected();
    void ROS_CMD_READ(command _cmd);

protected slots:
    void ROSConnected();
    void ROSDisconnected();
    void RSTConnected();
    void RSTDisconnected();
    void readCMD(char *_data);
    void sendSTATUS();

private:
    int ROSflag;
};

class ROSServer : public RBTCPServer
{
    Q_OBJECT
public:
    ROSServer();
    ~ROSServer();

    QByteArrays     dataReceived;
    char      *data;
    int     TXSize;
    int     RXSize;

signals:
    void ROS_UPDATE(char *_data);

protected slots:
    virtual void ReadData();
};

class RSTServer : public RBTCPServer
{
    Q_OBJECT
public:
    RSTServer();
    ~RSTServer();

    int  TXSize;
signals:
    void ROS_UPDATE(char *_data);

protected slots:
    virtual void ReadData();
};

#endif // ROSCONNECT_H
