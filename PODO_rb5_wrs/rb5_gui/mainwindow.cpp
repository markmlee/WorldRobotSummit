#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QMessageBox>
#include <algorithm>
#include <signal.h>
#include <QThread>
#include <QByteArray>
#include <QToolBar>
#include <QAction>
//temporary
#include <QDebug>
#include <QHostInfo>

const int MAX_DISTANCE = 835;
const int MIN_DISTANCE = 200;

// THINGS I NEED ---------------------------------------
// joint data for ROS topics


/* Debug data */
FILE *fp;
#define ROW 50000
#define COL 4
int     Save_Index;
double  Save_Data[COL][ROW];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->LE_IP->setText("10.0.2.7");
    ui->LE_IP_2->setText("127.0.0.1");

    RB5 = new RB5connect();
    ROS = new ROSconnect();

    Daemon_processID = 0;
    resultFlag = false;
    ROScommand.type = '0';
    FLAG_pir = false;
    FLAG_pir_detected = false;
    settingFile = QApplication::applicationDirPath().left(1) + "/demosettings.ini";
    settingFile = "configs/LauncherConfig.ini";
    QSettings settings(settingFile, QSettings::NativeFormat);
    ui->LE_DAEMON_PATH->setText(settings.value("daemon", "").toString());
    createActions();
    detected_cnt = 0;

    expHandler = new ExpandDialogHandler(this);

    frameCAN = new QFrame(this);
    frameStatus = new QFrame(this);
    statusDialog = new StatusDialog(frameStatus);
    canDialog = new CANDialog(frameCAN);

    expHandler->registerDialog(statusDialog, frameStatus);
    expHandler->registerDialog(canDialog, frameCAN);

    statusDialog->SetStatus(&(RB5->systemStat));

    ui->textCmdPort->setStyleSheet("QLineEdit{background-color:red}");
    ui->textDataPort->setStyleSheet("QLineEdit{background-color:red}");
    ui->textROSPort->setStyleSheet("QLineEdit{background-color:red}");
    ui->textRSTPort->setStyleSheet("QLineEdit{background-color:red}");

    connect(&pirTimer, SIGNAL(timeout()),this,SLOT(pirCheck()));
    connect(&systemTimer, SIGNAL(timeout()), this, SLOT(onSystemCheck()));
    connect(&systemTimer, SIGNAL(timeout()), this, SLOT(RB5toROS()));

    connect(ui->actionCAN, SIGNAL(toggled(bool)), this, SLOT(ActionCAN_Toggled()));
    connect(ui->actionMANUAL, SIGNAL(toggled(bool)), this, SLOT(ActionMANUAL_Toggled()));
    connect(ui->actionSTATUS, SIGNAL(toggled(bool)), this, SLOT(ActionSTATUS_Toggled()));

    connect(RB5, SIGNAL(RB5_CMD_Connected()), this, SLOT(onCmdConnected()));
    connect(RB5, SIGNAL(RB5_CMD_Disconnected()), this, SLOT(onCmdDisconnected()));
    connect(RB5, SIGNAL(RB5_DATA_Connected()), this, SLOT(onDataConnected()));
    connect(RB5, SIGNAL(RB5_DATA_Disconnected()), this, SLOT(onDataDisconnected()));
    connect(ROS, SIGNAL(ROS_Connected()), this, SLOT(onROSConnected()));
    connect(ROS, SIGNAL(ROS_Disconnected()), this, SLOT(onROSDisconnected()));
    connect(ROS, SIGNAL(RST_Connected()), this, SLOT(onRSTConnected()));
    connect(ROS, SIGNAL(RST_Disconnected()), this, SLOT(onRSTDisconnected()));

    systemTimer.start(10);
    pirTimer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(CANenable)
    {
        delete CAN;
        kill(Daemon_processID, SIGKILL);
    }
    delete RB5;
    delete ROS;
    systemTimer.stop();
}

void MainWindow::checkPir(int rst)
{
    ROS->result.rb5_result = rst;
    ROS->result.wheel_result = BREAK;
    detected_cnt = 0;

    FILE_LOG(logSUCCESS) << "Check Pir sensor";
    FLAG_pir = true;
}

void MainWindow::RB5toROS()
{
    ROS->message.robot_state =  RB5->systemStat.sdata.robot_state;
    ROS->message.power_state =  RB5->systemStat.sdata.power_state;
    ROS->message.program_mode = RB5->systemStat.sdata.program_mode;

    ROS->message.collision_detect = RB5->systemStat.sdata.op_stat_collision_occur;
    ROS->message.freedrive_mode = RB5->systemStat.sdata.is_freedrive_mode;

    ROS->message.speed = RB5->systemStat.sdata.default_speed;

    for(int i = 0; i < 6; i++)
    {
        ROS->message.joint_angles[i] = RB5->systemStat.sdata.jnt_ang[i];
        ROS->message.joint_references[i] = RB5->systemStat.sdata.jnt_ref[i];
        ROS->message.joint_current[i] = RB5->systemStat.sdata.cur[i];
        ROS->message.joint_temperature[i] = RB5->systemStat.sdata.temperature_mc[i];
        ROS->message.joint_information[i] = RB5->systemStat.sdata.jnt_info[i];

        ROS->message.tcp_reference[i] = RB5->systemStat.sdata.tcp_ref[i];
        ROS->message.tcp_position[i] = RB5->systemStat.sdata.tcp_pos[i];
    }

    ROS->message.tool_reference = RB5->systemStat.sdata.tfb_voltage_out;

    if(CANenable == true)
    {
        ROS->message.ft_sensor[0] = CAN->sharedSEN->FT[0].Mx;
        ROS->message.ft_sensor[1] = CAN->sharedSEN->FT[0].My;
        ROS->message.ft_sensor[2] = CAN->sharedSEN->FT[0].Fz;
        ROS->message.ft_sensor[3] = CAN->sharedSEN->FT[0].Fx;
        ROS->message.ft_sensor[4] = CAN->sharedSEN->FT[0].Fy;
        ROS->message.ft_sensor[5] = CAN->sharedSEN->FT[0].Mz;
    }else
    {
        ROS->message.ft_sensor[0] = 0.;
        ROS->message.ft_sensor[1] = 0.;
        ROS->message.ft_sensor[2] = 0.;
        ROS->message.ft_sensor[3] = 0.;
        ROS->message.ft_sensor[4] = 0.;
        ROS->message.ft_sensor[5] = 0.;
    }

    for(int i=0;i<4;i++)
    {
        ROS->message.pir_detected[i] = RB5->pir_detected[i];
    }
}

void MainWindow::onSystemCheck()
{
    // check initialize status
    if(initFlag == true)
    {
        int init_info = RB5->systemStat.sdata.init_state_info;
        switch(init_info)
        {
        case INIT_STAT_INFO_VOLTAGE_CHECK:
            ui->LE_INIT_POWER->setStyleSheet("QLineEdit{background-color:yellow}");
            ui->LE_INIT_DEVICE->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_SYSTEM->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_ROBOT->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_STATUS->setText(QString().sprintf("1/5 Voltage check..."));
            break;
        case INIT_STAT_INFO_DEVICE_CHECK:
            ui->LE_INIT_POWER->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_DEVICE->setStyleSheet("QLineEdit{background-color:yellow}");
            ui->LE_INIT_SYSTEM->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_ROBOT->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_STATUS->setText(QString().sprintf("2/5 Device check..."));
            break;
        case INIT_STAT_INFO_FIND_HOME:
            ui->LE_INIT_POWER->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_DEVICE->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_SYSTEM->setStyleSheet("QLineEdit{background-color:yellow}");
            ui->LE_INIT_ROBOT->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_STATUS->setText(QString().sprintf("3/5 Find home..."));
            break;
        case INIT_STAT_INFO_VARIABLE_CHECK:
            ui->LE_INIT_POWER->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_DEVICE->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_SYSTEM->setStyleSheet("QLineEdit{background-color:yellow}");
            ui->LE_INIT_ROBOT->setStyleSheet("QLineEdit{background-color:red}");
            ui->LE_INIT_STATUS->setText(QString().sprintf("4/5 Variable check..."));
            break;
        case INIT_STAT_INFO_COLLISION_ON:
            ui->LE_INIT_POWER->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_DEVICE->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_SYSTEM->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_ROBOT->setStyleSheet("QLineEdit{background-color:yellow}");
            ui->LE_INIT_STATUS->setText(QString().sprintf("5/5 Collision on..."));
            break;
        case INIT_STAT_INFO_INIT_DONE:
            ui->LE_INIT_POWER->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_DEVICE->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_SYSTEM->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_ROBOT->setStyleSheet("QLineEdit{background-color:green}");
            ui->LE_INIT_STATUS->setText(QString().sprintf("Initialize done."));
            initFlag = false;
            break;
        case INIT_STAT_INFO_NOACT:
        default:
            break;
        }
    }

    // sendResult to ROS
    if(resultFlag == true && (ROS->result.rb5_result == ACCEPT||ROS->result.wheel_result == ACCEPT))
    {
        switch(ROScommand.type)
        {
        case 'I':
        {
            if(RB5->systemStat.sdata.init_state_info == INIT_STAT_INFO_INIT_DONE)
            {
                FILE_LOG(logSUCCESS) << "Initialize done";
                ROS->sendRB5RESULT(DONE);//done
            }else if(RB5->systemStat.sdata.init_error == true)//191022TempDebugneed
            {//init error
                FILE_LOG(logSUCCESS) << "Initialize error";
                ROS->sendRB5RESULT(ERROR_STOP);
            }
            break;
        }
        case 'M':
        {
            if(ROScommand.data == 0)//Simulation
            {
                if(RB5->systemStat.sdata.program_mode == 1)
                {
                    FILE_LOG(logSUCCESS) << "Program mode : Simulation mode";
                    ROS->sendRB5RESULT(DONE);//done
                }else if(Count > 300)
                {
                    FILE_LOG(logERROR) << "Time Out : Simulation mode";
                    ROS->sendRB5RESULT(ERROR_STOP);
                    Count = 0;
                }
            }else if(ROScommand.data == 1)//Real
            {
                if(RB5->systemStat.sdata.program_mode == 0)
                {
                    FILE_LOG(logSUCCESS) << "Program mode : Real mode";
                    ROS->sendRB5RESULT(DONE);//done
                }else if(Count > 300)
                {
                    FILE_LOG(logERROR) << "Time Out : Real mode";
                    ROS->sendRB5RESULT(ERROR_STOP);
                }
            }
            break;
        }
        case 'J':
        case 'T':
        case 'B':
        {
            if(RB5->systemStat.sdata.op_stat_collision_occur == 1)
            {
                FILE_LOG(logERROR) << "Collision!";
                checkPir(EXT_COLLISION);
            }else if(RB5->systemStat.sdata.op_stat_ems_flag == 1)
            {
                FILE_LOG(logERROR) << "EMS!";
                checkPir(ERROR_STOP);
            }else if(RB5->systemStat.sdata.robot_state == PAUSED)
            {
                FILE_LOG(logERROR) << "Something wrong, Motion paused!";
                checkPir(ERROR_STOP);
            }

            if(RB5->systemStat.sdata.robot_state == MOVING)
                break;
            else if(RB5->systemStat.sdata.robot_state == IDLE)
            {
                if(Count > 3)
                {
                    Count = 0;
                    FILE_LOG(logSUCCESS) << "Move Done";
                    printf("RB5 = %d, WHEEL = %d\n",ROS->result.rb5_result,ROS->result.wheel_result);
                    checkPir(DONE);
                }
            }
            break;
        }
        case 'H':
        {
            if(RB5->systemStat.sdata.op_stat_collision_occur == 0 && RB5->systemStat.sdata.op_stat_ems_flag == 0)
            {
                FILE_LOG(logSUCCESS) << "Halt done";
                ROS->sendRB5RESULT(DONE);
            }else
            {
//                RB5->MotionHalt();
            }
        }
        case 'V':
        {
            if(RB5->systemStat.sdata.default_speed == ROScommand.data)
            {
                FILE_LOG(logSUCCESS) << "Speed changed";
                ROS->sendRB5RESULT(DONE);
            }else if(Count > 300)
            {
                FILE_LOG(logERROR) << "Time Out : Speed changed";
                ROS->sendRB5RESULT(ERROR_STOP);
                Count = 0;
            }
            break;
        }
        case 'E':
        {
            if(Count > ROScommand.data/10)
            {
                Count = 0;
                RB5->ToolOut(0,LOW,LOW);
                FILE_LOG(logSUCCESS) << "Gripper move done";
                ROS->sendRB5RESULT(DONE);
            }
            break;
        }
        case 'W':
        {
            if(ROScommand.d0 == 0 && ROScommand.d1 == 1)//goalmode
            {
                if(CAN->sharedSEN->WHEEL_STATE == OMNI_MOVE_DONE)
                {
                    printf("state = %d\n",CAN->sharedSEN->WHEEL_STATE);
                    FILE_LOG(logSUCCESS) << "Goal mode move done";
                    ROS->sendWHEELRESULT(DONE);
                    CAN->sharedSEN->WHEEL_STATE = OMNI_BREAK;
                }else if(CAN->sharedSEN->WHEEL_STATE == OMNI_BREAK && Count > 1000)
                {
                    Count = 0;
                    printf("RB5 = %d, WHEEL = %d\n",ROS->result.rb5_result,ROS->result.wheel_result);
                    FILE_LOG(logERROR) << "Time out : Wheel move";
                    ROS->sendWHEELRESULT(ERROR_STOP);
                }
            }

            if(CAN->sharedSEN->WHEEL_STATE != OMNI_BREAK)
            {
                if(CAN->sharedSEN->CAN_Enabled == false || CAN->sharedSEN->REF_Enabled == false)
                {
                    FILE_LOG(logERROR) << "CAN disconnected";
                    ROS->sendWHEELRESULT(ERROR_STOP);
                }
            }
            break;
        }
        }
        Count++;
    }


    //check robot state for pir sensor
    if(FLAG_pir == true)
    {
        if(detected_cnt > 3)
        {
            FILE_LOG(logERROR) << "Human detected!!";
            usleep(7000*1000);

            FLAG_pir = false;
            ROS->sendRB5RESULT(ROS->result.rb5_result);
        }

        static int cnt = 101;
        if(FLAG_pir_detected == 1)//detected
        {
            if(cnt > 100)
            {
                FILE_LOG(logERROR) << "Human detect?";
                cnt = 1;
            }
            cnt++;
        }else
        {
            FLAG_pir = false;
            ROS->sendRB5RESULT(ROS->result.rb5_result);
        }
    }


    if(canDialog->FLAG_Wheelmove == true)
    {
        if(CAN->sharedSEN->WHEEL_STATE == OMNI_MOVE_DONE)
        {
            printf("state = %d\n",CAN->sharedSEN->WHEEL_STATE);
            FILE_LOG(logSUCCESS) << "Goal mode move done";
            CAN->sharedSEN->WHEEL_STATE = OMNI_BREAK;
            canDialog->FLAG_Wheelmove = false;
        }
    }

    //save data
    save_rb5();
}

void MainWindow::pirCheck()
{//per 1 sec
    //check pir sensor value and turn on detected flag
    detected_cnt++;
    FLAG_pir_detected = RB5->pir_detected[0];
}

// connect & disconnect ---------------------------

void MainWindow::onCmdConnected()
{
    ui->BTN_CONNECT_COM->setText("Disconnect");
    ui->textCmdPort->setStyleSheet("QLineEdit{background-color:green}");
}

void MainWindow::onCmdDisconnected()
{
    ui->BTN_CONNECT_COM->setText("Connect");
    ui->textCmdPort->setStyleSheet("QLineEdit{background-color:red}");
}

void MainWindow::onDataConnected()
{
    ui->BTN_CONNECT_DATA->setText("Disconnect");
    ui->textDataPort->setStyleSheet("QLineEdit{background-color:green}");
}

void MainWindow::onDataDisconnected()
{
    ui->BTN_CONNECT_DATA->setText("Connect");
    ui->textDataPort->setStyleSheet("QLineEdit{background-color:red}");
}

void MainWindow::onROSConnected()
{
    ui->BTN_CONNECT_ROS->setText("Connected");
    ui->textROSPort->setStyleSheet("QLineEdit{background-color:green}");
}

void MainWindow::onROSDisconnected()
{
    ui->BTN_CONNECT_ROS->setText("Close");
    ui->textROSPort->setStyleSheet("QLineEdit{background-color:yellow}");
}

void MainWindow::onRSTConnected()
{
    ui->textRSTPort->setStyleSheet("QLineEdit{background-color:green}");
    resultFlag = true;
}

void MainWindow::onRSTDisconnected()
{
    ui->textRSTPort->setStyleSheet("QLineEdit{background-color:yellow}");
    resultFlag = false;
}

void MainWindow::on_BTN_CONNECT_COM_clicked()
{
    if(ui->BTN_CONNECT_COM->text() == "Connect")
    {
        QString addr = ui->LE_IP->text();
        RB5->CMDconnect(addr, ui->textCmdPort->text().toInt());
    }else{
        RB5->CMDdisconnect();
    }
}

void MainWindow::on_BTN_CONNECT_DATA_clicked()
{
    if(ui->BTN_CONNECT_DATA->text() == "Connect")
    {
        QString addr = ui->LE_IP->text();
        RB5->DATAconnect(addr, ui->textDataPort->text().toInt());
    }else
    {
        RB5->DATAdisconnect();
    }
}

void MainWindow::on_BTN_CONNECT_ROS_clicked()
{
    if(ui->BTN_CONNECT_ROS->text() == "Open")
    {
        ROS->Connect(ui->textROSPort->text().toInt(), ui->textRSTPort->text().toInt());
        connect(ROS, SIGNAL(ROS_CMD_READ(command)), this, SLOT(setCommand(command)));

        ui->BTN_CONNECT_ROS->setText("Close");
        ui->textROSPort->setStyleSheet("QLineEdit{background-color:yellow}");
        ui->textRSTPort->setStyleSheet("QLineEdit{background-color:yellow}");

        RB5->ProgramMode_Simulation();
        RB5->BaseSpeedChange(0);
    }else
    {
        ROS->Disconnect();
        ui->BTN_CONNECT_ROS->setText("Open");
        ui->textROSPort->setStyleSheet("QLineEdit{background-color:red}");
        ui->textRSTPort->setStyleSheet("QLineEdit{background-color:red}");
    }
}


// Commands ---------------------------------------
void MainWindow::setCommand(command _cmd)//fromROS
{
    ROScommand = _cmd;
    Count = 0;

    FILE_LOG(logSUCCESS) << "SetCommand";
    ROS->RESULTreset();
    switch(ROScommand.type)
    {
    case 'I':
    {
        if(RB5->systemStat.sdata.init_state_info == INIT_STAT_INFO_INIT_DONE)
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Initialize already done";
           ROS->sendRB5RESULT(DONE);
        }else
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Initialize";
            ROS->sendRB5RESULT(ACCEPT);
            RB5->RB5_init();
            initFlag = true;
        }
        break;
    }
    case 'M':
    {
        if(ROScommand.data == 0)//Simulation
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Change to Simulation mode";
            ROS->sendRB5RESULT(ACCEPT);
            RB5->ProgramMode_Simulation();
        }else if(ROScommand.data == 1)//Real
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Change to Real mode";
            ROS->sendRB5RESULT(ACCEPT);
            RB5->ProgramMode_Real();
        }
        break;
    }
    case 'S':
    {//Suction
        ROS->sendRB5RESULT(ACCEPT);
        if(ROScommand.data == 0)        //reset
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Suction reset";
            RB5->Suction(0);
            FLAG_suction = false;
        }else if(ROScommand.data == 1)  //suction
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Suction";
            FLAG_suction = true;
            RB5->Suction(1);

        }else if(ROScommand.data == 2)  //release
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Suction release";
            RB5->Suction(2);
            FLAG_suction = false;
        }
        checkPir(DONE);

        break;
    }
    case 'J':
    {
        if(RB5->systemStat.sdata.init_state_info == INIT_STAT_INFO_INIT_DONE)
        {
            if(checkJointInput() == 1)
            {
                if(RB5->systemStat.sdata.robot_state == IDLE)
                {
                    FILE_LOG(logSUCCESS) << "NEW CMD::Move Joint";
                    ROS->sendRB5RESULT(ACCEPT);
                    RB5->MoveJoint(_cmd.coordinate, _cmd.spd, _cmd.acc);
                }else
                {
                    printf("robot_state = %d\n",RB5->systemStat.sdata.robot_state);
                    FILE_LOG(logERROR) << "NEW CMD::Move Joint -> RB5 is moving";
                    checkPir(STATE_ERROR);
                }
            }else
            {
                FILE_LOG(logERROR) << "NEW CMD::Move Joint -> Input error";
                checkPir(INPUT_ERROR);
            }
        }else
        {
            FILE_LOG(logERROR) << "NEW CMD::Move Joint -> RB5 is not initialized";
            ROS->sendWHEELRESULT(STATE_ERROR);
        }
        break;
    }
    case 'T':
    {
        if(RB5->systemStat.sdata.init_state_info == INIT_STAT_INFO_INIT_DONE)
        {
            if(checkTCPInput() == 1)
            {
                if(RB5->systemStat.sdata.robot_state == IDLE)
                {
                    FILE_LOG(logSUCCESS) << "NEW CMD::Move TCP";
                    ROS->sendRB5RESULT(ACCEPT);
                    RB5->MoveTCP(_cmd.coordinate, _cmd.spd, _cmd.acc);
                }else
                {
                    FILE_LOG(logERROR) << "NEW CMD::Move TCP -> RB5 is moving";
                    checkPir(STATE_ERROR);
                }
            }else
            {
                FILE_LOG(logERROR) << "NEW CMD::Move TCP -> Input error";
                checkPir(INPUT_ERROR);
            }
        }else
        {
            FILE_LOG(logERROR) << "NEW CMD::Move TCP -> RB5 is not initialized";
            ROS->sendWHEELRESULT(STATE_ERROR);
        }
        break;
    }
    case 'B':
    {//it is needed debug!
        if(RB5->systemStat.sdata.init_state_info == INIT_STAT_INFO_INIT_DONE)
        {
            if(_cmd.data == 0)
            {
                //Blend list clear
                FILE_LOG(logSUCCESS) << "NEW CMD::Blend TCP Clear";
                RB5->MoveTCPBlend_Clear();
                checkPir(DONE);
            }
            else if(_cmd.data == 1)
            {
                FILE_LOG(logSUCCESS) << "NEW CMD::Blend TCP AddPoint";
                //Blend Add point
                int radius = _cmd.d0;
                if(_cmd.d0 == 0)    radius = 100;
                RB5->MoveTCPBlend_AddPoint(radius, _cmd.coordinate, _cmd.spd, _cmd.acc);
                checkPir(DONE);
            }
            else if(_cmd.data == 2)
            {
                FILE_LOG(logSUCCESS) << "NEW CMD::Blend TCP Move!!";
                //Blend Move
                //0 = intended, 2 = radial
                int mode = _cmd.d0;
                if(_cmd.d0 == 0)    mode = 1;
                ROS->sendRB5RESULT(ACCEPT);
                RB5->MoveTCPBlend_MovePoint(mode);
            }else if(_cmd.data == 3)
            {
                FILE_LOG(logSUCCESS) << "NEW CMD::Blend Joint Clear";
                //Blend list clear
                RB5->MoveJointBlend_Clear();
                checkPir(DONE);
            }
            else if(_cmd.data == 4)
            {
                FILE_LOG(logSUCCESS) << "NEW CMD::Blend Joint AddPoint";
                //Blend Add point
                RB5->MoveJointBlend_AddPoint(_cmd.coordinate, _cmd.spd, _cmd.acc);
                checkPir(DONE);
            }
            else if(_cmd.data == 5)
            {
                FILE_LOG(logSUCCESS) << "NEW CMD::Blend Joint Move!!";
                //Blend Move
                //0 = intended, 2 = radial
                ROS->sendRB5RESULT(ACCEPT);
                RB5->MoveJointBlend_MovePoint();
            }
        }else
        {
            FILE_LOG(logERROR) << "NEW CMD::Blend -> RB5 is not initialized";
            ROS->sendWHEELRESULT(STATE_ERROR);
        }
        break;
    }
    case 'V':
    {
        FILE_LOG(logSUCCESS) << "NEW CMD::Base Speed change";
        ROS->sendRB5RESULT(ACCEPT);
        RB5->BaseSpeedChange(_cmd.data);
        break;
    }
    case 'E':
    {
        if(_cmd.d0 == 0 && _cmd.d1 == 0)
        {
            FILE_LOG(logSUCCESS) << "NEW CMD::Gripper Stop";
            ROS->sendRB5RESULT(ACCEPT);
            RB5->ToolOut(0, LOW, LOW);
            FILE_LOG(logSUCCESS) << "Gripper stop done";
            ROS->sendRB5RESULT(DONE);
            break;
        }
        FILE_LOG(logSUCCESS) << "NEW CMD::Gripper Move";
        ROS->sendRB5RESULT(ACCEPT);
        RB5->ToolOut(24, _cmd.d0, _cmd.d1);
        qDebug() << "_cmd.d0 = " << _cmd.d0;
        qDebug() << "_cmd.d1 = " << _cmd.d1;
        break;
    }
    case 'F':
    {
        if(_cmd.data == 0)//null
        {
            FILE_LOG(logSUCCESS) << "NEW CMD : FT sensor Null";
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_SENSOR_FT_NULL;
            usleep(1000*1000);
            ROS->sendRB5RESULT(DONE);
            break;
        }
    }
    case 'P':
    {
        FILE_LOG(logSUCCESS) << "NEW CMD : Motion Pause";
        ROS->sendRB5RESULT(ACCEPT);
        RB5->MotionPause();
        checkPir(DONE);
        break;
    }
    case 'H':
    {
        FILE_LOG(logSUCCESS) << "NEW CMD : Motion Halt";
        ROS->sendRB5RESULT(ACCEPT);
        RB5->MotionHalt();
//        checkPir(DONE);
        break;
    }
    case 'Q':
    {
        FILE_LOG(logSUCCESS) << "NEW CMD : Motion Resume";
        ROS->sendRB5RESULT(ACCEPT);
        RB5->MotionResume();
        checkPir(DONE);
        break;
    }
    case 'C':
    {
        FILE_LOG(logSUCCESS) << "NEW CMD : Collision Resume";
        ROS->sendRB5RESULT(ACCEPT);
        RB5->CollisionResume();
        checkPir(DONE);
        break;
    }
    case 'W':
    {//wheel
        if(CANenable)
        {
            if(CAN->sharedSEN->CAN_Enabled && CAN->sharedSEN->REF_Enabled)
            {
                if(ROScommand.d0 == 0 && ROScommand.d1 == 0)
                {
                    FILE_LOG(logSUCCESS) << "NEW CMD : ROS Wheel Initialize";
                    CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_JOY_OFF;
                }else if(ROScommand.d0 == 0 && ROScommand.d1 == 1)
                {
                    if(CAN->sharedSEN->WHEEL_STATE == OMNI_BREAK)
                    {
                        FILE_LOG(logSUCCESS) << "NEW CMD : Wheel Goal mode";
                        ROS->sendWHEELRESULT(ACCEPT);
                        CAN->WheelMovewithGoalPos(ROScommand.wheel[0],ROScommand.wheel[1],ROScommand.wheel[2]);

                    }else
                    {
                        FILE_LOG(logERROR) << "NEW CMD : Wheel Goal mode -> wheel is already moving";
                        ROS->sendWHEELRESULT(STATE_ERROR);
                    }
                }else if(ROScommand.d0 == 1 && ROScommand.d1 == 0)
                {
                    if(CAN->sharedSEN->WHEEL_STATE == OMNI_BREAK)
                    {
                        FILE_LOG(logSUCCESS) << "NEW CMD : Wheel Velocity mode Start";
                        ROS->sendWHEELRESULT(ACCEPT);
                        CAN->WheelMoveStart();
                        ROS->sendWHEELRESULT(DONE);
                    }else
                    {
                        FILE_LOG(logERROR) << "NEW CMD : Wheel Velocity mode -> wheel is already moving";
                        ROS->sendWHEELRESULT(STATE_ERROR);
                    }
                }else if(ROScommand.d0 == 1 && ROScommand.d1 == 1)
                {
                    FILE_LOG(logSUCCESS) << "NEW CMD : Wheel Velocity mode Stop";
                    ROS->sendWHEELRESULT(ACCEPT);
                    CAN->WheelMoveStop();
                    ROS->sendWHEELRESULT(DONE);
                }else
                {
                    FILE_LOG(logERROR) << "NEW CMD : Wheel Velocity mode -> Input error";
                    ROS->sendWHEELRESULT(INPUT_ERROR);
                }
            }else
            {
                FILE_LOG(logERROR) << "NEW CMD : Wheel Velocity mode -> CAN device not set";
                ROS->sendWHEELRESULT(STATE_ERROR);
            }
            break;
        }
    }
    case 'N':
    {//navigation
        if(CANenable)
        {
            if(CAN->sharedSEN->WHEEL_STATE == OMNI_VEL_ON)
            {
                CAN->VelModeInputPush(ROScommand.wheel[0],ROScommand.wheel[1],ROScommand.wheel[2]);
                ROS->sendWHEELRESULT(DONE);
            }else
            {
                FILE_LOG(logERROR) << "NEW CMD : Wheel Velocity -> not velocity mode";
                ROS->sendWHEELRESULT(STATE_ERROR);
            }
            break;
        }
    }
    default:
        break;
    }
}


void MainWindow::ActionSTATUS_Toggled()
{
    if(expHandler->isVisible(statusDialog))
    {
        expHandler->hideDialog(statusDialog);
    }else
    {
        expHandler->showDialog(statusDialog);
    }
}

void MainWindow::ActionCAN_Toggled()
{
    if(expHandler->isVisible(canDialog))
    {
        expHandler->hideDialog(canDialog);
    }else
    {
        expHandler->showDialog(canDialog);
    }
}

void MainWindow::ActionMANUAL_Toggled()
{
    ManualDialog dialogMANUAL;
    dialogMANUAL.setModal(true);
    dialogMANUAL.SetStatus(RB5);
    dialogMANUAL.exec();
}


// ------------------------------------------------
void MainWindow::on_BTN_INITIALIZE_clicked()
{
    if(RB5->systemStat.sdata.init_state_info != INIT_STAT_INFO_INIT_DONE)
    {
        RB5->RB5_init();
        ui->LE_INIT_STATUS->setText(QString().sprintf("Initialize start..."));
    }
    initFlag = true;
}

void MainWindow::on_BTN_CHANGE_DAEMON_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Change Daemon"), "", tr("AllFiles (*)"));
    if(fileName.isEmpty())
        return;
    else{
        ui->LE_DAEMON_PATH->setText(fileName);
    }
}

void MainWindow::on_BTN_START_DAEMON_clicked()
{
    if(Daemon_processID != 0)
    {
        QMessageBox::warning(this, "double executing Daemon", "There is an another Daemon working now..");
        return;
    }
    QStringList arguments;
    QFileInfo fileInfo(ui->LE_DAEMON_PATH->text());

    arguments << "-geometry";
    arguments << "80x60";
    arguments << "-e";
    arguments << "./"+fileInfo.fileName();

    if(!QProcess::startDetached("xterm", arguments, fileInfo.path(), &Daemon_processID))
    {
        QMessageBox::warning(this, "Process Creation Error", "Process Creation Error");
        return;
    }

    ui->LE_DAEMON_PID->setText(QString().sprintf("%d", Daemon_processID));
    ui->LE_DAEMON_PID->setStyleSheet("QLineEdit{background-color:green}");
    ui->BTN_START_DAEMON->setEnabled(false);
    QSettings settings(settingFile, QSettings::NativeFormat);
    settings.setValue("daemon", ui->LE_DAEMON_PATH->text());

    ui->BTN_START_DAEMON->setEnabled(false);
    startDaemonAction->setEnabled(false);

    usleep(100000);
    CAN = new CANconnect();
    int SMFlag = CAN->isConnectCAN();
    if(SMFlag == false)
    {
        FILE_LOG(logERROR) << "SharedMemory Initialize Failed..";
    }else
    {
        FILE_LOG(logSUCCESS) << "SharedMemory Connected!";
        CANenable = true;
    }
    canDialog->CAN_connect(CAN);

    ui->BTN_STOP_DAEMON->setEnabled(true);
}

void MainWindow::on_BTN_STOP_DAEMON_clicked()
{
    if(Daemon_processID == 0)
        return;

    canDialog->CAN_disconnect();
    ui->BTN_STOP_DAEMON->setEnabled(false);
    kill(Daemon_processID, SIGKILL);
    Daemon_processID = 0;
    ui->LE_DAEMON_PID->setText(QString().sprintf("%d", Daemon_processID));
    ui->LE_DAEMON_PID->setStyleSheet("QLineEdit{background-color:red}");

    usleep(100000);
    ui->BTN_START_DAEMON->setEnabled(true);
    startDaemonAction->setEnabled(true);
    CANenable = false;
    delete CAN;
}

void MainWindow::on_showNormal(){
    this->showNormal();
    QSettings settings(settingFile, QSettings::NativeFormat);
    settings.setValue("show", true);
}

void MainWindow::createActions()
{
    restoreAction = new QAction(tr("&restore"), this);
    quitAction = new QAction(tr("&exit"), this);
    connect(restoreAction, SIGNAL(triggered(bool)), this, SLOT(on_showNormal()));
    connect(quitAction, SIGNAL(triggered(bool)), qApp,SLOT(quit()));

    startDaemonAction = new QAction(tr("&start Daemon"), this);
    stopDaemonAction = new QAction(tr("&stop Daemon"), this);
    connect(startDaemonAction, SIGNAL(triggered(bool)), this, SLOT(on_BTN_START_DAEMON_clicked()));
    connect(stopDaemonAction, SIGNAL(triggered(bool)), this, SLOT(on_BTN_STOP_DAEMON_clicked()));
}

int  MainWindow::checkJointInput()
{
    if(ROScommand.spd < 0 || ROScommand.spd > 1)
        return -1;

    if(ROScommand.acc < 0)
        return -1;

    for(int i=0;i<6;i++)
    {
        if(fabs(ROScommand.coordinate[i]) > 999.)
            return -1;
    }

    return 1;
}

int  MainWindow::checkTCPInput()
{
    printf("input : %f, %f, %f, %f, %f, %f, %f, %f\n",ROScommand.coordinate[0],ROScommand.coordinate[1],ROScommand.coordinate[2],ROScommand.coordinate[3],ROScommand.coordinate[4],ROScommand.coordinate[5], ROScommand.spd, ROScommand.acc);
    if(ROScommand.spd < 0 || ROScommand.spd > 1)
        return -1;

    if(ROScommand.acc < 0)
        return -1;

    for(int i=0;i<6;i++)
    {
        if(fabs(ROScommand.coordinate[i]) > 999.)
            return -1;
    }

    float distance, coordinate0, coordinate1, coordinate2;
    coordinate0 = ROScommand.coordinate[0];
    coordinate1 = ROScommand.coordinate[1];
    coordinate2 = ROScommand.coordinate[2];

    distance = qSqrt(coordinate0*coordinate0 + coordinate1*coordinate1 + coordinate2*coordinate2); //add up the distance of the tcp from the robot

    printf("distance : %f\n",distance);
//    if(distance > MAX_DISTANCE || distance < MIN_DISTANCE)
//        return -1;

    return 1;

}

void MainWindow::on_pushButton_3_clicked()
{
    //RB5->Suction(2);
    ManualDialog dialogMANUAL;
    dialogMANUAL.setModal(true);
    dialogMANUAL.SetStatus(RB5);
    dialogMANUAL.exec();

}

void MainWindow::on_BTN_SAVE_START_clicked()
{
    FILE_LOG(logSUCCESS) << "NEW COMMAND :: DATA RESET";
    Save_Index = 0;
    memset(&Save_Data,0,sizeof(double)*COL*ROW);
}

void MainWindow::on_BTN_SAVE_clicked()
{
    FILE_LOG(logSUCCESS) << "NEW COMMAND :: DATA SAVE";
    fp = fopen("/home/rainbow/Desktop/dataPODO.txt","w");
    for(int i=0;i<Save_Index;i++)
    {
        for(int j=0;j<COL;j++)fprintf(fp,"%g\t", Save_Data[j][i]);
        fprintf(fp,"\n");
    }
    fclose(fp);
    FILE_LOG(logSUCCESS) << "Data Save Completed -> /home/rainbow/Desktop/dataPODO.txt";
}


void MainWindow::save_rb5()
{
    if(Save_Index < ROW)
    {
        Save_Data[0][Save_Index] = RB5->pir_detected[0];
        Save_Data[1][Save_Index] = RB5->pir_detected[1];
        Save_Data[2][Save_Index] = RB5->pir_detected[2];
        Save_Data[3][Save_Index] = RB5->pir_detected[3];
        Save_Index++;

        if(Save_Index >= ROW) Save_Index = 0;
    }
}
