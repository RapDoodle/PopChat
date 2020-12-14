#pragma once

#include <QWidget>

#include "ui_EntranceForm.h"
#include "ChatForm.h"

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
