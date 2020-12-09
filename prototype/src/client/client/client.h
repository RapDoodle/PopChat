#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_client.h"
#include "RecvThread.h"

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = Q_NULLPTR);
    client::~client();
    Ui::clientClass ui;
    RecvThread* recvThread;

public slots:
    void onMsgRecv(QString);

private slots:
    void btnConnectHandler();
    void btnSendHandler();

private:
    
};