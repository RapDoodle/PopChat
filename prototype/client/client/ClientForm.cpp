#ifndef __INTELLISENSE__
#include <QMutex>
#endif

#include <QMessageBox>
#include <qapplication.h>
#include <qDebug>
#include <iostream>
#include <thread>
#include <windows.h>

#include "ClientForm.h"
#include "ChatSocket.h"
#include "RecvThread.h"
#include "protocol.h"

ClientForm::ClientForm(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(btnConnectHandler()));
    connect(ui.pushButton_Send, SIGNAL(clicked()), this, SLOT(btnSendHandler()));
    connect(ui.lineEdit_Input, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_Input_returnPressed()));
    connect(ui.pushButton_Create, SIGNAL(clicked()), this, SLOT(btnCreateHandler()));
    connect(ui.pushButton_Join, SIGNAL(clicked()), this, SLOT(btnJoinHandler()));
    connect(ui.pushButton_Leave, SIGNAL(clicked()), this, SLOT(btnLeaveHandler()));
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
        msgBoxInfo("Connected.");
    } else {
        msgBoxCritical("Unable to connect to the server. Please verify the IP address and the port number.");
    }
}

void ClientForm::btnSendHandler() 
{
    if (ui.lineEdit_Input->text().length() > 0) {
        QString retMsg = QString::fromStdString(sendMsg(ui.lineEdit_Input->text().toStdString()));

        if (retMsg.length() > 0)
            ui.plainTextEdit_Msg->appendPlainText(retMsg);
        ui.lineEdit_Input->clear();
    }
}

void ClientForm::onMsgRecv(QString msg)
{
    string srchStr = msg.toStdString();

    /* The string for searching (will be modified) */
    string version = nextParam(&srchStr);

    if (version != PROTOCOL_VERSION) {
        return;  // Ignore the message
    }

    string checkSum = nextParam(&srchStr);
    string calCheckSum = to_string(calculateCheckSum(srchStr));
    if (checkSum != calCheckSum) {
        // TO-DO: Request for retransmission
        return;  // Error packet
    }

    string type = nextParam(&srchStr);

    /* Route different packets to different routine */
    if (type == PACKET_TYPE_PING) {
        /* Ping */
        packetSend(PACKET_TYPE_PONG);
    } else if (type == PACKET_TYPE_SUCCESS) {
        /* Success opration */
        if (srchStr == "Connected")
            return;  // Ignore the connected message
    } else if (type == PACKET_TYPE_FAILED) {
        /* Failed operation */
    } else if (type == PACKET_TYPE_VALIDATION_ERROR) {
        /* Packet validation error */
    } else if (type == PACKET_TYPE_ERROR) {
        /* Error */
    } else if (type == PACKET_TYPE_SERVER_SEND) {
        /* Data from server */
    } else {
        /* Must do nothing to avoid infinite loop */
        return;
    }
    ui.plainTextEdit_Msg->appendPlainText(QString::fromStdString(srchStr).trimmed());
}

void ClientForm::on_lineEdit_Input_returnPressed()
{
    ClientForm::btnSendHandler();
}

void ClientForm::btnJoinHandler()
{
    if (ui.lineEdit_Room->text().length() == 0) {
        msgBoxWarning("The field for room number cannot be empty");
        return;
    }

    if (ui.lineEdit_Nickname->text().length() == 0) {
        msgBoxWarning("The field for nickname cannot be empty");
        return;
    }

    int res = packetSend(PACKET_TYPE_JOIN_ROOM " " + ui.lineEdit_Room->text().toStdString() +
        " " + ui.lineEdit_Password->text().toStdString() +
        " " + ui.lineEdit_Nickname->text().toStdString());

    if (res == SOCKET_ERROR)
        msgBoxCritical("Netowrk error");
}

void ClientForm::btnCreateHandler()
{
    if (ui.lineEdit_Nickname->text().length() == 0) {
        msgBoxWarning("The field for nickname cannot be empty");
        return;
    }

    int res = packetSend(PACKET_TYPE_CREATE_ROOM " " +
        ui.lineEdit_Password->text().toStdString() + 
        " " + ui.lineEdit_Nickname->text().toStdString());

    if (res == SOCKET_ERROR)
        msgBoxCritical("Netowrk error");
}

void ClientForm::btnLeaveHandler()
{
    packetSend(PACKET_TYPE_LEAVE_ROOM);
}

void ClientForm::msgBoxInfo(QString s)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Infomation");
    msgBox.setText(s);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void ClientForm::msgBoxWarning(QString s)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Message");
    msgBox.setText(s);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void ClientForm::msgBoxCritical(QString s)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(s);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}