#include "entranceForm.h"
#include "chatForm.h"
#include<qmessagebox.h>

entranceForm::entranceForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.JoinRoom_2, SIGNAL(clicked()), this, SLOT(checkJoin()));
	connect(ui.CreateNewRoom_2, SIGNAL(clicked()), this, SLOT(checkCreate()));
}

entranceForm::~entranceForm()
{
}

void entranceForm::checkJoin() {
	QString nickName = ui.MyJoinNickname_4->text();
	QString chat = ui.ChatID_4->text();
	QString password = ui.ExistRoomPassword_4->text();

	if (nickName.isEmpty() || chat.isEmpty()|| password.isEmpty())
		QMessageBox::warning(this, "Invalid Input", "Please input all the required infornation!");
	else {



		//密码是否正确 房间是否存在 以及 昵称有无重复
		tochatForm(20201212);
	}
}

void entranceForm::checkCreate() {

	QString nickName = ui.MyNickname_4->text();
	QString password = ui.NewRoomPassword_4->text();

	if (nickName.isEmpty() || password.isEmpty())
		QMessageBox::warning(this, "Invalid Input", "Please input all the needed infornation!");
	else {

		//获得房间ID

		tochatForm(20201212);
	}
}

void entranceForm::tochatForm(int roomID){
	room = new chatForm;
	room->setWindowTitle("Chat Room: " + QString::number(roomID, 10));
	room->show();
	hide();
}