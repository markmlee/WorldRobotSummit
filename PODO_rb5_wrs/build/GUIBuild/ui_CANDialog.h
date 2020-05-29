/********************************************************************************
** Form generated from reading UI file 'CANDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANDIALOG_H
#define UI_CANDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CANDialog
{
public:
    QTableWidget *TW_0;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *RB_INIT_POS;
    QRadioButton *RB_ENC_ZERO;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *RB_FET_ON;
    QRadioButton *RB_FET_OFF;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *RB_CTRL_ON;
    QRadioButton *RB_CTRL_OFF;
    QFrame *line_5;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *RB_ERROR_CLEAR;
    QRadioButton *RB_JOINT_RECOVER;
    QPushButton *BTN_EXECUTE_COMMAND;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *BTN_CAN_CHECK;
    QPushButton *BTN_FIND_HOME;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_7;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLineEdit *LE_JOINT_ANGLE;
    QLabel *label_8;
    QLineEdit *LE_JOINT_TIME;
    QPushButton *BTN_MOVE_JOINT;
    QFrame *line_2;
    QFrame *line_4;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *BTN_SAVE_RESET;
    QPushButton *pushButton_4;
    QLineEdit *LE_CAN_STATE;
    QWidget *layoutWidget4;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *LE_FLW_REF;
    QLineEdit *LE_FLW_ENC;
    QLabel *label_4;
    QLineEdit *LE_FRW_REF;
    QLineEdit *LE_FRW_ENC;
    QLabel *label_5;
    QLineEdit *LE_BRW_REF;
    QLineEdit *LE_BRW_ENC;
    QLabel *label_6;
    QLineEdit *LE_BLW_REF;
    QLineEdit *LE_BLW_ENC;
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *BTN_REF_ON;
    QPushButton *BTN_ENC_ON;
    QGroupBox *groupBox_2;
    QPushButton *BTN_JOY_START;
    QPushButton *BTN_JOY_ON;
    QPushButton *BTN_JOY_MOVE;
    QWidget *layoutWidget6;
    QGridLayout *gridLayout_4;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *LE_JOY_LUD;
    QLineEdit *LE_JOY_LRL;
    QLineEdit *LE_JOY_RRL;
    QWidget *layoutWidget7;
    QHBoxLayout *horizontalLayout_8;
    QGridLayout *gridLayout_3;
    QLabel *label_9;
    QLineEdit *LE_WHEEL_X;
    QLabel *label_10;
    QLineEdit *LE_WHEEL_Y;
    QLabel *label_11;
    QLineEdit *LE_WHEEL_R;
    QLabel *label_12;
    QLineEdit *LE_WHEEL_T;
    QPushButton *pushButton;
    QFrame *line;
    QLineEdit *LE_WHEEL_STATE;

    void setupUi(QDialog *CANDialog)
    {
        if (CANDialog->objectName().isEmpty())
            CANDialog->setObjectName(QStringLiteral("CANDialog"));
        CANDialog->resize(511, 660);
        TW_0 = new QTableWidget(CANDialog);
        TW_0->setObjectName(QStringLiteral("TW_0"));
        TW_0->setGeometry(QRect(20, 50, 241, 181));
        groupBox = new QGroupBox(CANDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 240, 241, 401));
        groupBox->setStyleSheet(QLatin1String("QGroupBox {\n"
"    border: 1px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 7px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 8px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
"\n"
""));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 160, 181, 181));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        RB_INIT_POS = new QRadioButton(layoutWidget);
        RB_INIT_POS->setObjectName(QStringLiteral("RB_INIT_POS"));
        QFont font;
        font.setPointSize(10);
        RB_INIT_POS->setFont(font);
        RB_INIT_POS->setChecked(true);

        horizontalLayout_2->addWidget(RB_INIT_POS);

        RB_ENC_ZERO = new QRadioButton(layoutWidget);
        RB_ENC_ZERO->setObjectName(QStringLiteral("RB_ENC_ZERO"));
        RB_ENC_ZERO->setFont(font);

        horizontalLayout_2->addWidget(RB_ENC_ZERO);


        verticalLayout->addLayout(horizontalLayout_2);

        line_3 = new QFrame(layoutWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line_3->sizePolicy().hasHeightForWidth());
        line_3->setSizePolicy(sizePolicy);
        line_3->setMinimumSize(QSize(150, 0));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        RB_FET_ON = new QRadioButton(layoutWidget);
        RB_FET_ON->setObjectName(QStringLiteral("RB_FET_ON"));
        RB_FET_ON->setFont(font);

        horizontalLayout_3->addWidget(RB_FET_ON);

        RB_FET_OFF = new QRadioButton(layoutWidget);
        RB_FET_OFF->setObjectName(QStringLiteral("RB_FET_OFF"));
        RB_FET_OFF->setFont(font);

        horizontalLayout_3->addWidget(RB_FET_OFF);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        RB_CTRL_ON = new QRadioButton(layoutWidget);
        RB_CTRL_ON->setObjectName(QStringLiteral("RB_CTRL_ON"));
        RB_CTRL_ON->setFont(font);

        horizontalLayout_4->addWidget(RB_CTRL_ON);

        RB_CTRL_OFF = new QRadioButton(layoutWidget);
        RB_CTRL_OFF->setObjectName(QStringLiteral("RB_CTRL_OFF"));
        RB_CTRL_OFF->setFont(font);

        horizontalLayout_4->addWidget(RB_CTRL_OFF);


        verticalLayout->addLayout(horizontalLayout_4);

        line_5 = new QFrame(layoutWidget);
        line_5->setObjectName(QStringLiteral("line_5"));
        sizePolicy.setHeightForWidth(line_5->sizePolicy().hasHeightForWidth());
        line_5->setSizePolicy(sizePolicy);
        line_5->setMinimumSize(QSize(150, 0));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_5);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        RB_ERROR_CLEAR = new QRadioButton(layoutWidget);
        RB_ERROR_CLEAR->setObjectName(QStringLiteral("RB_ERROR_CLEAR"));
        RB_ERROR_CLEAR->setFont(font);

        horizontalLayout_5->addWidget(RB_ERROR_CLEAR);

        RB_JOINT_RECOVER = new QRadioButton(layoutWidget);
        RB_JOINT_RECOVER->setObjectName(QStringLiteral("RB_JOINT_RECOVER"));
        RB_JOINT_RECOVER->setFont(font);

        horizontalLayout_5->addWidget(RB_JOINT_RECOVER);


        verticalLayout->addLayout(horizontalLayout_5);

        BTN_EXECUTE_COMMAND = new QPushButton(layoutWidget);
        BTN_EXECUTE_COMMAND->setObjectName(QStringLiteral("BTN_EXECUTE_COMMAND"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(BTN_EXECUTE_COMMAND->sizePolicy().hasHeightForWidth());
        BTN_EXECUTE_COMMAND->setSizePolicy(sizePolicy1);
        BTN_EXECUTE_COMMAND->setFont(font);

        verticalLayout->addWidget(BTN_EXECUTE_COMMAND);

        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 20, 201, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        BTN_CAN_CHECK = new QPushButton(layoutWidget1);
        BTN_CAN_CHECK->setObjectName(QStringLiteral("BTN_CAN_CHECK"));
        sizePolicy1.setHeightForWidth(BTN_CAN_CHECK->sizePolicy().hasHeightForWidth());
        BTN_CAN_CHECK->setSizePolicy(sizePolicy1);
        BTN_CAN_CHECK->setFont(font);

        horizontalLayout->addWidget(BTN_CAN_CHECK);

        BTN_FIND_HOME = new QPushButton(layoutWidget1);
        BTN_FIND_HOME->setObjectName(QStringLiteral("BTN_FIND_HOME"));
        sizePolicy1.setHeightForWidth(BTN_FIND_HOME->sizePolicy().hasHeightForWidth());
        BTN_FIND_HOME->setSizePolicy(sizePolicy1);
        BTN_FIND_HOME->setFont(font);

        horizontalLayout->addWidget(BTN_FIND_HOME);

        layoutWidget2 = new QWidget(groupBox);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(20, 80, 201, 54));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_7 = new QLabel(layoutWidget2);
        label_7->setObjectName(QStringLiteral("label_7"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        LE_JOINT_ANGLE = new QLineEdit(layoutWidget2);
        LE_JOINT_ANGLE->setObjectName(QStringLiteral("LE_JOINT_ANGLE"));

        gridLayout_2->addWidget(LE_JOINT_ANGLE, 0, 1, 1, 1);

        label_8 = new QLabel(layoutWidget2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font1);
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        LE_JOINT_TIME = new QLineEdit(layoutWidget2);
        LE_JOINT_TIME->setObjectName(QStringLiteral("LE_JOINT_TIME"));

        gridLayout_2->addWidget(LE_JOINT_TIME, 1, 1, 1, 1);


        horizontalLayout_7->addLayout(gridLayout_2);

        BTN_MOVE_JOINT = new QPushButton(layoutWidget2);
        BTN_MOVE_JOINT->setObjectName(QStringLiteral("BTN_MOVE_JOINT"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(BTN_MOVE_JOINT->sizePolicy().hasHeightForWidth());
        BTN_MOVE_JOINT->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(BTN_MOVE_JOINT);

        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(20, 140, 201, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(groupBox);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(20, 60, 201, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        layoutWidget3 = new QWidget(groupBox);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(20, 350, 201, 41));
        horizontalLayout_15 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        BTN_SAVE_RESET = new QPushButton(layoutWidget3);
        BTN_SAVE_RESET->setObjectName(QStringLiteral("BTN_SAVE_RESET"));
        sizePolicy1.setHeightForWidth(BTN_SAVE_RESET->sizePolicy().hasHeightForWidth());
        BTN_SAVE_RESET->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(BTN_SAVE_RESET);

        pushButton_4 = new QPushButton(layoutWidget3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        sizePolicy1.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(pushButton_4);

        LE_CAN_STATE = new QLineEdit(CANDialog);
        LE_CAN_STATE->setObjectName(QStringLiteral("LE_CAN_STATE"));
        LE_CAN_STATE->setGeometry(QRect(20, 20, 241, 22));
        LE_CAN_STATE->setAlignment(Qt::AlignCenter);
        layoutWidget4 = new QWidget(CANDialog);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(290, 70, 201, 128));
        gridLayout = new QGridLayout(layoutWidget4);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget4);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget4);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        label_3 = new QLabel(layoutWidget4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        LE_FLW_REF = new QLineEdit(layoutWidget4);
        LE_FLW_REF->setObjectName(QStringLiteral("LE_FLW_REF"));

        gridLayout->addWidget(LE_FLW_REF, 1, 1, 1, 1);

        LE_FLW_ENC = new QLineEdit(layoutWidget4);
        LE_FLW_ENC->setObjectName(QStringLiteral("LE_FLW_ENC"));

        gridLayout->addWidget(LE_FLW_ENC, 1, 2, 1, 1);

        label_4 = new QLabel(layoutWidget4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        LE_FRW_REF = new QLineEdit(layoutWidget4);
        LE_FRW_REF->setObjectName(QStringLiteral("LE_FRW_REF"));

        gridLayout->addWidget(LE_FRW_REF, 2, 1, 1, 1);

        LE_FRW_ENC = new QLineEdit(layoutWidget4);
        LE_FRW_ENC->setObjectName(QStringLiteral("LE_FRW_ENC"));

        gridLayout->addWidget(LE_FRW_ENC, 2, 2, 1, 1);

        label_5 = new QLabel(layoutWidget4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font1);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        LE_BRW_REF = new QLineEdit(layoutWidget4);
        LE_BRW_REF->setObjectName(QStringLiteral("LE_BRW_REF"));

        gridLayout->addWidget(LE_BRW_REF, 3, 1, 1, 1);

        LE_BRW_ENC = new QLineEdit(layoutWidget4);
        LE_BRW_ENC->setObjectName(QStringLiteral("LE_BRW_ENC"));

        gridLayout->addWidget(LE_BRW_ENC, 3, 2, 1, 1);

        label_6 = new QLabel(layoutWidget4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        LE_BLW_REF = new QLineEdit(layoutWidget4);
        LE_BLW_REF->setObjectName(QStringLiteral("LE_BLW_REF"));

        gridLayout->addWidget(LE_BLW_REF, 4, 1, 1, 1);

        LE_BLW_ENC = new QLineEdit(layoutWidget4);
        LE_BLW_ENC->setObjectName(QStringLiteral("LE_BLW_ENC"));

        gridLayout->addWidget(LE_BLW_ENC, 4, 2, 1, 1);

        layoutWidget5 = new QWidget(CANDialog);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(330, 30, 161, 38));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        BTN_REF_ON = new QPushButton(layoutWidget5);
        BTN_REF_ON->setObjectName(QStringLiteral("BTN_REF_ON"));
        sizePolicy2.setHeightForWidth(BTN_REF_ON->sizePolicy().hasHeightForWidth());
        BTN_REF_ON->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(BTN_REF_ON);

        BTN_ENC_ON = new QPushButton(layoutWidget5);
        BTN_ENC_ON->setObjectName(QStringLiteral("BTN_ENC_ON"));
        sizePolicy2.setHeightForWidth(BTN_ENC_ON->sizePolicy().hasHeightForWidth());
        BTN_ENC_ON->setSizePolicy(sizePolicy2);

        horizontalLayout_6->addWidget(BTN_ENC_ON);

        groupBox_2 = new QGroupBox(CANDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(280, 240, 221, 401));
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox {\n"
"    border: 1px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 7px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 8px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
"\n"
""));
        BTN_JOY_START = new QPushButton(groupBox_2);
        BTN_JOY_START->setObjectName(QStringLiteral("BTN_JOY_START"));
        BTN_JOY_START->setGeometry(QRect(110, 210, 101, 51));
        BTN_JOY_ON = new QPushButton(groupBox_2);
        BTN_JOY_ON->setObjectName(QStringLiteral("BTN_JOY_ON"));
        BTN_JOY_ON->setGeometry(QRect(10, 210, 91, 51));
        BTN_JOY_MOVE = new QPushButton(groupBox_2);
        BTN_JOY_MOVE->setObjectName(QStringLiteral("BTN_JOY_MOVE"));
        BTN_JOY_MOVE->setGeometry(QRect(10, 270, 201, 51));
        layoutWidget6 = new QWidget(groupBox_2);
        layoutWidget6->setObjectName(QStringLiteral("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 330, 201, 44));
        gridLayout_4 = new QGridLayout(layoutWidget6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(layoutWidget6);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFont(font1);
        label_13->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_13, 0, 0, 1, 1);

        label_14 = new QLabel(layoutWidget6);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFont(font1);
        label_14->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_14, 0, 1, 1, 1);

        label_15 = new QLabel(layoutWidget6);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setFont(font1);
        label_15->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_15, 0, 2, 1, 1);

        LE_JOY_LUD = new QLineEdit(layoutWidget6);
        LE_JOY_LUD->setObjectName(QStringLiteral("LE_JOY_LUD"));

        gridLayout_4->addWidget(LE_JOY_LUD, 1, 0, 1, 1);

        LE_JOY_LRL = new QLineEdit(layoutWidget6);
        LE_JOY_LRL->setObjectName(QStringLiteral("LE_JOY_LRL"));

        gridLayout_4->addWidget(LE_JOY_LRL, 1, 1, 1, 1);

        LE_JOY_RRL = new QLineEdit(layoutWidget6);
        LE_JOY_RRL->setObjectName(QStringLiteral("LE_JOY_RRL"));

        gridLayout_4->addWidget(LE_JOY_RRL, 1, 2, 1, 1);

        layoutWidget7 = new QWidget(groupBox_2);
        layoutWidget7->setObjectName(QStringLiteral("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(10, 60, 201, 110));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget7);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_9 = new QLabel(layoutWidget7);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font1);
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_9, 0, 0, 1, 1);

        LE_WHEEL_X = new QLineEdit(layoutWidget7);
        LE_WHEEL_X->setObjectName(QStringLiteral("LE_WHEEL_X"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(LE_WHEEL_X->sizePolicy().hasHeightForWidth());
        LE_WHEEL_X->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(LE_WHEEL_X, 0, 1, 1, 1);

        label_10 = new QLabel(layoutWidget7);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font1);
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_10, 1, 0, 1, 1);

        LE_WHEEL_Y = new QLineEdit(layoutWidget7);
        LE_WHEEL_Y->setObjectName(QStringLiteral("LE_WHEEL_Y"));
        sizePolicy3.setHeightForWidth(LE_WHEEL_Y->sizePolicy().hasHeightForWidth());
        LE_WHEEL_Y->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(LE_WHEEL_Y, 1, 1, 1, 1);

        label_11 = new QLabel(layoutWidget7);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_11, 2, 0, 1, 1);

        LE_WHEEL_R = new QLineEdit(layoutWidget7);
        LE_WHEEL_R->setObjectName(QStringLiteral("LE_WHEEL_R"));
        sizePolicy3.setHeightForWidth(LE_WHEEL_R->sizePolicy().hasHeightForWidth());
        LE_WHEEL_R->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(LE_WHEEL_R, 2, 1, 1, 1);

        label_12 = new QLabel(layoutWidget7);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font1);
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_12, 3, 0, 1, 1);

        LE_WHEEL_T = new QLineEdit(layoutWidget7);
        LE_WHEEL_T->setObjectName(QStringLiteral("LE_WHEEL_T"));
        sizePolicy3.setHeightForWidth(LE_WHEEL_T->sizePolicy().hasHeightForWidth());
        LE_WHEEL_T->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(LE_WHEEL_T, 3, 1, 1, 1);


        horizontalLayout_8->addLayout(gridLayout_3);

        pushButton = new QPushButton(layoutWidget7);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(60, 0));

        horizontalLayout_8->addWidget(pushButton);

        line = new QFrame(groupBox_2);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(20, 180, 191, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        LE_WHEEL_STATE = new QLineEdit(groupBox_2);
        LE_WHEEL_STATE->setObjectName(QStringLiteral("LE_WHEEL_STATE"));
        LE_WHEEL_STATE->setGeometry(QRect(10, 20, 201, 22));

        retranslateUi(CANDialog);

        QMetaObject::connectSlotsByName(CANDialog);
    } // setupUi

    void retranslateUi(QDialog *CANDialog)
    {
        CANDialog->setWindowTitle(QApplication::translate("CANDialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("CANDialog", "Control", 0));
        RB_INIT_POS->setText(QApplication::translate("CANDialog", "Init Pos", 0));
        RB_ENC_ZERO->setText(QApplication::translate("CANDialog", "Enc Zero", 0));
        RB_FET_ON->setText(QApplication::translate("CANDialog", "FET On", 0));
        RB_FET_OFF->setText(QApplication::translate("CANDialog", "FET Off", 0));
        RB_CTRL_ON->setText(QApplication::translate("CANDialog", "Ctrl On", 0));
        RB_CTRL_OFF->setText(QApplication::translate("CANDialog", "Ctrl Off", 0));
        RB_ERROR_CLEAR->setText(QApplication::translate("CANDialog", "Err Clear", 0));
        RB_JOINT_RECOVER->setText(QApplication::translate("CANDialog", "Recover", 0));
        BTN_EXECUTE_COMMAND->setText(QApplication::translate("CANDialog", "Execute", 0));
        BTN_CAN_CHECK->setText(QApplication::translate("CANDialog", "CAN Check", 0));
        BTN_FIND_HOME->setText(QApplication::translate("CANDialog", "Find Home", 0));
        label_7->setText(QApplication::translate("CANDialog", "Angle", 0));
        LE_JOINT_ANGLE->setText(QApplication::translate("CANDialog", "0.", 0));
        label_8->setText(QApplication::translate("CANDialog", "Time", 0));
        LE_JOINT_TIME->setText(QApplication::translate("CANDialog", "0", 0));
        BTN_MOVE_JOINT->setText(QApplication::translate("CANDialog", "Move\n"
"Joint", 0));
        BTN_SAVE_RESET->setText(QApplication::translate("CANDialog", "Save\n"
"Reset", 0));
        pushButton_4->setText(QApplication::translate("CANDialog", "Data\n"
"Save", 0));
        LE_CAN_STATE->setText(QApplication::translate("CANDialog", "CAN disconnected", 0));
        label->setText(QApplication::translate("CANDialog", "Reference", 0));
        label_2->setText(QApplication::translate("CANDialog", "Encoder", 0));
        label_3->setText(QApplication::translate("CANDialog", "FLW", 0));
        LE_FLW_REF->setText(QApplication::translate("CANDialog", "0.", 0));
        LE_FLW_ENC->setText(QApplication::translate("CANDialog", "0.", 0));
        label_4->setText(QApplication::translate("CANDialog", "FRW", 0));
        LE_FRW_REF->setText(QApplication::translate("CANDialog", "0.", 0));
        LE_FRW_ENC->setText(QApplication::translate("CANDialog", "0.", 0));
        label_5->setText(QApplication::translate("CANDialog", "BRW", 0));
        LE_BRW_REF->setText(QApplication::translate("CANDialog", "0.", 0));
        LE_BRW_ENC->setText(QApplication::translate("CANDialog", "0.", 0));
        label_6->setText(QApplication::translate("CANDialog", "BLW", 0));
        LE_BLW_REF->setText(QApplication::translate("CANDialog", "0.", 0));
        LE_BLW_ENC->setText(QApplication::translate("CANDialog", "0.", 0));
        BTN_REF_ON->setText(QApplication::translate("CANDialog", "REF\n"
"ON", 0));
        BTN_ENC_ON->setText(QApplication::translate("CANDialog", "ENC\n"
"ON", 0));
        groupBox_2->setTitle(QApplication::translate("CANDialog", "OmniWheel", 0));
        BTN_JOY_START->setText(QApplication::translate("CANDialog", "JoyThread\n"
"Start", 0));
        BTN_JOY_ON->setText(QApplication::translate("CANDialog", "Joystick\n"
"Connect", 0));
        BTN_JOY_MOVE->setText(QApplication::translate("CANDialog", "Wheelmove\n"
"Start", 0));
        label_13->setText(QApplication::translate("CANDialog", "R_UD", 0));
        label_14->setText(QApplication::translate("CANDialog", "R_RL", 0));
        label_15->setText(QApplication::translate("CANDialog", "L_RL", 0));
        LE_JOY_LUD->setText(QApplication::translate("CANDialog", "0", 0));
        LE_JOY_LRL->setText(QApplication::translate("CANDialog", "0", 0));
        LE_JOY_RRL->setText(QApplication::translate("CANDialog", "0", 0));
        label_9->setText(QApplication::translate("CANDialog", "x(m)", 0));
        LE_WHEEL_X->setText(QApplication::translate("CANDialog", "0.", 0));
        label_10->setText(QApplication::translate("CANDialog", "y(m)", 0));
        LE_WHEEL_Y->setText(QApplication::translate("CANDialog", "0.", 0));
        label_11->setText(QApplication::translate("CANDialog", "yaw(deg)", 0));
        LE_WHEEL_R->setText(QApplication::translate("CANDialog", "0.", 0));
        label_12->setText(QApplication::translate("CANDialog", "sec", 0));
        LE_WHEEL_T->setText(QApplication::translate("CANDialog", "0", 0));
        pushButton->setText(QApplication::translate("CANDialog", "Wheel\n"
"Move", 0));
    } // retranslateUi

};

namespace Ui {
    class CANDialog: public Ui_CANDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANDIALOG_H
