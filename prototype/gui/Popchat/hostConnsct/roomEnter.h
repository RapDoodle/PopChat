#pragma once

#include <QWidget>
#include "ui_roomEnter.h"
#include "chatRoom.h"

class roomEnter : public QWidget
{
	Q_OBJECT

public:
	roomEnter(QWidget *parent = Q_NULLPTR);
	~roomEnter();

private slots:
	void checkCreate();
	void checkJoin();

private:
	Ui::roomEnter ui;
	chatRoom* room;
	void toChatRoom(int roomID );
};
