#pragma once

#include <QWidget>
#include "ui_chatRoom.h"
#include <QCloseEvent>  

class chatRoom : public QWidget
{
	Q_OBJECT

public:
	chatRoom(QWidget *parent = Q_NULLPTR);
	~chatRoom();

private:
	Ui::chatRoom ui;

protected:
	void closeEvent(QCloseEvent* event);
};

