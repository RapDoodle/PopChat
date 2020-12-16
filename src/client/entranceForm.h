#pragma once

#include <QWidget>
#include <QCloseEvent>

#include "ui_EntranceForm.h"
#include "MessageBox.h"
#include "ChatSocket.h"
#include "ChatForm.h"
#include "Protocol.h"
#include "utils.h"

class EntranceForm : public QWidget
{
	Q_OBJECT

	public:
		EntranceForm(QWidget *parent = Q_NULLPTR);
		~EntranceForm();
		std::string hostStrGlobal;

	private slots:
		void btnCreateHandler();
		void btnJoinHandler();

	private:
		Ui::EntranceFormClass ui;
		ChatForm* room;
		void toChatForm(QString roomID, QString nickname);
};
