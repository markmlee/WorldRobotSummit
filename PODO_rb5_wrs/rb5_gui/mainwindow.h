#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "StatusDialog.h"
#include "CANDialog.h"
#include "ManualDialog.h"
#include "LAN/CANconnect.h"
#include "LAN/RB5connect.h"
#include "LAN/ROSconnect.h"

#include <QVector>
#include <signal.h>
#include <QtAlgorithms>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QFileDialog>
#include <QToolBar>
#include <QThread>

namespace Ui {
class MainWindow;
}

class ExpandDialogHandler;
extern qint64  Daemon_processID;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    CANconnect      *CAN;   //Connect to Daemon with SharedMemory
    RB5connect      *RB5;   //Connect to RB5 (send Command, receive Status)
    ROSconnect      *ROS;   //Connect to ROS (send Status, receive Command)

    QTimer systemTimer;     //update data on display (onSystemCheck)
    QTimer pirTimer;
    int             FLAG_pir = false;
    int             detected_cnt = 1;
    int             FLAG_pir_detected = false;
    int             FLAG_suction = false;
    void save_rb5();

    void checkPir(int rst);

public slots:
    void onSystemCheck();
    void pirCheck();

    void onCmdConnected();
    void onCmdDisconnected();
    void onDataConnected();
    void onDataDisconnected();
    void onROSConnected();
    void onROSDisconnected();
    void onRSTConnected();
    void onRSTDisconnected();

    void setCommand(command _cmd);
    void RB5toROS();

    void on_BTN_CONNECT_COM_clicked();
    void on_BTN_CONNECT_DATA_clicked();
    void on_BTN_CONNECT_ROS_clicked();

    void ActionSTATUS_Toggled();
    void ActionCAN_Toggled();
    void ActionMANUAL_Toggled();

private slots:
    void on_BTN_INITIALIZE_clicked();

    void on_BTN_CHANGE_DAEMON_clicked();

    void on_BTN_START_DAEMON_clicked();

    void on_BTN_STOP_DAEMON_clicked();

    void on_showNormal();

    void on_pushButton_3_clicked();

    void on_BTN_SAVE_START_clicked();

    void on_BTN_SAVE_clicked();

private:
    Ui::MainWindow *ui;

    ExpandDialogHandler *expHandler;

    StatusDialog    *statusDialog;
    CANDialog       *canDialog;
    QFrame          *frameStatus;
    QFrame          *frameCAN;

    QString settingFile;
    QAction *restoreAction, *quitAction;
    QAction *startDaemonAction, *stopDaemonAction;

    void createActions();
    int checkJointInput();
    int checkTCPInput();

    int CANenable = false;
    int initFlag;
    int resultFlag;
    int Count;
    command ROScommand;
};


typedef QVector<QDialog*>	Dialogs;
typedef QVector<QFrame*>	Frames;

class ExpandDialogHandler
{
    friend class PODOMainWindow;
private:
    QWidget		*parent;
    Frames		allFrame;
    Dialogs		allDialog;
    Dialogs		visibleDialog;
    Frames		visibleFrame;

    int			startX;
    int			startY;
    int			height;
    int			sideGap;
public:
    ExpandDialogHandler(QWidget *_parent, int x, int y, int h, int gap)
        : parent(_parent), startX(x), startY(y), height(h), sideGap(gap){
        allFrame.clear();
        allDialog.clear();
        visibleFrame.clear();
        visibleDialog.clear();
    }
    ExpandDialogHandler(QWidget *_parent, int gap = 10, int y = 10)
        : parent(_parent){
        startX = ((MainWindow*)parent)->size().width();
        height = ((MainWindow*)parent)->size().height();
        startY = y;
        sideGap = gap;
        allFrame.clear();
        allDialog.clear();
        visibleFrame.clear();
        visibleDialog.clear();
    }

    void registerDialog(QDialog *dialog, QFrame *frame){
        dialog->setWindowFlags(Qt::Widget);
        frame->setFixedSize(dialog->size());
        dialog->move(0,0);

        allDialog.push_back(dialog);
        allFrame.push_back(frame);

        hideDialog(dialog);
    }
    void showDialog(QDialog *dialog){
        // check already shown
        for(int i=0; i<visibleDialog.size(); i++){
            if(visibleDialog[i] == dialog) return;
        }
        // add show list
        visibleDialog.push_back(dialog);
        for(int i=0; i<allDialog.size(); i++){
            if(dialog == allDialog[i]){
                visibleFrame.push_back(allFrame[i]);
                break;
            }
        }
        refreshDialog();
    }
    void hideDialog(QDialog *dialog){
        for(int i=0; i<visibleDialog.size(); i++){
            if(visibleDialog[i] == dialog){
                visibleDialog.remove(i);
                visibleFrame.remove(i);
                break;
            }
        }
        refreshDialog();
    }
    bool isVisible(QDialog *dialog){
        for(int i=0; i<visibleDialog.size(); i++){
            if(visibleDialog[i] == dialog)
                return true;
        }
        return false;
    }

    void refreshDialog(){
        for(int i=0; i<allDialog.size(); i++){
            allFrame[i]->hide();
        }
        int x = startX;
        for(int i=0; i<visibleDialog.size(); i++){
            visibleFrame[i]->move(x, startY);
            visibleFrame[i]->show();
            x = x + visibleFrame[i]->size().width() + sideGap;
        }
        ((MainWindow*)parent)->setFixedWidth(x);
    }
};

#endif // MAINWINDOW_H
