#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_client.h"
#include "RecvThread.h"

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = Q_NULLPTR);
    Ui::clientClass ui;
    RecvThread* recvThread;

public slots:
    void onMsgRecv(std::string);

private slots:
    void btnConnectHandler();
    void btnSendHandler();

private:
    
};