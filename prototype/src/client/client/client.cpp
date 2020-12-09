#include "client.h"
#include <qapplication.h>
#include <qDebug>
#include <iostream>
#include <thread>
#include "socket.h"

#include "RecvThread.h"

client::client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(btnConnectHandler()));
    connect(ui.pushButton_Send, SIGNAL(clicked()), this, SLOT(btnSendHandler()));
    recvThread = new RecvThread(this);
    connect(recvThread, SIGNAL(msgRecv(std::string)), this, SLOT(onMsgRecv(std::string)));
    // connect(recvThread)
}

void client::btnConnectHandler() {
    ui.plainTextEdit_Msg->appendPlainText(QString::fromStdString(clientStartup(ui.lineEdit_IP->text().toStdString().c_str(), atoi(ui.lineEdit_Port->text().toStdString().c_str()))));
    recvThread->start();
}

void client::btnSendHandler() {
    ui.plainTextEdit_Msg->appendPlainText(QString::fromStdString(sendMsg(ui.lineEdit_Input->text().toStdString().c_str())));
}

void client::onMsgRecv(std::string msg)
{
    ui.plainTextEdit_Msg->appendPlainText("New message");
    ui.plainTextEdit_Msg->appendPlainText(QString::fromStdString(msg));
    
}