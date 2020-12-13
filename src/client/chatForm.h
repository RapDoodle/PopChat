#pragma once

#include <QWidget>
#include <QCloseEvent>
#include "ui_ChatForm.h"

class ChatForm : public QWidget
{
	Q_OBJECT

	public:
		ChatForm(QWidget *parent = Q_NULLPTR);
		~ChatForm();

	private:
		Ui::ChatFormClass ui;
		void closeEvent(QCloseEvent* event);
};
