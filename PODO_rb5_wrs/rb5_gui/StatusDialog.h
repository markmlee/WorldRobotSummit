#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLineEdit>
#include "DataStructure/rb5data.h"

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDialog(QWidget *parent = nullptr);
    ~StatusDialog();

    systemSTAT  *stat;

    QLineEdit   *le_digital_in_on[16];
    QLineEdit   *le_digital_in_off[16];
    QLineEdit   *le_digital_out_on[16];
    QLineEdit   *le_digital_out_off[16];

    void SetStatus(systemSTAT *input_stat);

public slots:
    void onUpdate();


private:
    Ui::StatusDialog *ui;
    QTimer  updateTimer;
};

#endif // STATUSDIALOG_H
