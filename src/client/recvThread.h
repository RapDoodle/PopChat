#pragma once

#include <QtCore>

#include "chatSocket.h"

class RecvThread : public QThread
{
    Q_OBJECT

public:
    explicit RecvThread(QObject* parent = 0);
    void run();

signals:
    void msgRecvEvent(QString);
};