#ifndef CANDIALOG_H
#define CANDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTableWidget>
#include "LAN/CANconnect.h"

namespace Ui {
class CANDialog;
}

class CANDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CANDialog(QWidget *parent = 0);
    ~CANDialog();

    CANconnect  *CAN;
    QTimer displayTimer;
    int     FLAG_Wheelmove;

    void CAN_connect(CANconnect *can);
    void CAN_disconnect();

private slots:
    void    UpdateSettings();

    void on_BTN_CAN_CHECK_clicked();

    void on_BTN_FIND_HOME_clicked();

    void on_BTN_EXECUTE_COMMAND_clicked();

    void on_TW_0_itemSelectionChanged();

    void on_BTN_REF_ON_clicked();

    void on_BTN_ENC_ON_clicked();

    void on_BTN_MOVE_JOINT_clicked();

    void on_BTN_JOY_ON_clicked();

    void on_BTN_JOY_START_clicked();

    void on_BTN_JOY_MOVE_clicked();

    void on_pushButton_clicked();

    void on_BTN_SAVE_RESET_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::CANDialog *ui;
    int     CANflag;
    int     CANconnected;
    int     FLAG_Firstinit;

    void InitTable(QTableWidget *table, QString j_names[], int num);

    int     FindLastSelected(int tw, int row);
    void    ChangeSelectedJoint();
    void    UnselectOtherTable(int table);
    int     select_working;
    int     lastSelected;
};

#endif // CANDIALOG_H
