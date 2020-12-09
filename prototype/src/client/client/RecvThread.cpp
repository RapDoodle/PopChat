#include "RecvThread.h"
#include <QtCore>
#include <QDebug>
#include "client.h"
#include "socket.h"

RecvThread::RecvThread(QObject* parent) : QThread(parent)
{

}

void RecvThread::run()
{
    emit msgRecv("Hello world");
    while (true) {

        // emit msgRecv(recvMsg());
        emit msgRecv("Hello world");
    }

}