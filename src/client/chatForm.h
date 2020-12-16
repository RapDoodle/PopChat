#pragma once

#define MAX_WAIT_ATTEMPT 5

#include <QMutex>
#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QCloseEvent>
#include "ui_ChatForm.h"

#include "utils.h"
#include "db.h"
#include "Protocol.h"
#include "RecvThread.h"
#include "MessageBox.h"
#include "ChatSocket.h"
#include "ChatForm.h"

class ChatForm : public QWidget
{
	Q_OBJECT

	public:
		ChatForm(QWidget *parent, QString nickname, QString roomId, QString host);
		~ChatForm();
		RecvThread* recvThread;

	public slots:
		void onMsgRecv(QString);

	private slots:
		void sendBtnClicked();
		void onFieldEnterKeyPressed();

	private:
		Ui::ChatFormClass ui;
		std::string nicknameStrGlobal;
		std::string roomIdStrGlobal;
		std::string hostStrGlobal;
		std::string localSessionId;
		void closeEvent(QCloseEvent* event);
		void renderMsg(QString nickname, QString content);

};
