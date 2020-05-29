/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCAN;
    QAction *actionMANUAL;
    QAction *actionSTATUS;
    QAction *actionDUMMY;
    QWidget *centralWidget;
    QGroupBox *GB_NETWORK;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *LE_IP;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLineEdit *textCmdPort;
    QPushButton *BTN_CONNECT_COM;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLineEdit *textDataPort;
    QPushButton *BTN_CONNECT_DATA;
    QWidget *layoutWidget3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *LE_INIT_POWER;
    QLineEdit *LE_INIT_DEVICE;
    QLineEdit *LE_INIT_SYSTEM;
    QLineEdit *LE_INIT_ROBOT;
    QLineEdit *LE_INIT_STATUS;
    QPushButton *BTN_INITIALIZE;
    QGroupBox *GB_NETWORK_2;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *LE_IP_2;
    QWidget *layoutWidget5;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLineEdit *textROSPort;
    QLineEdit *textRSTPort;
    QLabel *label_7;
    QPushButton *BTN_CONNECT_ROS;
    QGroupBox *GB_NETWORK_3;
    QLineEdit *LE_DAEMON_PATH;
    QLabel *label_5;
    QPushButton *BTN_CHANGE_DAEMON;
    QLineEdit *LE_DAEMON_PID;
    QWidget *layoutWidget6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *BTN_START_DAEMON;
    QPushButton *BTN_STOP_DAEMON;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *BTN_SAVE_START;
    QPushButton *BTN_SAVE;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(370, 660);
        QPalette palette;
        QBrush brush(QColor(231, 228, 224, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        MainWindow->setPalette(palette);
        actionCAN = new QAction(MainWindow);
        actionCAN->setObjectName(QStringLiteral("actionCAN"));
        actionCAN->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral("../share/Icon/icon_can.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCAN->setIcon(icon);
        actionMANUAL = new QAction(MainWindow);
        actionMANUAL->setObjectName(QStringLiteral("actionMANUAL"));
        actionMANUAL->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral("../share/Icon/icon_manual.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMANUAL->setIcon(icon1);
        actionSTATUS = new QAction(MainWindow);
        actionSTATUS->setObjectName(QStringLiteral("actionSTATUS"));
        actionSTATUS->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("../share/Icon/icon_status.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSTATUS->setIcon(icon2);
        actionDUMMY = new QAction(MainWindow);
        actionDUMMY->setObjectName(QStringLiteral("actionDUMMY"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("../share/Icon/icon_dummy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDUMMY->setIcon(icon3);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GB_NETWORK = new QGroupBox(centralWidget);
        GB_NETWORK->setObjectName(QStringLiteral("GB_NETWORK"));
        GB_NETWORK->setGeometry(QRect(10, 10, 241, 261));
        QFont font;
        font.setPointSize(10);
        GB_NETWORK->setFont(font);
        GB_NETWORK->setLayoutDirection(Qt::LeftToRight);
        layoutWidget = new QWidget(GB_NETWORK);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 30, 201, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        LE_IP = new QLineEdit(layoutWidget);
        LE_IP->setObjectName(QStringLiteral("LE_IP"));
        LE_IP->setMaximumSize(QSize(150, 16777215));
        LE_IP->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(LE_IP);

        layoutWidget1 = new QWidget(GB_NETWORK);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 60, 82, 81));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        QFont font1;
        font1.setPointSize(9);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        textCmdPort = new QLineEdit(layoutWidget1);
        textCmdPort->setObjectName(QStringLiteral("textCmdPort"));
        textCmdPort->setEnabled(true);
        textCmdPort->setMaximumSize(QSize(100, 16777215));
        textCmdPort->setAlignment(Qt::AlignCenter);
        textCmdPort->setReadOnly(false);

        verticalLayout->addWidget(textCmdPort);

        BTN_CONNECT_COM = new QPushButton(layoutWidget1);
        BTN_CONNECT_COM->setObjectName(QStringLiteral("BTN_CONNECT_COM"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BTN_CONNECT_COM->sizePolicy().hasHeightForWidth());
        BTN_CONNECT_COM->setSizePolicy(sizePolicy);
        BTN_CONNECT_COM->setMinimumSize(QSize(0, 40));

        verticalLayout->addWidget(BTN_CONNECT_COM);

        layoutWidget2 = new QWidget(GB_NETWORK);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(130, 60, 82, 81));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget2);
        label_3->setObjectName(QStringLiteral("label_3"));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setUnderline(false);
        font2.setWeight(50);
        font2.setStrikeOut(false);
        font2.setKerning(true);
        label_3->setFont(font2);
        label_3->setMouseTracking(false);
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_3);

        textDataPort = new QLineEdit(layoutWidget2);
        textDataPort->setObjectName(QStringLiteral("textDataPort"));
        textDataPort->setEnabled(true);
        textDataPort->setMaximumSize(QSize(100, 16777215));
        textDataPort->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(textDataPort);

        BTN_CONNECT_DATA = new QPushButton(layoutWidget2);
        BTN_CONNECT_DATA->setObjectName(QStringLiteral("BTN_CONNECT_DATA"));
        sizePolicy.setHeightForWidth(BTN_CONNECT_DATA->sizePolicy().hasHeightForWidth());
        BTN_CONNECT_DATA->setSizePolicy(sizePolicy);
        BTN_CONNECT_DATA->setMinimumSize(QSize(0, 40));

        verticalLayout_2->addWidget(BTN_CONNECT_DATA);

        layoutWidget3 = new QWidget(GB_NETWORK);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(20, 150, 201, 100));
        verticalLayout_4 = new QVBoxLayout(layoutWidget3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        LE_INIT_POWER = new QLineEdit(layoutWidget3);
        LE_INIT_POWER->setObjectName(QStringLiteral("LE_INIT_POWER"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LE_INIT_POWER->sizePolicy().hasHeightForWidth());
        LE_INIT_POWER->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(LE_INIT_POWER);

        LE_INIT_DEVICE = new QLineEdit(layoutWidget3);
        LE_INIT_DEVICE->setObjectName(QStringLiteral("LE_INIT_DEVICE"));
        sizePolicy1.setHeightForWidth(LE_INIT_DEVICE->sizePolicy().hasHeightForWidth());
        LE_INIT_DEVICE->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(LE_INIT_DEVICE);

        LE_INIT_SYSTEM = new QLineEdit(layoutWidget3);
        LE_INIT_SYSTEM->setObjectName(QStringLiteral("LE_INIT_SYSTEM"));
        sizePolicy1.setHeightForWidth(LE_INIT_SYSTEM->sizePolicy().hasHeightForWidth());
        LE_INIT_SYSTEM->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(LE_INIT_SYSTEM);

        LE_INIT_ROBOT = new QLineEdit(layoutWidget3);
        LE_INIT_ROBOT->setObjectName(QStringLiteral("LE_INIT_ROBOT"));
        sizePolicy1.setHeightForWidth(LE_INIT_ROBOT->sizePolicy().hasHeightForWidth());
        LE_INIT_ROBOT->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(LE_INIT_ROBOT);


        verticalLayout_4->addLayout(horizontalLayout_3);

        LE_INIT_STATUS = new QLineEdit(layoutWidget3);
        LE_INIT_STATUS->setObjectName(QStringLiteral("LE_INIT_STATUS"));
        LE_INIT_STATUS->setEnabled(true);
        LE_INIT_STATUS->setReadOnly(true);

        verticalLayout_4->addWidget(LE_INIT_STATUS);

        BTN_INITIALIZE = new QPushButton(layoutWidget3);
        BTN_INITIALIZE->setObjectName(QStringLiteral("BTN_INITIALIZE"));
        sizePolicy1.setHeightForWidth(BTN_INITIALIZE->sizePolicy().hasHeightForWidth());
        BTN_INITIALIZE->setSizePolicy(sizePolicy1);
        BTN_INITIALIZE->setFont(font);

        verticalLayout_4->addWidget(BTN_INITIALIZE);

        GB_NETWORK_2 = new QGroupBox(centralWidget);
        GB_NETWORK_2->setObjectName(QStringLiteral("GB_NETWORK_2"));
        GB_NETWORK_2->setGeometry(QRect(10, 280, 241, 161));
        GB_NETWORK_2->setFont(font);
        layoutWidget4 = new QWidget(GB_NETWORK_2);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(19, 29, 201, 25));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget4);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        LE_IP_2 = new QLineEdit(layoutWidget4);
        LE_IP_2->setObjectName(QStringLiteral("LE_IP_2"));
        LE_IP_2->setMaximumSize(QSize(150, 16777215));
        LE_IP_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(LE_IP_2);

        layoutWidget5 = new QWidget(GB_NETWORK_2);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(50, 60, 141, 87));
        verticalLayout_3 = new QVBoxLayout(layoutWidget5);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setVerticalSpacing(0);
        label_6 = new QLabel(layoutWidget5);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 0, 0, 1, 1);

        textROSPort = new QLineEdit(layoutWidget5);
        textROSPort->setObjectName(QStringLiteral("textROSPort"));
        textROSPort->setEnabled(true);
        textROSPort->setMaximumSize(QSize(100, 16777215));
        textROSPort->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(textROSPort, 1, 0, 1, 1);

        textRSTPort = new QLineEdit(layoutWidget5);
        textRSTPort->setObjectName(QStringLiteral("textRSTPort"));
        textRSTPort->setEnabled(true);
        textRSTPort->setMaximumSize(QSize(100, 16777215));
        textRSTPort->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(textRSTPort, 1, 1, 1, 1);

        label_7 = new QLabel(layoutWidget5);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font1);
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 0, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        BTN_CONNECT_ROS = new QPushButton(layoutWidget5);
        BTN_CONNECT_ROS->setObjectName(QStringLiteral("BTN_CONNECT_ROS"));
        sizePolicy1.setHeightForWidth(BTN_CONNECT_ROS->sizePolicy().hasHeightForWidth());
        BTN_CONNECT_ROS->setSizePolicy(sizePolicy1);
        BTN_CONNECT_ROS->setMinimumSize(QSize(0, 40));

        verticalLayout_3->addWidget(BTN_CONNECT_ROS);

        GB_NETWORK_3 = new QGroupBox(centralWidget);
        GB_NETWORK_3->setObjectName(QStringLiteral("GB_NETWORK_3"));
        GB_NETWORK_3->setGeometry(QRect(10, 450, 241, 151));
        GB_NETWORK_3->setFont(font);
        LE_DAEMON_PATH = new QLineEdit(GB_NETWORK_3);
        LE_DAEMON_PATH->setObjectName(QStringLiteral("LE_DAEMON_PATH"));
        LE_DAEMON_PATH->setGeometry(QRect(10, 50, 171, 21));
        label_5 = new QLabel(GB_NETWORK_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 30, 101, 23));
        BTN_CHANGE_DAEMON = new QPushButton(GB_NETWORK_3);
        BTN_CHANGE_DAEMON->setObjectName(QStringLiteral("BTN_CHANGE_DAEMON"));
        BTN_CHANGE_DAEMON->setGeometry(QRect(180, 40, 51, 31));
        QFont font3;
        font3.setPointSize(8);
        BTN_CHANGE_DAEMON->setFont(font3);
        LE_DAEMON_PID = new QLineEdit(GB_NETWORK_3);
        LE_DAEMON_PID->setObjectName(QStringLiteral("LE_DAEMON_PID"));
        LE_DAEMON_PID->setEnabled(false);
        LE_DAEMON_PID->setGeometry(QRect(180, 70, 51, 21));
        LE_DAEMON_PID->setReadOnly(true);
        layoutWidget6 = new QWidget(GB_NETWORK_3);
        layoutWidget6->setObjectName(QStringLiteral("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(20, 100, 201, 41));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget6);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        BTN_START_DAEMON = new QPushButton(layoutWidget6);
        BTN_START_DAEMON->setObjectName(QStringLiteral("BTN_START_DAEMON"));
        sizePolicy.setHeightForWidth(BTN_START_DAEMON->sizePolicy().hasHeightForWidth());
        BTN_START_DAEMON->setSizePolicy(sizePolicy);
        BTN_START_DAEMON->setFont(font);

        horizontalLayout_4->addWidget(BTN_START_DAEMON);

        BTN_STOP_DAEMON = new QPushButton(layoutWidget6);
        BTN_STOP_DAEMON->setObjectName(QStringLiteral("BTN_STOP_DAEMON"));
        sizePolicy.setHeightForWidth(BTN_STOP_DAEMON->sizePolicy().hasHeightForWidth());
        BTN_STOP_DAEMON->setSizePolicy(sizePolicy);
        BTN_STOP_DAEMON->setFont(font);

        horizontalLayout_4->addWidget(BTN_STOP_DAEMON);

        layoutWidget_2 = new QWidget(centralWidget);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(40, 620, 171, 31));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        BTN_SAVE_START = new QPushButton(layoutWidget_2);
        BTN_SAVE_START->setObjectName(QStringLiteral("BTN_SAVE_START"));
        sizePolicy.setHeightForWidth(BTN_SAVE_START->sizePolicy().hasHeightForWidth());
        BTN_SAVE_START->setSizePolicy(sizePolicy);
        BTN_SAVE_START->setFont(font);

        horizontalLayout_5->addWidget(BTN_SAVE_START);

        BTN_SAVE = new QPushButton(layoutWidget_2);
        BTN_SAVE->setObjectName(QStringLiteral("BTN_SAVE"));
        sizePolicy.setHeightForWidth(BTN_SAVE->sizePolicy().hasHeightForWidth());
        BTN_SAVE->setSizePolicy(sizePolicy);
        BTN_SAVE->setFont(font);

        horizontalLayout_5->addWidget(BTN_SAVE);

        MainWindow->setCentralWidget(centralWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolBar->sizePolicy().hasHeightForWidth());
        toolBar->setSizePolicy(sizePolicy2);
        QPalette palette1;
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        QBrush brush3(QColor(77, 77, 77, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush1);
        QBrush brush4(QColor(212, 212, 159, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush4);
        QBrush brush5(QColor(19, 85, 74, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush5);
        QBrush brush6(QColor(113, 113, 84, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush1);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        QBrush brush7(QColor(67, 255, 89, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush7);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        QBrush brush8(QColor(212, 212, 191, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush8);
        QBrush brush9(QColor(131, 255, 235, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush9);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        QBrush brush10(QColor(170, 170, 127, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        toolBar->setPalette(palette1);
        toolBar->setLayoutDirection(Qt::LeftToRight);
        toolBar->setIconSize(QSize(100, 100));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);
        QWidget::setTabOrder(BTN_CONNECT_ROS, BTN_CONNECT_COM);
        QWidget::setTabOrder(BTN_CONNECT_COM, textDataPort);
        QWidget::setTabOrder(textDataPort, LE_IP);
        QWidget::setTabOrder(LE_IP, BTN_CONNECT_DATA);
        QWidget::setTabOrder(BTN_CONNECT_DATA, LE_INIT_POWER);
        QWidget::setTabOrder(LE_INIT_POWER, LE_INIT_DEVICE);
        QWidget::setTabOrder(LE_INIT_DEVICE, LE_INIT_SYSTEM);
        QWidget::setTabOrder(LE_INIT_SYSTEM, LE_INIT_ROBOT);
        QWidget::setTabOrder(LE_INIT_ROBOT, BTN_INITIALIZE);
        QWidget::setTabOrder(BTN_INITIALIZE, LE_IP_2);
        QWidget::setTabOrder(LE_IP_2, textROSPort);
        QWidget::setTabOrder(textROSPort, textRSTPort);
        QWidget::setTabOrder(textRSTPort, textCmdPort);
        QWidget::setTabOrder(textCmdPort, LE_DAEMON_PATH);
        QWidget::setTabOrder(LE_DAEMON_PATH, BTN_CHANGE_DAEMON);
        QWidget::setTabOrder(BTN_CHANGE_DAEMON, LE_DAEMON_PID);
        QWidget::setTabOrder(LE_DAEMON_PID, BTN_START_DAEMON);
        QWidget::setTabOrder(BTN_START_DAEMON, BTN_STOP_DAEMON);

        toolBar->addAction(actionSTATUS);
        toolBar->addAction(actionMANUAL);
        toolBar->addAction(actionDUMMY);
        toolBar->addAction(actionCAN);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionCAN->setText(QApplication::translate("MainWindow", "CAN", 0));
#ifndef QT_NO_TOOLTIP
        actionCAN->setToolTip(QApplication::translate("MainWindow", "CAN dialog", 0));
#endif // QT_NO_TOOLTIP
        actionMANUAL->setText(QApplication::translate("MainWindow", "MANUAL", 0));
#ifndef QT_NO_TOOLTIP
        actionMANUAL->setToolTip(QApplication::translate("MainWindow", "manual dialog", 0));
#endif // QT_NO_TOOLTIP
        actionSTATUS->setText(QApplication::translate("MainWindow", "STATUS", 0));
#ifndef QT_NO_TOOLTIP
        actionSTATUS->setToolTip(QApplication::translate("MainWindow", "STATUS dialog", 0));
#endif // QT_NO_TOOLTIP
        actionDUMMY->setText(QApplication::translate("MainWindow", "DUMMY", 0));
        GB_NETWORK->setTitle(QApplication::translate("MainWindow", "RB5 Connection", 0));
        label->setText(QApplication::translate("MainWindow", "RB5 IP", 0));
        LE_IP->setText(QApplication::translate("MainWindow", "127.0.0.1", 0));
        label_2->setText(QApplication::translate("MainWindow", "CMD PORT", 0));
        textCmdPort->setText(QApplication::translate("MainWindow", "5000", 0));
        BTN_CONNECT_COM->setText(QApplication::translate("MainWindow", "Connect", 0));
        label_3->setText(QApplication::translate("MainWindow", "DATA PORT", 0));
        textDataPort->setText(QApplication::translate("MainWindow", "5001", 0));
        BTN_CONNECT_DATA->setText(QApplication::translate("MainWindow", "Connect", 0));
        BTN_INITIALIZE->setText(QApplication::translate("MainWindow", "RB5\n"
"Initialize", 0));
        GB_NETWORK_2->setTitle(QApplication::translate("MainWindow", "ROS Connection", 0));
        label_4->setText(QApplication::translate("MainWindow", "ROS IP", 0));
        LE_IP_2->setText(QApplication::translate("MainWindow", "127.0.0.1", 0));
        label_6->setText(QApplication::translate("MainWindow", "ROS PORT", 0));
        textROSPort->setText(QApplication::translate("MainWindow", "4000", 0));
        textRSTPort->setText(QApplication::translate("MainWindow", "4001", 0));
        label_7->setText(QApplication::translate("MainWindow", "RST PORT", 0));
        BTN_CONNECT_ROS->setText(QApplication::translate("MainWindow", "Open", 0));
        GB_NETWORK_3->setTitle(QApplication::translate("MainWindow", "CAN Connection", 0));
        label_5->setText(QApplication::translate("MainWindow", "DAEMON PATH", 0));
        BTN_CHANGE_DAEMON->setText(QApplication::translate("MainWindow", "Change\n"
"Daemon", 0));
        BTN_START_DAEMON->setText(QApplication::translate("MainWindow", "Start\n"
"Daemon", 0));
        BTN_STOP_DAEMON->setText(QApplication::translate("MainWindow", "Stop\n"
"Daemon", 0));
        BTN_SAVE_START->setText(QApplication::translate("MainWindow", "Save Start", 0));
        BTN_SAVE->setText(QApplication::translate("MainWindow", "Save", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
