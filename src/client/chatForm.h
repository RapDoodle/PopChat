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
		ChatForm(QWidget *parent = Q_NULLPTR);
		~ChatForm();
		RecvThread* recvThread;

	public slots:
		void onMsgRecv(QString);

	private slots:
		void sendBtnClicked();

	private:
		Ui::ChatFormClass ui;
		void closeEvent(QCloseEvent* event);

};
