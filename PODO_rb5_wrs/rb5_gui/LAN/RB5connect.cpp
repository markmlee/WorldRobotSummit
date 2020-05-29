#include "RB5connect.h"

RB5connect::RB5connect()
{
    initFlag = false;
    cmdConfirmFlag = false;
    moveSendFlag = false;

    memset(&systemStat, 0, sizeof(systemSTAT));
    systemStat.sdata.program_mode = -1;
    systemStat.sdata.robot_state = -1;

    clientCMD = new CMDClient();
    clientDATA = new DATAClient();
    connect(clientCMD, SIGNAL(SIG_Connected()), this, SLOT(CMDConnected()));
    connect(clientCMD, SIGNAL(SIG_Disconnected()), this, SLOT(CMDDisconnected()));
    connect(clientDATA, SIGNAL(SIG_Connected()), this, SLOT(DATAConnected()));
    connect(clientDATA, SIGNAL(SIG_Disconnected()), this, SLOT(DATADisconnected()));

    requestTimer = new QTimer(this);
}

RB5connect::~RB5connect()
{
    printf("delete RB5connect\n");
    requestTimer->stop();
    delete clientCMD;
    delete clientDATA;
}

void RB5connect::CMDConnected()
{
    emit RB5_CMD_Connected();
}

void RB5connect::CMDDisconnected()
{
    emit RB5_CMD_Disconnected();
}

void RB5connect::DATAConnected()
{
    connect(requestTimer, SIGNAL(timeout()), this, SLOT(DATArequest()));
    emit RB5_DATA_Connected();
}

void RB5connect::DATADisconnected()
{
    emit RB5_DATA_Disconnected();
}

static int ignore_data_cnt = 0;

void RB5connect::onReadyCmdRead()
{
    QByteArray datas = clientCMD->readAll();
    if(QString(datas.data()).compare("The command was executed\n") == 0){
        if(moveSendFlag == true){
            moveSendFlag = false;
            ignore_data_cnt = 5;
            systemStat.sdata.robot_state = MOVING; //run
        }
        cmdConfirmFlag = true;
    }
}

#include <iostream>
void RB5connect::CMDconnect(QString _addr, int _port)
{
    QString temp = "hi!";
    std::cout << temp.toStdString() << std::endl;
    printf("Try connect to ");
    std::cout << _addr.toStdString();
    printf("(%d)\n",_port);

    clientCMD->RBConnect(_addr,_port);
    connect(clientCMD, SIGNAL(CMD_READED()), this, SLOT(CMDread()));
}

void RB5connect::DATAconnect(QString _addr, int _port)
{
    clientDATA->RBConnect(_addr, _port);
    connect(clientDATA, SIGNAL(DATA_READED()), this, SLOT(DATAread()));
    requestTimer->start(20);
}

void RB5connect::CMDdisconnect()
{
    clientCMD->RBDisconnect();
}

void RB5connect::DATAdisconnect()
{
    clientDATA->RBDisconnect();
    requestTimer->stop();
}

void RB5connect::CMDread()
{
    if(QString(clientCMD->dataReceived.data()).compare("The command was executed\n") == 0){
        if(moveSendFlag == true){
            moveSendFlag = false;
            ignore_data_cnt = 5;
            systemStat.sdata.robot_state = MOVING; //run
        }
        cmdConfirmFlag = true;
    }
}

void RB5connect::DATAread()
{
    while(clientDATA->dataReceived.length() > 4 )
    {
        if( clientDATA->dataReceived[0] == '$')
        {
            int size = ((int)((unsigned char)clientDATA->dataReceived[2]<<8)|((unsigned char)clientDATA->dataReceived[1]));
            if(size <= clientDATA->dataReceived.length() )
            {
                if(3 == clientDATA->dataReceived[3])
                {
                    if(ignore_data_cnt > 0){
                        ignore_data_cnt--;
                    }else{
                        memcpy(&systemStat,clientDATA->dataReceived.data(),sizeof(systemSTAT));
                    }
                    clientDATA->dataReceived.remove(0,sizeof(systemSTAT));
                }
                else if(4 == clientDATA->dataReceived[3])
                {
                    memcpy(&systemConfig,clientDATA->dataReceived.data(),sizeof(systemCONFIG));
                    clientDATA->dataReceived.remove(0,sizeof(systemCONFIG));
                }
                else if(10 == clientDATA->dataReceived[3])
                {
                    memcpy(&systemPopup,clientDATA->dataReceived.data(),sizeof(systemPOPUP));
                    clientDATA->dataReceived.remove(0,sizeof(systemPOPUP));
                }
                else
                {
                    clientDATA->dataReceived.remove(0,1);
                }

            }

        }
        else
        {
            clientDATA->dataReceived.remove(0,1);
        }
    }

    pir_detected[0] = systemStat.sdata.digital_in[12];
    pir_detected[1] = systemStat.sdata.digital_in[13];
    pir_detected[2] = systemStat.sdata.digital_in[14];
    pir_detected[3] = systemStat.sdata.digital_in[15];
}


void RB5connect::DATArequest()
{
    QByteArray temp = "reqdata";
    clientDATA->RBSendData(temp);
}

int RB5connect::checkPir()
{
    for(int i=0;i<4;i++)
    {
        if(pir_detected[i] != 0)
            return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------//

void RB5connect::RB5_init()
{
    QByteArray msg = "mc jall init";
    clientCMD->RBSendData(msg);
    cmdConfirmFlag = false;
}

void RB5connect::Suction(int mode)
{
    if(mode == 0)
    {
        QByteArray msg = "gripper_rtq_epick_reset(1)";
        clientCMD->RBSendData(msg);
//        usleep(500*1000);
//        printf("suction reset\n");
    }else if(mode == 1)
    {
        QByteArray msg = "gripper_rtq_epick_suction(1)";
        clientCMD->RBSendData(msg);
//        usleep(500*1000);
//        printf("suction\n");

    }else
    {
        QByteArray msg = "gripper_rtq_epick_release(1)";
        clientCMD->RBSendData(msg);
//        usleep(500*1000);
//        printf("suction release\n");
    }
}


void RB5connect::ProgramMode_Real()
{
    printf("Real mode\n");
    QByteArray msg = "pgmode real";
    clientCMD->RBSendData(msg);
    cmdConfirmFlag = false;
}

void RB5connect::ProgramMode_Simulation()
{
    QByteArray msg = "pgmode simulation";
    clientCMD->RBSendData(msg);
    cmdConfirmFlag = false;
}

void RB5connect::MoveJoint(float joint1, float joint2, float joint3, float joint4, float joint5, float joint6, float spd, float acc)
{
    qDebug() << "jointall = " << joint1, joint2, joint3, joint4, joint5, joint6;
    QString text;
    text.sprintf("jointall %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, joint1, joint2, joint3, joint4, joint5, joint6);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    moveSendFlag = true;
    cmdConfirmFlag = false;
//    systemStat.sdata.robot_state = MOVING; //run
}

void RB5connect::MoveJoint(float coordinate[6],float spd, float acc)
{
    qDebug() << "jointall = " << coordinate[0] << " " << coordinate[1] << " " << coordinate[2] << " " << coordinate[3] << " " << coordinate[4] << " " << coordinate[5];
    QString text;
    text.sprintf("jointall %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, coordinate[0], coordinate[1], coordinate[2], coordinate[3], coordinate[4], coordinate[5]);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
    moveSendFlag = true;
    systemStat.sdata.robot_state = MOVING; //run
}

void RB5connect::MoveTCP(float coordinate[6], float spd, float acc)
{
    QString text;
    text.sprintf("movetcp %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, coordinate[0], coordinate[1], coordinate[2], coordinate[3], coordinate[4], coordinate[5]);
    printf("movetcp %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", spd, acc, coordinate[0], coordinate[1], coordinate[2], coordinate[3], coordinate[4], coordinate[5]);
    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    systemStat.sdata.robot_state = MOVING; //run
    cmdConfirmFlag = false;
    moveSendFlag = true;
}

void RB5connect::MoveCircle_ThreePoint(int type, float x1, float y1, float z1, float rx1, float ry1, float rz1, float x2, float y2, float z2, float rx2, float ry2, float rz2, float spd, float acc)
{
    QString text;
    char buf[15];
    if(type == 0){
        sprintf(buf, "intended");
    }else if(type == 1){
        sprintf(buf, "constant");
    }else if(type == 2){
        sprintf(buf, "radial");
    }
    text.sprintf("movecircle threepoints %s %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f",
                 buf, spd, acc, x1, y1, z1, rx1, ry1, rz1, x2, y2, z2, rx2, ry2, rz2);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag =false;
    moveSendFlag = true;
    systemStat.sdata.robot_state = MOVING;
}

void RB5connect::MoveCircle_Axis(int type, float cx, float cy, float cz, float ax, float ay, float az, float rot_angle, float spd, float acc)
{
    QString text;
    char buf[15];
    if(type == 0){
        sprintf(buf, "intended");
    }else if(type == 1){
        sprintf(buf, "constant");
    }else if(type == 2){
        sprintf(buf, "radial");
    }
    text.sprintf("movecircle axis %s %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f",
                 buf, spd, acc, rot_angle, cx, cy, cz, ax, ay, az);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag =false;
    moveSendFlag = true;
    systemStat.sdata.robot_state = MOVING;
}

void RB5connect::MoveJointBlend_Clear()
{
    QByteArray msg = "blend_jnt clear_pt";
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::MoveJointBlend_AddPoint(float joint1, float joint2, float joint3, float joint4, float joint5, float joint6, float spd, float acc)
{
    QString text;
    text.sprintf("blend_jnt add_pt %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, joint1, joint2, joint3, joint4, joint5, joint6);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
    systemStat.sdata.robot_state = MOVING; //run
}

void RB5connect::MoveJointBlend_AddPoint(float coordinate[6], float spd, float acc)
{
    QString text;
    text.sprintf("blend_jnt add_pt %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, coordinate[0], coordinate[1], coordinate[2], coordinate[3], coordinate[4], coordinate[5]);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
    systemStat.sdata.robot_state = MOVING; //run
}

void RB5connect::MoveJointBlend_MovePoint()
{
    QByteArray msg = "blend_jnt move_pt";
    clientCMD->RBSendData(msg);

    moveSendFlag = true;
    cmdConfirmFlag = false;
    systemStat.sdata.robot_state = MOVING;
}

void RB5connect::MoveTCPBlend_Clear()
{
    QByteArray msg = "blend_tcp clear_pt";
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::MoveTCPBlend_AddPoint(float radius, float x, float y, float z, float rx, float ry, float rz, float spd, float acc)
{
    QString text;
    text.sprintf("blend_tcp add_pt %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, radius, x, y, z, rx, ry, rz);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
    systemStat.sdata.robot_state = MOVING; //run
}

void RB5connect::MoveTCPBlend_AddPoint(float radius, float coordinate[6], float spd, float acc)
{
    QString text;
    text.sprintf("blend_tcp add_pt %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f", spd, acc, radius, coordinate[0], coordinate[1], coordinate[2], coordinate[3], coordinate[4], coordinate[5]);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
    systemStat.sdata.robot_state = MOVING; //run
}

void RB5connect::MoveTCPBlend_MovePoint(int mode)
{
    QString text;
    char buf[15];
    if(mode == 0){
        sprintf(buf, "intended");
    }else if(mode == 1){
        sprintf(buf, "constant");
    }else if(mode == 2){
        sprintf(buf, "radial");
    }
    text.sprintf("blend_tcp move_pt %s", buf);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    moveSendFlag = true;
    cmdConfirmFlag = false;
    systemStat.sdata.robot_state = MOVING;
}

void RB5connect::ControlBoxDigitalOut(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, int d9, int d10, int d11, int d12, int d13, int d14, int d15)
{
    QString text;
    text.sprintf("digital_out %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::ControlBoxAnalogOut(float a0, float a1, float a2, float a3)
{
    QString text;
    text.sprintf("analog_out %.3f, %.3f, %.3f, %.3f", a0, a1, a2, a3);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::ToolOut(int volt, int d0, int d1)
{
    int temp_volt = volt;
    if((temp_volt != 12) && (temp_volt != 24))
        temp_volt = 0;

    QString text;
    text.sprintf("tool_out %d, %d, %d", temp_volt, d0, d1);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::BaseSpeedChange(float spd)
{
    printf("kk\n");
    QString text;
    if(spd > 1.0)
        spd = 1.0;
    if(spd < 0.0)
        spd = 0.0;
    text.sprintf("sdw default_speed %.3f", spd);

    QByteArray msg = text.toStdString().c_str();
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::MotionPause()
{
    QByteArray msg = "task pause";
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::MotionHalt()
{
    printf("Halt\n");
    QByteArray msg = "task stop";
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::MotionResume()
{
    QByteArray msg = "task resume_a";
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

void RB5connect::CollisionResume()
{
    QByteArray msg = "task resume_b";
    clientCMD->RBSendData(msg);

    cmdConfirmFlag = false;
}

int RB5connect::IsMotionIdle()
{
   return ((cmdConfirmFlag == true) && (systemStat.sdata.robot_state == IDLE));
}

//---------------------------------------------------------------------------------//
CMDClient::CMDClient()
{
    dataSize = sizeof(int);
    dataReceived.clear();
}

CMDClient::~CMDClient()
{
    printf("delete CMD\n");
    RBClientSocket->close();
}

void CMDClient::ReadData()
{
    dataReceived.clear();
    dataReceived = RBClientSocket->readAll();

    emit CMD_READED();
}

DATAClient::DATAClient()
{
    dataSize = sizeof(char);
    dataReceived.clear();
}

DATAClient::~DATAClient()
{
    printf("delete DATA\n");
    RBClientSocket->close();
}

void DATAClient::ReadData()
{
    QByteArray datas = RBClientSocket->readAll();
    dataReceived += datas;

    emit DATA_READED();
}
