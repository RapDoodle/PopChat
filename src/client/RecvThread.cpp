#include <QtCore>
#include <QDebug>
#include <QMessageBox>

#include "chatSocket.h"
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
            emit msgRecvEvent(qsmsg.trimmed());
        }
    }
}