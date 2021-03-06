#include <QtCore>
#include <QDebug>
#include <QMessageBox>

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

        if (!isActive())
            this->terminate();

        if (msg.size() > 0) {
            QString qsmsg = QString::fromStdString(msg);
            emit msgRecv(qsmsg.trimmed());
        }
    }
}