/********************************************************************************
** Form generated from reading UI file 'ManualDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANUALDIALOG_H
#define UI_MANUALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ManualDialog
{
public:
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *BTN_SUCTION_RESET;
    QPushButton *BNT_SUCTION;
    QPushButton *BTN_SUCTION_RELEASE;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QLineEdit *LE_COORDINATE1;
    QLineEdit *LE_COORDINATE2;
    QLineEdit *LE_COORDINATE3;
    QLineEdit *LE_COORDINATE4;
    QLineEdit *LE_COORDINATE5;
    QLineEdit *LE_COORDINATE6;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *LE_SPEED;
    QLabel *label_3;
    QLineEdit *LE_ACC;
    QPushButton *BTN_COL_RESUME;
    QPushButton *BTN_MOTION_RESUME;
    QRadioButton *RB_TCP;
    QPushButton *BTN_MOTION_HALT;
    QPushButton *BTN_MOTION_MOVE;
    QRadioButton *RB_JOINT;
    QPushButton *BTN_MOTION_PAUSE;
    QPushButton *BTN_GET_JOINT;
    QPushButton *BTN_GET_TCP;
    QPushButton *BTN_GO_HOME;
    QGroupBox *groupBox_3;
    QPushButton *BTN_CHANGE_SPEED;
    QSlider *SB_SPEED;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *BTN_GRIPPER_STOP;
    QPushButton *BTN_GRIPPER_OPEN;
    QPushButton *BTN_GRIPPER_CLOSE;
    QLineEdit *LE_GRIPPER_TIME;
    QLabel *label_4;
    QPushButton *BTN_BLEND_TEST;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *ManualDialog)
    {
        if (ManualDialog->objectName().isEmpty())
            ManualDialog->setObjectName(QStringLiteral("ManualDialog"));
        ManualDialog->resize(512, 503);
        groupBox = new QGroupBox(ManualDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(190, 280, 271, 81));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 254, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        BTN_SUCTION_RESET = new QPushButton(layoutWidget);
        BTN_SUCTION_RESET->setObjectName(QStringLiteral("BTN_SUCTION_RESET"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BTN_SUCTION_RESET->sizePolicy().hasHeightForWidth());
        BTN_SUCTION_RESET->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(BTN_SUCTION_RESET);

        BNT_SUCTION = new QPushButton(layoutWidget);
        BNT_SUCTION->setObjectName(QStringLiteral("BNT_SUCTION"));
        sizePolicy.setHeightForWidth(BNT_SUCTION->sizePolicy().hasHeightForWidth());
        BNT_SUCTION->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(BNT_SUCTION);

        BTN_SUCTION_RELEASE = new QPushButton(layoutWidget);
        BTN_SUCTION_RELEASE->setObjectName(QStringLiteral("BTN_SUCTION_RELEASE"));
        sizePolicy.setHeightForWidth(BTN_SUCTION_RELEASE->sizePolicy().hasHeightForWidth());
        BTN_SUCTION_RELEASE->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(BTN_SUCTION_RELEASE);

        groupBox_2 = new QGroupBox(ManualDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 10, 481, 261));
        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 30, 201, 166));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        LE_COORDINATE1 = new QLineEdit(layoutWidget1);
        LE_COORDINATE1->setObjectName(QStringLiteral("LE_COORDINATE1"));

        verticalLayout->addWidget(LE_COORDINATE1);

        LE_COORDINATE2 = new QLineEdit(layoutWidget1);
        LE_COORDINATE2->setObjectName(QStringLiteral("LE_COORDINATE2"));

        verticalLayout->addWidget(LE_COORDINATE2);

        LE_COORDINATE3 = new QLineEdit(layoutWidget1);
        LE_COORDINATE3->setObjectName(QStringLiteral("LE_COORDINATE3"));

        verticalLayout->addWidget(LE_COORDINATE3);

        LE_COORDINATE4 = new QLineEdit(layoutWidget1);
        LE_COORDINATE4->setObjectName(QStringLiteral("LE_COORDINATE4"));

        verticalLayout->addWidget(LE_COORDINATE4);

        LE_COORDINATE5 = new QLineEdit(layoutWidget1);
        LE_COORDINATE5->setObjectName(QStringLiteral("LE_COORDINATE5"));

        verticalLayout->addWidget(LE_COORDINATE5);

        LE_COORDINATE6 = new QLineEdit(layoutWidget1);
        LE_COORDINATE6->setObjectName(QStringLiteral("LE_COORDINATE6"));

        verticalLayout->addWidget(LE_COORDINATE6);


        horizontalLayout_2->addLayout(verticalLayout);

        layoutWidget2 = new QWidget(groupBox_2);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(60, 200, 151, 52));
        gridLayout = new QGridLayout(layoutWidget2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        LE_SPEED = new QLineEdit(layoutWidget2);
        LE_SPEED->setObjectName(QStringLiteral("LE_SPEED"));

        gridLayout->addWidget(LE_SPEED, 0, 1, 1, 1);

        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        LE_ACC = new QLineEdit(layoutWidget2);
        LE_ACC->setObjectName(QStringLiteral("LE_ACC"));

        gridLayout->addWidget(LE_ACC, 1, 1, 1, 1);

        BTN_COL_RESUME = new QPushButton(groupBox_2);
        BTN_COL_RESUME->setObjectName(QStringLiteral("BTN_COL_RESUME"));
        BTN_COL_RESUME->setGeometry(QRect(380, 160, 80, 41));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(BTN_COL_RESUME->sizePolicy().hasHeightForWidth());
        BTN_COL_RESUME->setSizePolicy(sizePolicy1);
        BTN_COL_RESUME->setMinimumSize(QSize(10, 20));
        BTN_MOTION_RESUME = new QPushButton(groupBox_2);
        BTN_MOTION_RESUME->setObjectName(QStringLiteral("BTN_MOTION_RESUME"));
        BTN_MOTION_RESUME->setGeometry(QRect(290, 160, 80, 41));
        sizePolicy1.setHeightForWidth(BTN_MOTION_RESUME->sizePolicy().hasHeightForWidth());
        BTN_MOTION_RESUME->setSizePolicy(sizePolicy1);
        BTN_MOTION_RESUME->setMinimumSize(QSize(10, 20));
        RB_TCP = new QRadioButton(groupBox_2);
        buttonGroup = new QButtonGroup(ManualDialog);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(RB_TCP);
        RB_TCP->setObjectName(QStringLiteral("RB_TCP"));
        RB_TCP->setGeometry(QRect(400, 30, 61, 20));
        BTN_MOTION_HALT = new QPushButton(groupBox_2);
        BTN_MOTION_HALT->setObjectName(QStringLiteral("BTN_MOTION_HALT"));
        BTN_MOTION_HALT->setGeometry(QRect(380, 110, 80, 41));
        sizePolicy1.setHeightForWidth(BTN_MOTION_HALT->sizePolicy().hasHeightForWidth());
        BTN_MOTION_HALT->setSizePolicy(sizePolicy1);
        BTN_MOTION_HALT->setMinimumSize(QSize(10, 20));
        BTN_MOTION_MOVE = new QPushButton(groupBox_2);
        BTN_MOTION_MOVE->setObjectName(QStringLiteral("BTN_MOTION_MOVE"));
        BTN_MOTION_MOVE->setGeometry(QRect(290, 60, 171, 41));
        sizePolicy1.setHeightForWidth(BTN_MOTION_MOVE->sizePolicy().hasHeightForWidth());
        BTN_MOTION_MOVE->setSizePolicy(sizePolicy1);
        BTN_MOTION_MOVE->setMinimumSize(QSize(10, 20));
        RB_JOINT = new QRadioButton(groupBox_2);
        buttonGroup->addButton(RB_JOINT);
        RB_JOINT->setObjectName(QStringLiteral("RB_JOINT"));
        RB_JOINT->setGeometry(QRect(300, 30, 61, 20));
        RB_JOINT->setChecked(true);
        BTN_MOTION_PAUSE = new QPushButton(groupBox_2);
        BTN_MOTION_PAUSE->setObjectName(QStringLiteral("BTN_MOTION_PAUSE"));
        BTN_MOTION_PAUSE->setGeometry(QRect(290, 110, 80, 41));
        sizePolicy1.setHeightForWidth(BTN_MOTION_PAUSE->sizePolicy().hasHeightForWidth());
        BTN_MOTION_PAUSE->setSizePolicy(sizePolicy1);
        BTN_MOTION_PAUSE->setMinimumSize(QSize(10, 20));
        BTN_GET_JOINT = new QPushButton(groupBox_2);
        BTN_GET_JOINT->setObjectName(QStringLiteral("BTN_GET_JOINT"));
        BTN_GET_JOINT->setGeometry(QRect(220, 60, 61, 41));
        BTN_GET_TCP = new QPushButton(groupBox_2);
        BTN_GET_TCP->setObjectName(QStringLiteral("BTN_GET_TCP"));
        BTN_GET_TCP->setGeometry(QRect(220, 110, 61, 41));
        BTN_GO_HOME = new QPushButton(groupBox_2);
        BTN_GO_HOME->setObjectName(QStringLiteral("BTN_GO_HOME"));
        BTN_GO_HOME->setGeometry(QRect(410, 230, 61, 21));
        groupBox_3 = new QGroupBox(ManualDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 290, 151, 161));
        BTN_CHANGE_SPEED = new QPushButton(groupBox_3);
        BTN_CHANGE_SPEED->setObjectName(QStringLiteral("BTN_CHANGE_SPEED"));
        BTN_CHANGE_SPEED->setGeometry(QRect(10, 120, 131, 31));
        SB_SPEED = new QSlider(groupBox_3);
        SB_SPEED->setObjectName(QStringLiteral("SB_SPEED"));
        SB_SPEED->setGeometry(QRect(10, 100, 131, 16));
        SB_SPEED->setMaximum(100);
        SB_SPEED->setSliderPosition(50);
        SB_SPEED->setOrientation(Qt::Horizontal);
        pushButton_2 = new QPushButton(groupBox_3);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 60, 131, 31));
        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 20, 131, 31));
        groupBox_4 = new QGroupBox(ManualDialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(190, 370, 201, 111));
        layoutWidget_2 = new QWidget(groupBox_4);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 30, 181, 41));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        BTN_GRIPPER_STOP = new QPushButton(layoutWidget_2);
        BTN_GRIPPER_STOP->setObjectName(QStringLiteral("BTN_GRIPPER_STOP"));
        sizePolicy.setHeightForWidth(BTN_GRIPPER_STOP->sizePolicy().hasHeightForWidth());
        BTN_GRIPPER_STOP->setSizePolicy(sizePolicy);
        BTN_GRIPPER_STOP->setMinimumSize(QSize(10, 10));

        horizontalLayout_3->addWidget(BTN_GRIPPER_STOP);

        BTN_GRIPPER_OPEN = new QPushButton(layoutWidget_2);
        BTN_GRIPPER_OPEN->setObjectName(QStringLiteral("BTN_GRIPPER_OPEN"));
        sizePolicy.setHeightForWidth(BTN_GRIPPER_OPEN->sizePolicy().hasHeightForWidth());
        BTN_GRIPPER_OPEN->setSizePolicy(sizePolicy);
        BTN_GRIPPER_OPEN->setMinimumSize(QSize(10, 10));

        horizontalLayout_3->addWidget(BTN_GRIPPER_OPEN);

        BTN_GRIPPER_CLOSE = new QPushButton(layoutWidget_2);
        BTN_GRIPPER_CLOSE->setObjectName(QStringLiteral("BTN_GRIPPER_CLOSE"));
        sizePolicy.setHeightForWidth(BTN_GRIPPER_CLOSE->sizePolicy().hasHeightForWidth());
        BTN_GRIPPER_CLOSE->setSizePolicy(sizePolicy);
        BTN_GRIPPER_CLOSE->setMinimumSize(QSize(10, 10));

        horizontalLayout_3->addWidget(BTN_GRIPPER_CLOSE);

        LE_GRIPPER_TIME = new QLineEdit(groupBox_4);
        LE_GRIPPER_TIME->setObjectName(QStringLiteral("LE_GRIPPER_TIME"));
        LE_GRIPPER_TIME->setGeometry(QRect(120, 80, 41, 22));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 80, 71, 21));
        BTN_BLEND_TEST = new QPushButton(ManualDialog);
        BTN_BLEND_TEST->setObjectName(QStringLiteral("BTN_BLEND_TEST"));
        BTN_BLEND_TEST->setGeometry(QRect(410, 460, 80, 22));

        retranslateUi(ManualDialog);

        QMetaObject::connectSlotsByName(ManualDialog);
    } // setupUi

    void retranslateUi(QDialog *ManualDialog)
    {
        ManualDialog->setWindowTitle(QApplication::translate("ManualDialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("ManualDialog", "Suction", 0));
        BTN_SUCTION_RESET->setText(QApplication::translate("ManualDialog", "reset", 0));
        BNT_SUCTION->setText(QApplication::translate("ManualDialog", "suction", 0));
        BTN_SUCTION_RELEASE->setText(QApplication::translate("ManualDialog", "release", 0));
        groupBox_2->setTitle(QApplication::translate("ManualDialog", "Joint/TCP move", 0));
        label->setText(QApplication::translate("ManualDialog", "<html><head/><body><p>JOINT MODE [deg]</p><p>[joint1,2,3,4,5,6]</p><p><br/></p><p>TCP MODE [mm,deg]</p><p>[x,y,z,roll,pitch,yaw]</p></body></html>", 0));
        LE_COORDINATE1->setText(QApplication::translate("ManualDialog", "0.", 0));
        LE_COORDINATE2->setText(QApplication::translate("ManualDialog", "0.", 0));
        LE_COORDINATE3->setText(QApplication::translate("ManualDialog", "0.", 0));
        LE_COORDINATE4->setText(QApplication::translate("ManualDialog", "0.", 0));
        LE_COORDINATE5->setText(QApplication::translate("ManualDialog", "0.", 0));
        LE_COORDINATE6->setText(QApplication::translate("ManualDialog", "0.", 0));
        label_2->setText(QApplication::translate("ManualDialog", "<html><head/><body><p>speed [0~1]</p></body></html>", 0));
        LE_SPEED->setText(QApplication::translate("ManualDialog", "0.5", 0));
        label_3->setText(QApplication::translate("ManualDialog", "<html><head/><body><p>acc [0~1]</p></body></html>", 0));
        LE_ACC->setText(QApplication::translate("ManualDialog", "0.3", 0));
        BTN_COL_RESUME->setText(QApplication::translate("ManualDialog", "Collision\n"
"Resume", 0));
        BTN_MOTION_RESUME->setText(QApplication::translate("ManualDialog", "Motion\n"
"Resume", 0));
        RB_TCP->setText(QApplication::translate("ManualDialog", "TCP", 0));
        BTN_MOTION_HALT->setText(QApplication::translate("ManualDialog", "Halt", 0));
        BTN_MOTION_MOVE->setText(QApplication::translate("ManualDialog", "Move", 0));
        RB_JOINT->setText(QApplication::translate("ManualDialog", "Joint", 0));
        BTN_MOTION_PAUSE->setText(QApplication::translate("ManualDialog", "Pause", 0));
        BTN_GET_JOINT->setText(QApplication::translate("ManualDialog", "Get\n"
"Cur Joint", 0));
        BTN_GET_TCP->setText(QApplication::translate("ManualDialog", "Get\n"
"Cur TCP", 0));
        BTN_GO_HOME->setText(QApplication::translate("ManualDialog", "GoHome", 0));
        groupBox_3->setTitle(QApplication::translate("ManualDialog", "Setting", 0));
        BTN_CHANGE_SPEED->setText(QApplication::translate("ManualDialog", "change\n"
"speed", 0));
#ifndef QT_NO_TOOLTIP
        SB_SPEED->setToolTip(QApplication::translate("ManualDialog", "<html><head/><body><p>speed [0~100%]</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButton_2->setText(QApplication::translate("ManualDialog", "Simulation mode", 0));
        pushButton->setText(QApplication::translate("ManualDialog", "Real mode", 0));
        groupBox_4->setTitle(QApplication::translate("ManualDialog", "Gripper", 0));
        BTN_GRIPPER_STOP->setText(QApplication::translate("ManualDialog", "stop", 0));
        BTN_GRIPPER_OPEN->setText(QApplication::translate("ManualDialog", "open", 0));
        BTN_GRIPPER_CLOSE->setText(QApplication::translate("ManualDialog", "close", 0));
#ifndef QT_NO_TOOLTIP
        LE_GRIPPER_TIME->setToolTip(QApplication::translate("ManualDialog", "[0~600]", 0));
#endif // QT_NO_TOOLTIP
        LE_GRIPPER_TIME->setText(QApplication::translate("ManualDialog", "600", 0));
        label_4->setText(QApplication::translate("ManualDialog", "time [ms]", 0));
        BTN_BLEND_TEST->setText(QApplication::translate("ManualDialog", "blend test", 0));
    } // retranslateUi

};

namespace Ui {
    class ManualDialog: public Ui_ManualDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANUALDIALOG_H
