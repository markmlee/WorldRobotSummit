#include "mainwindow.h"
#include <QApplication>
#include <signal.h>
#include <stdlib.h>

qint64  Daemon_processID;

void CatchSignals(int _signal)
{
    printf("%d\n",_signal);
    kill(Daemon_processID, SIGKILL);
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, CatchSignals);  /* Termination (ANSI).  */
    signal(SIGINT,  CatchSignals);   /* Interrupt (ANSI).  */
    signal(SIGHUP,  CatchSignals);   /* Hangup (POSIX).  */
    signal(SIGKILL, CatchSignals);  /* Kill, unblockable (POSIX).  */
    signal(SIGSEGV, CatchSignals);  /* Segmentation violation (ANSI).  */

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
