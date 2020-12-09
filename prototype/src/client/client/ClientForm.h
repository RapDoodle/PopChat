#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ClientForm.h"
#include "RecvThread.h"

class ClientForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientForm(QWidget *parent = Q_NULLPTR);
    ClientForm::~ClientForm();
    Ui::clientFormClass ui;
    RecvThread* recvThread;

public slots:
    void onMsgRecv(QString);

private slots:
    void btnConnectHandler();
    void btnSendHandler();
    void on_lineEdit_Input_returnPressed();

private:
    
};