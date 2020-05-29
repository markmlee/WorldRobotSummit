#ifndef MANUALDIALOG_H
#define MANUALDIALOG_H

#include <QDialog>
#include "LAN/RB5connect.h"
#define HIGH    0
#define LOW     1
namespace Ui {
class ManualDialog;
}

class ManualDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManualDialog(QWidget *parent = 0);
    ~ManualDialog();

    void SetStatus(RB5connect *rb5)
    {
        RB5 = rb5;
    }

private slots:
    void threadManual();
    void on_BTN_SUCTION_RESET_clicked();

    void on_BTN_MOTION_MOVE_clicked();

    void on_BTN_MOTION_PAUSE_clicked();

    void on_BTN_MOTION_HALT_clicked();

    void on_BTN_COL_RESUME_clicked();

    void on_BTN_MOTION_RESUME_clicked();

    void on_BTN_CHANGE_SPEED_clicked();

    void on_BTN_GRIPPER_STOP_clicked();

    void on_BTN_GRIPPER_OPEN_clicked();

    void on_BTN_GRIPPER_CLOSE_clicked();

    void on_BNT_SUCTION_clicked();

    void on_BTN_SUCTION_RELEASE_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_BTN_GET_JOINT_clicked();

    void on_BTN_GET_TCP_clicked();

    void on_BTN_GO_HOME_clicked();/* Debug data */

    void on_BTN_BLEND_TEST_clicked();

private:
    Ui::ManualDialog *ui;
    RB5connect *RB5;
    QTimer timer;

    int FLAG_gripper = false;
    int Count = 0;
    int Gripper_ms = 0;
};

#endif // MANUALDIALOG_H
