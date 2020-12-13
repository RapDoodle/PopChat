#pragma once

#include <QtCore>

class RecvThread : public QThread
{
    Q_OBJECT

public:
    explicit RecvThread(QObject* parent = 0);
    void run();

signals:
    void msgRecv(QString);
};