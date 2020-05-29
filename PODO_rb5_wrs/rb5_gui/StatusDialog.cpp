#include "StatusDialog.h"
#include "ui_StatusDialog.h"
#include <math.h>


StatusDialog::StatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDialog)
{
    ui->setupUi(this);

    le_digital_in_on[0]  = ui->LE_D_IN_ON_0;
    le_digital_in_on[1]  = ui->LE_D_IN_ON_1;
    le_digital_in_on[2]  = ui->LE_D_IN_ON_2;
    le_digital_in_on[3]  = ui->LE_D_IN_ON_3;
    le_digital_in_on[4]  = ui->LE_D_IN_ON_4;
    le_digital_in_on[5]  = ui->LE_D_IN_ON_5;
    le_digital_in_on[6]  = ui->LE_D_IN_ON_6;
    le_digital_in_on[7]  = ui->LE_D_IN_ON_7;
    le_digital_in_on[8]  = ui->LE_D_IN_ON_8;
    le_digital_in_on[9]  = ui->LE_D_IN_ON_9;
    le_digital_in_on[10] = ui->LE_D_IN_ON_10;
    le_digital_in_on[11] = ui->LE_D_IN_ON_11;
    le_digital_in_on[12] = ui->LE_D_IN_ON_12;
    le_digital_in_on[13] = ui->LE_D_IN_ON_13;
    le_digital_in_on[14] = ui->LE_D_IN_ON_14;
    le_digital_in_on[15] = ui->LE_D_IN_ON_15;

    le_digital_in_off[0]  = ui->LE_D_IN_OFF_0;
    le_digital_in_off[1]  = ui->LE_D_IN_OFF_1;
    le_digital_in_off[2]  = ui->LE_D_IN_OFF_2;
    le_digital_in_off[3]  = ui->LE_D_IN_OFF_3;
    le_digital_in_off[4]  = ui->LE_D_IN_OFF_4;
    le_digital_in_off[5]  = ui->LE_D_IN_OFF_5;
    le_digital_in_off[6]  = ui->LE_D_IN_OFF_6;
    le_digital_in_off[7]  = ui->LE_D_IN_OFF_7;
    le_digital_in_off[8]  = ui->LE_D_IN_OFF_8;
    le_digital_in_off[9]  = ui->LE_D_IN_OFF_9;
    le_digital_in_off[10] = ui->LE_D_IN_OFF_10;
    le_digital_in_off[11] = ui->LE_D_IN_OFF_11;
    le_digital_in_off[12] = ui->LE_D_IN_OFF_12;
    le_digital_in_off[13] = ui->LE_D_IN_OFF_13;
    le_digital_in_off[14] = ui->LE_D_IN_OFF_14;
    le_digital_in_off[15] = ui->LE_D_IN_OFF_15;

    le_digital_out_on[0]  = ui->LE_D_OUT_ON_0;
    le_digital_out_on[1]  = ui->LE_D_OUT_ON_1;
    le_digital_out_on[2]  = ui->LE_D_OUT_ON_2;
    le_digital_out_on[3]  = ui->LE_D_OUT_ON_3;
    le_digital_out_on[4]  = ui->LE_D_OUT_ON_4;
    le_digital_out_on[5]  = ui->LE_D_OUT_ON_5;
    le_digital_out_on[6]  = ui->LE_D_OUT_ON_6;
    le_digital_out_on[7]  = ui->LE_D_OUT_ON_7;
    le_digital_out_on[8]  = ui->LE_D_OUT_ON_8;
    le_digital_out_on[9]  = ui->LE_D_OUT_ON_9;
    le_digital_out_on[10] = ui->LE_D_OUT_ON_10;
    le_digital_out_on[11] = ui->LE_D_OUT_ON_11;
    le_digital_out_on[12] = ui->LE_D_OUT_ON_12;
    le_digital_out_on[13] = ui->LE_D_OUT_ON_13;
    le_digital_out_on[14] = ui->LE_D_OUT_ON_14;
    le_digital_out_on[15] = ui->LE_D_OUT_ON_15;

    le_digital_out_off[0]  = ui->LE_D_OUT_OFF_0;
    le_digital_out_off[1]  = ui->LE_D_OUT_OFF_1;
    le_digital_out_off[2]  = ui->LE_D_OUT_OFF_2;
    le_digital_out_off[3]  = ui->LE_D_OUT_OFF_3;
    le_digital_out_off[4]  = ui->LE_D_OUT_OFF_4;
    le_digital_out_off[5]  = ui->LE_D_OUT_OFF_5;
    le_digital_out_off[6]  = ui->LE_D_OUT_OFF_6;
    le_digital_out_off[7]  = ui->LE_D_OUT_OFF_7;
    le_digital_out_off[8]  = ui->LE_D_OUT_OFF_8;
    le_digital_out_off[9]  = ui->LE_D_OUT_OFF_9;
    le_digital_out_off[10] = ui->LE_D_OUT_OFF_10;
    le_digital_out_off[11] = ui->LE_D_OUT_OFF_11;
    le_digital_out_off[12] = ui->LE_D_OUT_OFF_12;
    le_digital_out_off[13] = ui->LE_D_OUT_OFF_13;
    le_digital_out_off[14] = ui->LE_D_OUT_OFF_14;
    le_digital_out_off[15] = ui->LE_D_OUT_OFF_15;

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
    updateTimer.start(100);
}

StatusDialog::~StatusDialog()
{
    delete ui;
}

void StatusDialog::SetStatus(systemSTAT *input_stat){
    stat = input_stat;
}


void StatusDialog::onUpdate(){
    if(stat->sdata.program_mode == 0){
        ui->LE_PG_MODE_REAL->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_PG_MODE_SIMULATION->setStyleSheet("QLineEdit{background-color:white}");
    }else if(stat->sdata.program_mode == 1){
        ui->LE_PG_MODE_REAL->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_PG_MODE_SIMULATION->setStyleSheet("QLineEdit{background-color:green}");
    }else{
        ui->LE_PG_MODE_REAL->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_PG_MODE_SIMULATION->setStyleSheet("QLineEdit{background-color:white}");
    }

    if(stat->sdata.robot_state == IDLE){
        ui->LE_ROBOT_STATE_IDLE->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_ROBOT_STATE_MOVING->setStyleSheet("QLineEdit{background-color:white}");
    }else if(stat->sdata.robot_state == PAUSED){
        ui->LE_ROBOT_STATE_IDLE->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_ROBOT_STATE_MOVING->setStyleSheet("QLineEdit{background-color:white}");
    }else if(stat->sdata.robot_state == MOVING){
        ui->LE_ROBOT_STATE_IDLE->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_ROBOT_STATE_MOVING->setStyleSheet("QLineEdit{background-color:green}");
    }else{
        ui->LE_ROBOT_STATE_IDLE->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_ROBOT_STATE_MOVING->setStyleSheet("QLineEdit{background-color:white}");
    }


    if(stat->sdata.is_freedrive_mode == 1){
        ui->LE_ROBOT_STATUS_TEACHING->setStyleSheet("QLineEdit{background-color:green}");
    }else{
        ui->LE_ROBOT_STATUS_TEACHING->setStyleSheet("QLineEdit{background-color:white}");
    }
    if(stat->sdata.op_stat_collision_occur == 1){
        ui->LE_ROBOT_STATUS_EXT_COLLISION->setStyleSheet("QLineEdit{background-color:red}");
    }else{
        ui->LE_ROBOT_STATUS_EXT_COLLISION->setStyleSheet("QLineEdit{background-color:white}");
    }
    if(stat->sdata.op_stat_self_collision == 1){
        ui->LE_ROBOT_STATUS_SELF_COLLISION->setStyleSheet("QLineEdit{background-color:red}");
    }else{
        ui->LE_ROBOT_STATUS_SELF_COLLISION->setStyleSheet("QLineEdit{background-color:white}");
    }
    if(stat->sdata.op_stat_soft_estop_occur == 1){
        ui->LE_ROBOT_STATUS_PAUSED->setStyleSheet("QLineEdit{background-color:red}");
    }else{
        ui->LE_ROBOT_STATUS_PAUSED->setStyleSheet("QLineEdit{background-color:white}");
    }
    if(stat->sdata.op_stat_ems_flag != 0){
        ui->LE_ROBOT_STATUS_EMS->setStyleSheet("QLineEdit{background-color:red}");
    }else{
        ui->LE_ROBOT_STATUS_EMS->setStyleSheet("QLineEdit{background-color:white}");
    }
    if(stat->sdata.op_stat_sos_flag == 1){
        ui->LE_ROBOT_STATUS_SOS->setStyleSheet("QLineEdit{background-color:red}");
    }else{
        ui->LE_ROBOT_STATUS_SOS->setStyleSheet("QLineEdit{background-color:white}");
    }


    ui->LE_JNT_ENC_1->setText(QString().sprintf("%.3f", stat->sdata.jnt_ang[0]));
    ui->LE_JNT_ENC_2->setText(QString().sprintf("%.3f", stat->sdata.jnt_ang[1]));
    ui->LE_JNT_ENC_3->setText(QString().sprintf("%.3f", stat->sdata.jnt_ang[2]));
    ui->LE_JNT_ENC_4->setText(QString().sprintf("%.3f", stat->sdata.jnt_ang[3]));
    ui->LE_JNT_ENC_5->setText(QString().sprintf("%.3f", stat->sdata.jnt_ang[4]));
    ui->LE_JNT_ENC_6->setText(QString().sprintf("%.3f", stat->sdata.jnt_ang[5]));

    ui->LE_JNT_REF_1->setText(QString().sprintf("%.3f", stat->sdata.jnt_ref[0]));
    ui->LE_JNT_REF_2->setText(QString().sprintf("%.3f", stat->sdata.jnt_ref[1]));
    ui->LE_JNT_REF_3->setText(QString().sprintf("%.3f", stat->sdata.jnt_ref[2]));
    ui->LE_JNT_REF_4->setText(QString().sprintf("%.3f", stat->sdata.jnt_ref[3]));
    ui->LE_JNT_REF_5->setText(QString().sprintf("%.3f", stat->sdata.jnt_ref[4]));
    ui->LE_JNT_REF_6->setText(QString().sprintf("%.3f", stat->sdata.jnt_ref[5]));

    ui->LE_TCP_REF_X->setText(QString().sprintf("%.3f", stat->sdata.tcp_ref[0]));
    ui->LE_TCP_REF_Y->setText(QString().sprintf("%.3f", stat->sdata.tcp_ref[1]));
    ui->LE_TCP_REF_Z->setText(QString().sprintf("%.3f", stat->sdata.tcp_ref[2]));
    ui->LE_TCP_REF_RX->setText(QString().sprintf("%.3f", stat->sdata.tcp_ref[3]));
    ui->LE_TCP_REF_RY->setText(QString().sprintf("%.3f", stat->sdata.tcp_ref[4]));
    ui->LE_TCP_REF_RZ->setText(QString().sprintf("%.3f", stat->sdata.tcp_ref[5]));


    for(int i=0; i<16; i++){
        if(stat->sdata.digital_in[i] == 0){
            le_digital_in_off[i]->setStyleSheet("QLineEdit{background-color:green}");
            le_digital_in_on[i]->setStyleSheet("QLineEdit{background-color:white}");
        }else if(stat->sdata.digital_in[i] == 1){
            le_digital_in_off[i]->setStyleSheet("QLineEdit{background-color:white}");
            le_digital_in_on[i]->setStyleSheet("QLineEdit{background-color:green}");
        }else{
            le_digital_in_off[i]->setStyleSheet("QLineEdit{background-color:white}");
            le_digital_in_on[i]->setStyleSheet("QLineEdit{background-color:white}");
        }

        if(stat->sdata.digital_out[i] == 0){
            le_digital_out_off[i]->setStyleSheet("QLineEdit{background-color:green}");
            le_digital_out_on[i]->setStyleSheet("QLineEdit{background-color:white}");
        }else if(stat->sdata.digital_out[i] == 1){
            le_digital_out_off[i]->setStyleSheet("QLineEdit{background-color:white}");
            le_digital_out_on[i]->setStyleSheet("QLineEdit{background-color:green}");
        }else{
            le_digital_out_off[i]->setStyleSheet("QLineEdit{background-color:white}");
            le_digital_out_on[i]->setStyleSheet("QLineEdit{background-color:white}");
        }
    }

    ui->LE_A_IN_0->setText(QString().sprintf("%.3f", stat->sdata.analog_in[0]));
    ui->LE_A_IN_1->setText(QString().sprintf("%.3f", stat->sdata.analog_in[1]));
    ui->LE_A_IN_2->setText(QString().sprintf("%.3f", stat->sdata.analog_in[2]));
    ui->LE_A_IN_3->setText(QString().sprintf("%.3f", stat->sdata.analog_in[3]));

    ui->LE_A_OUT_0->setText(QString().sprintf("%.3f", stat->sdata.analog_out[0]));
    ui->LE_A_OUT_1->setText(QString().sprintf("%.3f", stat->sdata.analog_out[1]));
    ui->LE_A_OUT_2->setText(QString().sprintf("%.3f", stat->sdata.analog_out[2]));
    ui->LE_A_OUT_3->setText(QString().sprintf("%.3f", stat->sdata.analog_out[3]));



    if(fabs(stat->sdata.tfb_voltage_out) < 0.01){
        ui->LE_TV_0->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_TV_12->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TV_24->setStyleSheet("QLineEdit{background-color:white}");
    }else if(fabs(stat->sdata.tfb_voltage_out-12.0) < 0.01){
        ui->LE_TV_0->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TV_12->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_TV_24->setStyleSheet("QLineEdit{background-color:white}");
    }else if(fabs(stat->sdata.tfb_voltage_out-24.0) < 0.01){
        ui->LE_TV_0->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TV_12->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TV_24->setStyleSheet("QLineEdit{background-color:green}");
    }else{
        ui->LE_TV_0->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TV_12->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TV_24->setStyleSheet("QLineEdit{background-color:white}");
    }


    if(stat->sdata.tfb_digital_out[0] == 0){
        ui->LE_TD_OUT_ON_0->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TD_OUT_OFF_0->setStyleSheet("QLineEdit{background-color:green}");
    }else if(stat->sdata.tfb_digital_out[0] == 1){
        ui->LE_TD_OUT_ON_0->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_TD_OUT_OFF_0->setStyleSheet("QLineEdit{background-color:white}");
    }else{
        ui->LE_TD_OUT_ON_0->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TD_OUT_OFF_0->setStyleSheet("QLineEdit{background-color:white}");
    }
    if(stat->sdata.tfb_digital_out[1] == 0){
        ui->LE_TD_OUT_ON_1->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TD_OUT_OFF_1->setStyleSheet("QLineEdit{background-color:green}");
    }else if(stat->sdata.tfb_digital_out[1] == 1){
        ui->LE_TD_OUT_ON_1->setStyleSheet("QLineEdit{background-color:green}");
        ui->LE_TD_OUT_OFF_1->setStyleSheet("QLineEdit{background-color:white}");
    }else{
        ui->LE_TD_OUT_ON_1->setStyleSheet("QLineEdit{background-color:white}");
        ui->LE_TD_OUT_OFF_1->setStyleSheet("QLineEdit{background-color:white}");
    }


//    if(stat->sdata.tfb_digital_in[0] == 0){
//        ui->LE_TD_IN_ON_0->setStyleSheet("QLineEdit{background-color:white}");
//        ui->LE_TD_IN_OFF_0->setStyleSheet("QLineEdit{background-color:green}");
//    }else if(stat->sdata.tfb_digital_in[0] == 1){
//        ui->LE_TD_IN_ON_0->setStyleSheet("QLineEdit{background-color:green}");
//        ui->LE_TD_IN_OFF_0->setStyleSheet("QLineEdit{background-color:white}");
//    }else{
//        ui->LE_TD_IN_ON_0->setStyleSheet("QLineEdit{background-color:white}");
//        ui->LE_TD_IN_OFF_0->setStyleSheet("QLineEdit{background-color:white}");
//    }
//    if(stat->sdata.tfb_digital_in[1] == 0){
//        ui->LE_TD_IN_ON_1->setStyleSheet("QLineEdit{background-color:white}");
//        ui->LE_TD_IN_OFF_1->setStyleSheet("QLineEdit{background-color:green}");
//    }else if(stat->sdata.tfb_digital_in[1] == 1){
//        ui->LE_TD_IN_ON_1->setStyleSheet("QLineEdit{background-color:green}");
//        ui->LE_TD_IN_OFF_1->setStyleSheet("QLineEdit{background-color:white}");
//    }else{
//        ui->LE_TD_IN_ON_1->setStyleSheet("QLineEdit{background-color:white}");
//        ui->LE_TD_IN_OFF_1->setStyleSheet("QLineEdit{background-color:white}");
//    }


//    ui->LE_TA_IN_0->setText(QString().sprintf("%.3f", stat->sdata.tfb_analog_in[0]));
//    ui->LE_TA_IN_1->setText(QString().sprintf("%.3f", stat->sdata.tfb_analog_in[1]));

}
