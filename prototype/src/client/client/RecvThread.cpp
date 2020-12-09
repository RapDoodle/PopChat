#include "RecvThread.h"
#include <QtCore>
#include <QMessageBox>
#include <QDebug>
#include "client.h"
#include "socket.h"
#include <windows.h>

RecvThread::RecvThread(QObject* parent) 
    : QThread(parent)
{

}

void RecvThread::run()
{
    while (true) {
        string msg = recvMsg();
        if (msg != "") {
            QString qsmsg = QString::fromStdString(msg);
            emit msgRecv(qsmsg);
        }
    }
}