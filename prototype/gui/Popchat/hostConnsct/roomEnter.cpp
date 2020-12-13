#include "roomEnter.h"
#include "chatRoom.h"
#include<qmessagebox.h>

roomEnter::roomEnter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.JoinRoom_2, SIGNAL(clicked()), this, SLOT(checkJoin()));
	connect(ui.CreateNewRoom_2, SIGNAL(clicked()), this, SLOT(checkCreate()));
}

roomEnter::~roomEnter()
{
}

void roomEnter::checkJoin() {
	QString nickName = ui.MyJoinNickname_4->text();
	QString chat = ui.ChatID_4->text();
	QString password = ui.ExistRoomPassword_4->text();

	if (nickName.isEmpty() || chat.isEmpty()|| password.isEmpty())
		QMessageBox::warning(this, "Invalid Input", "Please input all the required infornation!");
	else {



		//密码是否正确 房间是否存在 以及 昵称有无重复
		toChatRoom(20201212);
	}
}

void roomEnter::checkCreate() {

	QString nickName = ui.MyNickname_4->text();
	QString password = ui.NewRoomPassword_4->text();

	if (nickName.isEmpty() || password.isEmpty())
		QMessageBox::warning(this, "Invalid Input", "Please input all the needed infornation!");
	else {

		//获得房间ID

		toChatRoom(20201212);
	}
}

void roomEnter::toChatRoom(int roomID){
	room = new chatRoom;
	room->setWindowTitle("Chat Room: " + QString::number(roomID, 10));
	room->show();
	hide();
}