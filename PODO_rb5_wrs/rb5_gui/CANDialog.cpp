#include "CANDialog.h"
#include "ui_CANDialog.h"



enum _SET_JointSequentialNumber{
    S_FLW = 0, S_FRW, S_BRW, S_BLW, S_NUM
};
QString table_joint_name[S_NUM] = {
    "FLW", "FRW", "BRW", "BLW"
};

const struct {
    int tw;
    int row;
    int exist;
} TW_ROW_Pairs[NO_OF_JOINTS] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};

CANDialog::CANDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CANDialog)
{
    ui->setupUi(this);

    InitTable(ui->TW_0, table_joint_name, S_NUM);
    lastSelected = FLW;
    select_working = false;
    CANflag = false;
    FLAG_Firstinit = false;

    ui->LE_CAN_STATE->setStyleSheet("QLineEdit{background-color:red}");
    ui->BTN_REF_ON->setStyleSheet("background-color: green");
    ui->BTN_ENC_ON->setStyleSheet("background-color: green");
    ui->BTN_JOY_ON->setStyleSheet("background-color: red");
    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel not set"));
    ui->LE_WHEEL_STATE->setStyleSheet("background-color: red");

    ui->BTN_REF_ON->setEnabled(false);
    FLAG_Wheelmove = false;
    connect(&displayTimer, SIGNAL(timeout()), this, SLOT(UpdateSettings()));
    displayTimer.start(10);
}

CANDialog::~CANDialog()
{
    delete ui;
}

void CANDialog::CAN_connect(CANconnect *can)
{
    CAN = can;
    CANflag = true;
}

void CANDialog::CAN_disconnect()
{
    CANflag = false;
    FLAG_Firstinit = false;
}

void CANDialog::on_BTN_CAN_CHECK_clicked()
{
    if(CANconnected)
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_CHECK_DEVICE;
    }
}

void CANDialog::on_BTN_FIND_HOME_clicked()
{
    if(CANconnected)
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_FIND_HOME;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = -1;
        FLAG_Firstinit = true;
    }
}

void CANDialog::on_BTN_EXECUTE_COMMAND_clicked()
{
    if(lastSelected < 0)
        return;

    int id = MC_GetID(lastSelected);
    int ch = MC_GetCH(lastSelected);

    if(CANconnected)
    {
        if(ui->RB_INIT_POS->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_FIND_HOME;
        }else if(ui->RB_ENC_ZERO->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_SENSOR_ENCODER_RESET;
        }else if(ui->RB_FET_ON->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 1;     //on
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_FET_ONOFF;
        }else if(ui->RB_FET_OFF->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 0;     //off
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_FET_ONOFF;
        }else if(ui->RB_CTRL_ON->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 1;     //on
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_CONTROL_ONOFF;
        }else if(ui->RB_CTRL_OFF->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 0;     //off
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_INIT_CONTROL_ONOFF;
        }else if(ui->RB_ERROR_CLEAR->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 0;     // only error clear
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_MOTION_ERROR_CLEAR;
        }else if(ui->RB_JOINT_RECOVER->isChecked()){
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
            CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 1;     // error clear + joint recovery
            CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_MOTION_ERROR_CLEAR;
        }
    }
}

void CANDialog::InitTable(QTableWidget *table, QString j_names[], int num)
{
    QFont tableFont;
    tableFont.setPointSize(8);

    const int item_height = 30;
    const int item_width = 50;
    const int col_0_width = 60;
    const int col_1_width = 100;
    const int col_2_width = 30;


    // Horizontal - Column
    for(int i=0; i<3; i++){
        table->insertColumn(i);
        table->setHorizontalHeaderItem(i, new QTableWidgetItem());
        table->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        table->horizontalHeaderItem(i)->setFont(tableFont);
    }
    table->horizontalHeaderItem(0)->setSizeHint(QSize(col_0_width, item_height));
    table->horizontalHeaderItem(1)->setSizeHint(QSize(col_1_width, item_height));
    table->horizontalHeaderItem(2)->setSizeHint(QSize(col_2_width, item_height));
    table->setColumnWidth(0, col_0_width);
    table->setColumnWidth(1, col_1_width);
    table->setColumnWidth(2, col_2_width);
    table->horizontalHeaderItem(0)->setText("Status");
    table->horizontalHeaderItem(1)->setText("Error");
    table->horizontalHeaderItem(2)->setText("T");

    // Vertical - Row
    for(int i=0; i<num; i++){
        table->insertRow(i);
        table->setRowHeight(i,30);
        table->setVerticalHeaderItem(i, new QTableWidgetItem());
        table->verticalHeaderItem(i)->setText(j_names[i]);
        table->verticalHeaderItem(i)->setSizeHint(QSize(item_width, item_height));
        table->verticalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        table->verticalHeaderItem(i)->setFont(tableFont);
    }

    for(int i=0; i<num; i++){
        for(int j=0; j<3; j++){
            table->setItem(i, j, new QTableWidgetItem());
            table->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
            table->item(i,j)->setFlags(table->item(i,j)->flags() & ~Qt::ItemIsEditable);
            table->item(i,j)->setFont(tableFont);
        }
    }

    table->setMinimumWidth(item_width + col_0_width + col_1_width + col_2_width + 2);
    table->setMaximumWidth(item_width + col_0_width + col_1_width + col_2_width + 2);
    table->setMinimumHeight(30*(num+1) + 2);
    table->setMaximumHeight(30*(num+1) + 2);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void CANDialog::UpdateSettings()
{
    int mcId, mcCh, row;
    QTableWidget *tw;
    QString str;

    if(CANflag) CANconnected = CAN->isConnectCAN();
    else        CANconnected = false;


    if(CANconnected)
    {
        ui->LE_CAN_STATE->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_CAN_STATE->setText(QString().sprintf("CAN connected"));
        for(int i=0; i<NO_OF_JOINTS; i++)
        {
            mcId = MC_GetID(i);
            mcCh = MC_GetCH(i);
            row = TW_ROW_Pairs[i].row;
            tw = ui->TW_0;

            mSTAT_hubo stat = CAN->sharedSEN->MCStatus[mcId][mcCh];
            str = "";
            if(CAN->sharedSEN->ENCODER[mcId][mcCh].BoardConnection) str += "C  ";
            else                                                      str += "N  ";
            if(stat.b.HIP == 1) str += "H/";
            else                str += "-/";
            if(stat.b.RUN == 1) str += "R/";
            else                str += "-/";


            str += QString().sprintf("%d", stat.b.HME);
            tw->item(row, 0)->setText(str);


            if(stat.b.RUN == 1 && stat.b.HME == 6)
            {
                tw->item(row, 0)->setBackgroundColor(QColor(100, 255, 100));    // green    Home(6) Run(on)
            }else if(stat.b.HME != 0){
                tw->item(row, 0)->setBackgroundColor(QColor(255, 100, 100));    // red      Home(x) Run(x)
            }else{
                tw->item(row, 0)->setBackgroundColor(QColor(255, 255, 100));    // yellow   Home(0)
            }

            str = "";
            if(stat.b.JAM == 1) str += "JAM ";
            if(stat.b.PWM == 1) str += "PWM ";
            if(stat.b.BIG == 1) str += "BIG ";
            if(stat.b.INP == 1) str += "INP ";
            if(stat.b.FLT == 1) str += "FLT ";
            if(stat.b.ENC == 1) str += "ENC ";
            if(stat.b.CUR == 1) str += "CUR ";
            if(stat.b.TMP == 1) str += "TMP ";
            if(stat.b.PS1 == 1) str += "PS1 ";
            if(stat.b.PS2 == 1) str += "PS2 ";
            if(str == "")
            {
                str = "-";
                tw->item(row, 1)->setBackgroundColor(QColor(255, 255, 255));
            }else
            {
                tw->item(row, 1)->setBackgroundColor(QColor(255, 100, 100));
            }
            tw->item(row, 1)->setText(str);
            tw->item(row, 2)->setText(QString().sprintf("%2d", (int)CAN->sharedSEN->MotorTemperature[mcId][mcCh]));
            if(CAN->sharedSEN->MotorTemperature[mcId][mcCh] > 60)
                tw->item(row, 2)->setBackgroundColor(QColor(255, 100, 100));
            else
                tw->item(row, 2)->setBackgroundColor(QColor(255, 255, 255));

        }


        //---------------------CAN ENC/REF display-----------------------------//
        ui->LE_FLW_ENC->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(FLW)][MC_GetCH(FLW)].CurrentPosition));
        ui->LE_FRW_ENC->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(FRW)][MC_GetCH(FRW)].CurrentPosition));
        ui->LE_BRW_ENC->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(BRW)][MC_GetCH(BRW)].CurrentPosition));
        ui->LE_BLW_ENC->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(BLW)][MC_GetCH(BLW)].CurrentPosition));
        ui->LE_FLW_REF->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(FLW)][MC_GetCH(FLW)].CurrentReference));
        ui->LE_FRW_REF->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(FRW)][MC_GetCH(FRW)].CurrentReference));
        ui->LE_BRW_REF->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(BRW)][MC_GetCH(BRW)].CurrentReference));
        ui->LE_BLW_REF->setText(QString().sprintf("%.3f",CAN->sharedSEN->ENCODER[MC_GetID(BLW)][MC_GetCH(BLW)].CurrentReference));


        if(CAN->sharedSEN->REF_Enabled)
        {
            ui->BTN_REF_ON->setText(QString().sprintf("REF\nOFF"));
            ui->BTN_REF_ON->setStyleSheet("background-color: green");
        }else
        {
            ui->BTN_REF_ON->setText(QString().sprintf("REF\nON"));
            ui->BTN_REF_ON->setStyleSheet("background-color: red");
        }

        if(CAN->sharedSEN->ENC_Enabled)
        {
            ui->BTN_ENC_ON->setText(QString().sprintf("ENC\nOFF"));
            ui->BTN_ENC_ON->setStyleSheet("background-color: green");
        }else
        {
            ui->BTN_ENC_ON->setText(QString().sprintf("ENC\nON"));
            ui->BTN_ENC_ON->setStyleSheet("background-color: red");
        }

        if(CAN->sharedSEN->JOY_Enabled)
        {
            ui->BTN_JOY_ON->setText(QString().sprintf("Joystick\nConnected"));
            ui->BTN_JOY_ON->setStyleSheet("background-color: green");

            ui->LE_JOY_LUD->setText(QString().sprintf("%d",CAN->sharedSEN->JOY_LJOG_UD));
            ui->LE_JOY_LRL->setText(QString().sprintf("%d",CAN->sharedSEN->JOY_LJOG_RL));
            ui->LE_JOY_RRL->setText(QString().sprintf("%d",CAN->sharedSEN->JOY_RJOG_RL));
        }else
        {
            ui->BTN_JOY_ON->setText(QString().sprintf("Joystick\nConnect"));
            ui->BTN_JOY_ON->setStyleSheet("background-color: red");
        }

        if(FLAG_Firstinit)
            ui->BTN_REF_ON->setEnabled(true);
        else
            ui->BTN_REF_ON->setEnabled(false);

        switch(CAN->sharedSEN->WHEEL_STATE)
                {
                case OMNI_NOT_SET:
                    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel not set"));
                    ui->LE_WHEEL_STATE->setStyleSheet("background-color: red");
                    break;
                case OMNI_BREAK:
                    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel state : idle"));
                    ui->LE_WHEEL_STATE->setStyleSheet("background-color: red");
                    break;
                case OMNI_JOY_ON:
                    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel state : Joystick mode"));
                    ui->LE_WHEEL_STATE->setStyleSheet("background-color: yellow");
                    break;
                case OMNI_VEL_ON:
                    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel state : ROS velocity mode"));
                    ui->LE_WHEEL_STATE->setStyleSheet("background-color: yellow");
                    break;
                case OMNI_MOVING:
                    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel state : Moving"));
                    ui->LE_WHEEL_STATE->setStyleSheet("background-color: yellow");
                    break;
                case OMNI_MOVE_DONE:
                    ui->LE_WHEEL_STATE->setText(QString().sprintf("Wheel state : Move Done"));
                    ui->LE_WHEEL_STATE->setStyleSheet("background-color: green");
                    break;
                }
    }else
    {
        ui->LE_CAN_STATE->setStyleSheet("QLineEdit{background-color:red}");
        ui->LE_CAN_STATE->setText(QString().sprintf("CAN disconnected"));
        ui->BTN_ENC_ON->setText(QString().sprintf("REF\nON"));
        ui->BTN_REF_ON->setStyleSheet("background-color: red");
        ui->BTN_ENC_ON->setText(QString().sprintf("ENC\nON"));
        ui->BTN_ENC_ON->setStyleSheet("background-color: red");
        ui->BTN_JOY_ON->setText(QString().sprintf("Joystick\nConnect"));
        ui->BTN_JOY_ON->setStyleSheet("background-color: red");
        ui->BTN_JOY_START->setText(QString().sprintf("JoyThread\nStart"));
        ui->BTN_JOY_MOVE->setText(QString().sprintf("Wheelmove\nStart"));
    }
}

void CANDialog::on_TW_0_itemSelectionChanged()
{
    if(select_working == true)
        return;

    select_working = true;
    lastSelected = FindLastSelected(0, ui->TW_0->currentRow());
    select_working = false;
}

int CANDialog::FindLastSelected(int tw, int row){
    for(int i=0; i<NO_OF_JOINTS; i++){
        if(TW_ROW_Pairs[i].tw == tw && TW_ROW_Pairs[i].row == row){
            FILE_LOG(logINFO) << i;
            return i;
        }
    }
    return -1;
}

void CANDialog::on_BTN_REF_ON_clicked()
{
    if(ui->BTN_REF_ON->text() == "REF\nON")
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_MOTION_REF_ONOFF;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = true;
    }else
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_MOTION_REF_ONOFF;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = false;
    }
}

void CANDialog::on_BTN_ENC_ON_clicked()
{
    if(ui->BTN_ENC_ON->text() == "ENC\nON")
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_SENSOR_ENCODER_ONOFF;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = true;
    }else
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_SENSOR_ENCODER_ONOFF;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = false;
    }
}

void CANDialog::on_BTN_MOVE_JOINT_clicked()
{
    if(lastSelected < 0)
        return;

    int id = MC_GetID(lastSelected);
    int ch = MC_GetCH(lastSelected);
    float time = ui->LE_JOINT_TIME->text().toFloat();
    float angle = ui->LE_JOINT_ANGLE->text().toFloat();
//    if(lastSelected == FLW || lastSelected == FRW || lastSelected == BLW || lastSelected == BRW)
//        angle /= 200.0;

    USER_COMMAND cmd;
    CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = id;
    CAN->sharedCMD->COMMAND.USER_PARA_CHAR[1] = ch;
    CAN->sharedCMD->COMMAND.USER_PARA_CHAR[2] = 1;     // relative
    CAN->sharedCMD->COMMAND.USER_PARA_FLOAT[0] = time;     // time(ms)
    CAN->sharedCMD->COMMAND.USER_PARA_FLOAT[1] = angle;	// angle
    CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_MOTION_MOVE;

}

void CANDialog::on_BTN_JOY_ON_clicked()
{
    if(ui->BTN_JOY_ON->text() == "Joystick\nConnect")
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_JOY_ON;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = JOY_CONNECT;
    }
}

void CANDialog::on_BTN_JOY_START_clicked()
{
    if(ui->BTN_JOY_START->text() == "JoyThread\nStart")
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_JOY_START;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = JOY_ON;
        ui->BTN_JOY_START->setText(QString().sprintf("JoyThread\nStop"));
    }else
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_JOY_START;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = JOY_OFF;
        ui->BTN_JOY_START->setText(QString().sprintf("JoyThread\nStart"));
    }
}

void CANDialog::on_BTN_JOY_MOVE_clicked()
{
    if(ui->BTN_JOY_MOVE->text() == "Wheelmove\nStart")
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_JOY_MOVE;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = JOY_START;
        ui->BTN_JOY_MOVE->setText(QString().sprintf("Wheelmove\nStop"));
    }else
    {
        CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_JOY_MOVE;
        CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = JOY_STOP;
        ui->BTN_JOY_MOVE->setText(QString().sprintf("Wheelmove\nStart"));
    }
}

void CANDialog::on_pushButton_clicked()
{
    if(CAN->sharedSEN->WHEEL_STATE == OMNI_BREAK)
    {
        double vx = ui->LE_WHEEL_X->text().toDouble();
        double vy = ui->LE_WHEEL_Y->text().toDouble();
        double vr = ui->LE_WHEEL_R->text().toDouble();
        double t = ui->LE_WHEEL_T->text().toDouble();

        if(fabs(vx) > 20.)  vx = 0.;

        if(fabs(vy) > 20.)  vx = 0.;

        if(fabs(vr) > 1000.) vr = 0.;

        if(t > 50. || t < 0.1) t = 0.;

        CAN->sharedCMD->omni_xm = vx;
        CAN->sharedCMD->omni_ym = vy;
        CAN->sharedCMD->omni_yawdeg = vr;
        CAN->sharedCMD->omni_sec = t;
        FLAG_Wheelmove = true;

        CAN->sharedCMD->COMMAND.USER_COMMAND = OMNIWHEEL_MOVE_TO_GOAL;

    }
}

void CANDialog::on_BTN_SAVE_RESET_clicked()
{
    CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_DATA_SAVE;
    CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = 0;
}

void CANDialog::on_pushButton_4_clicked()
{
    CAN->sharedCMD->COMMAND.USER_COMMAND = DAEMON_DATA_SAVE;
    CAN->sharedCMD->COMMAND.USER_PARA_CHAR[0] = 1;

}
