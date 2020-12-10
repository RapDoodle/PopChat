#include <qapplication.h>
#include <qDebug>
#include <iostream>
#include <thread>
#include <windows.h>

#include "ClientForm.h"
#include "ChatSocket.h"
#include "RecvThread.h"

ClientForm::ClientForm(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(btnConnectHandler()));
    connect(ui.pushButton_Send, SIGNAL(clicked()), this, SLOT(btnSendHandler()));
    connect(ui.lineEdit_Input, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_Input_returnPressed()));
    recvThread = new RecvThread(this);
    connect(recvThread, SIGNAL(msgRecv(QString)), this, SLOT(onMsgRecv(QString)));
}

ClientForm::~ClientForm()
{
    recvThread->terminate();
    QApplication::quit();
}

void ClientForm::btnConnectHandler()
{
    clientStartup(ui.lineEdit_IP->text().toStdString().c_str(), atoi(ui.lineEdit_Port->text().toStdString().c_str()));
    if (isActive()) {
        recvThread->start();
        ui.plainTextEdit_Msg->appendPlainText(QStringLiteral("Connected"));
    } else {
        ui.plainTextEdit_Msg->appendPlainText(QStringLiteral("Unable to connect"));
    }
    
}

void ClientForm::btnSendHandler() 
{
    ui.plainTextEdit_Msg->appendPlainText(QString::fromStdString(sendMsg(ui.lineEdit_Input->text().toStdString().c_str())));
    ui.lineEdit_Input->setText(QStringLiteral(""));
}

void ClientForm::onMsgRecv(QString msg)
{
    ui.plainTextEdit_Msg->appendPlainText(msg.trimmed());
}

void ClientForm::on_lineEdit_Input_returnPressed()
{
    ClientForm::btnSendHandler();
}