#pragma once

#include <QMutex>
#include <QWidget>
#include <QCloseEvent>
#include <QKeyEvent>
#include "ui_ChatForm.h"

#include "RecvThread.h"

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

	private:
		Ui::ChatFormClass ui;
		std::string nicknameStrGlobal;
		std::string roomIdStrGlobal;
		std::string hostStrGlobal;
		std::string localSessionId;
		void closeEvent(QCloseEvent* event);
		void renderMsg(QString nickname, QString content);

};
