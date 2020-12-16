#include "utils.h"
#include "db.h"
#include "Protocol.h"
#include "RecvThread.h"
#include "MessageBox.h"
#include "ChatSocket.h"
#include "ChatForm.h"
#include "ConnectForm.h"
#include <QMessageBox>
#include <QCloseEvent>

#define MAX_WAIT_ATTEMPT 5

ChatForm::ChatForm(QWidget *parent, QString nickname, QString roomId, QString host)
	: QWidget(parent)
{
	ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    connect(ui.sendBtn, SIGNAL(clicked()), this, SLOT(sendBtnClicked()));

	packetSend(PACKET_TYPE_JOINED);
	
	bool success = false;

	for (int i = 0; i < MAX_WAIT_ATTEMPT; i++) {
		string packet = recvMsg();
		int type = universalVerifier(&packet, false);

		if (type < 0) {
			break;

		} else {
			if (to_string(type) == PACKET_TYPE_SERVER_SEND) {
                success = true;
                string nickname = nextParam(&packet);
                renderMsg(QString::fromStdString(nickname), QString::fromStdString(packet));
				recvThread = new RecvThread(this);
				recvThread->start();
                connect(recvThread, SIGNAL(msgRecvEvent(QString)), this, SLOT(onMsgRecv(QString)));
			}
            break;

		}
	}

    hostStrGlobal = host.toStdString();
    nicknameStrGlobal = nickname.toStdString();
    roomIdStrGlobal = roomId.toStdString();

    openDB();
    localSessionId = to_string(createSession(host.toStdString()));
    
    if (!success) {
        msgBoxCritical("Failed to connect. Please try again later.");
    }

}

ChatForm::~ChatForm()
{
    recvThread->terminate();
    /* TO-DO: Reshow the parent window */
}

void ChatForm::sendBtnClicked()
{
    if (ui.messageField->toPlainText().length() > 0) {
        QString retMsg = QString::fromStdString(sendMsg(ui.messageField->toPlainText().toStdString()));

        if (retMsg.length() > 0)
            ui.messages->append(retMsg);
        ui.messageField->clear();
    }
}

void ChatForm::closeEvent(QCloseEvent* event)
{
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, tr("Leave"), QString(tr("Do you want to leave the chat room?")), 
		QMessageBox::Yes | QMessageBox::No);
	if (button == QMessageBox::No) {
		event->ignore();

	} else if (button == QMessageBox::Yes) {
		event->accept();

	}
	/* Code below: before exiting the window */
	
}

void ChatForm::onMsgRecv(QString msg)
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
        string nickname = nextParam(&srchStr);
        /*
        ui.messages->insertHtml(QStringLiteral("<br><br><b>") + QString::fromStdString(nickname) 
            + QStringLiteral("</b> ") + QString::fromStdString(getCurrentTimeString()) + QStringLiteral("<br>"));
        ui.messages->insertPlainText(QString::fromStdString(srchStr).trimmed());
        */
        renderMsg(QString::fromStdString(nickname), QString::fromStdString(srchStr));
        return;
    } else {
        /* Must do nothing to avoid infinite loop */
        return;

    }

    ui.messages->append(QString::fromStdString(srchStr).trimmed());
}

void ChatForm::renderMsg(QString nickname, QString content)
{
    ui.messages->moveCursor(QTextCursor::End);

    /* Duplicated code due to render issues of Qt */
    if (ui.messages->toPlainText().length() == 0) {
        if (nickname.toStdString() != nicknameStrGlobal) {
            ui.messages->insertHtml(QStringLiteral("<p align='left' style='margin:0;'><b>") + nickname
                + QStringLiteral("</b> ") + QString::fromStdString(getCurrentTimeString()) + QStringLiteral(
                    "</p><p align='left' style='margin:0;'>"));
            ui.messages->insertPlainText(content);
            ui.messages->insertHtml("</p><p align='left' style='margin:0;'></p>");
        } else {
            ui.messages->insertHtml(QStringLiteral("<p align='right' style='margin:0;'>") +
                QString::fromStdString(getCurrentTimeString()) + QStringLiteral(" <b>") + nickname +
                QStringLiteral("</b></p><p align='right' style='margin:0;'>"));
            ui.messages->insertPlainText(content);
            ui.messages->insertHtml("</p><p align='right' style='margin:0;'></p>");
        }

    } else {
        if (nickname.toStdString() != nicknameStrGlobal) {
            ui.messages->insertHtml(QStringLiteral("<br><p align='left' style='margin:0;'><b>") + nickname
                + QStringLiteral("</b> ") + QString::fromStdString(getCurrentTimeString()) + QStringLiteral(
                    "</p><p align='left' style='margin:0;'>"));
            ui.messages->insertPlainText(content);
            ui.messages->insertHtml("</p><p align='left' style='margin:0;'></p>");
        } else {
            ui.messages->insertHtml(QStringLiteral("<br><p align='right' style='margin:0;'>") +
                QString::fromStdString(getCurrentTimeString()) + QStringLiteral(" <b>") + nickname +
                QStringLiteral("</b></p><p align='right' style='margin:0;'>"));
            ui.messages->insertPlainText(content);
            ui.messages->insertHtml("</p><p align='right' style='margin:0;'></p>");
        }

    }

    ui.messages->verticalScrollBar()->setValue(ui.messages->verticalScrollBar()->maximum());

    /* Save the message to the local drive */
    insertMessage(localSessionId, nickname.toStdString(), content.toStdString());
    
}