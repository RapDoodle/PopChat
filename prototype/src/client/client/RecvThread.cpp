#include <QtCore>
#include <QMessageBox>
#include <QDebug>
#include <windows.h>

#include "ClientForm.h"
#include "ChatSocket.h"
#include "RecvThread.h"

RecvThread::RecvThread(QObject* parent) 
    : QThread(parent)
{

}

void RecvThread::run()
{
    while (true) {
        string msg = recvMsg();
        if (msg.size() > 0) {
            QString qsmsg = QString::fromStdString(msg);
            emit msgRecv(qsmsg.trimmed());
        }
    }
}