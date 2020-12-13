#include "Protocol.h"
#include "MessageBox.h"
#include "ChatSocket.h"
#include "EntranceForm.h"
#include "ChatForm.h"

EntranceForm::EntranceForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.JoinRoom_2, SIGNAL(clicked()), this, SLOT(btnJoinHandler()));
	connect(ui.CreateNewRoom_2, SIGNAL(clicked()), this, SLOT(btnCreateHandler()));
}

EntranceForm::~EntranceForm()
{

}

void EntranceForm::btnJoinHandler() 
{
	QString nickName = ui.MyJoinNickname_4->text();
	QString roomId = ui.ChatID_4->text();
	QString password = ui.ExistRoomPassword_4->text();

	if (nickName.isEmpty() || roomId.isEmpty()) {
		msgBoxWarning("The field for nickname, room number cannot be empty");

	}  else {

		/* Before joining the room */
		int res = packetSend(PACKET_TYPE_JOIN_ROOM " " + 
			roomId.toStdString() + " " + 
			password.toStdString() + " " + 
			nickName.toStdString());

		if (res == -1) {
			/* SOCKET_ERROR */
			msgBoxCritical("Netowrk error");
			return;
		}

		string packet = recvMsg();
		int type = universalVerifier(packet, false);

		msgBoxInfo(QString::fromStdString(packet));

		toChatForm(20201212);
	}
}

void EntranceForm::btnCreateHandler()
{
	QString nickName = ui.MyNickname_4->text();
	QString password = ui.NewRoomPassword_4->text();

	if (nickName.isEmpty() || password.isEmpty()) {
		msgBoxWarning("The field for nickname and password cannot be empty");
	} else {
		/* Before creating the room */
		if (nickName.length() == 0) {
			msgBoxWarning("The field for nickname cannot be empty");
			return;
		}

		int res = packetSend(PACKET_TYPE_CREATE_ROOM " " +
			password.toStdString() + " " + nickName.toStdString());

		if (res == -1) {
			msgBoxCritical("Netowrk error");
			return;
		}

		toChatForm(20201212);
	}
}

void EntranceForm::toChatForm(int roomID) {
	room = new ChatForm;
	room->setWindowTitle("Chat Room: " + QString::number(roomID, 10));
	room->show();
	hide();
}