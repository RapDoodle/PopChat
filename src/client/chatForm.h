#pragma once

#include <QWidget>
#include "ui_chatForm.h"

class chatForm : public QWidget
{
	Q_OBJECT

public:
	chatForm(QWidget *parent = Q_NULLPTR);
	~chatForm();

private:
	Ui::chatForm ui;
	// void closeEvent(QCloseEvent* event);
};
