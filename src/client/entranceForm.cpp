#include "Protocol.h"
#include "utils.h"
#include "MessageBox.h"
#include "ChatSocket.h"
#include "EntranceForm.h"
#include "ChatForm.h"
#include <QCloseEvent>

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
	QString nickname = ui.joinNickname->text();
	QString roomId = ui.ChatID_4->text();
	QString password = ui.joinRoomPassword->text();

	if (nickname.isEmpty() || roomId.isEmpty()) {
		msgBoxWarning("The field for nickname, room number cannot be empty");

	}  else {

		/* Before joining the room */
		int res = packetSend(PACKET_TYPE_JOIN_ROOM " " + 
			roomId.toStdString() + " " + 
			password.toStdString() + " " + 
			nickname.toStdString());

		if (res == -1) {
			/* SOCKET_ERROR */
			msgBoxCritical("Netowrk error");
			return;
		}

		string packet = recvMsg();
		int type = universalVerifier(&packet, false);
		string roomId, nickname;

		if (type == safeToInt(PACKET_TYPE_ADMITTED)) {
			roomId = nextParam(&packet);
			nickname = nextParam(&packet);
			toChatForm(QString::fromStdString(roomId), QString::fromStdString(nickname));
			return;

		} else if (type == safeToInt(PACKET_TYPE_FAILED)) {
			msgBoxCritical(QString::fromStdString(packet));
			return;

		} else {
			msgBoxCritical("Unknown error");
			return;

		}
	}
}

void EntranceForm::btnCreateHandler()
{
	QString nickname = ui.createNickname->text();
	QString password = ui.createRoomPassword->text();

	if (nickname.isEmpty()) {
		msgBoxWarning("The field for nickname cannot be empty");
	} else {
		/* Before creating the room */
		if (nickname.length() == 0) {
			msgBoxWarning("The field for nickname cannot be empty");
			return;
		}

		int res = packetSend(PACKET_TYPE_CREATE_ROOM " " +
			password.toStdString() + " " + nickname.toStdString());

		string packet = recvMsg();
		int type = universalVerifier(&packet, false);
		string roomId, nickname;

		if (type == safeToInt(PACKET_TYPE_ADMITTED)) {
			roomId = nextParam(&packet);
			nickname = nextParam(&packet);
			toChatForm(QString::fromStdString(roomId), QString::fromStdString(nickname));
		} else if (type == safeToInt(PACKET_TYPE_FAILED)) {
			msgBoxCritical(QString::fromStdString(packet));
			return;
		} else {
			msgBoxCritical("Unknown error");
			return;
		}

	}
}

void EntranceForm::toChatForm(QString roomID, QString nickname) 
{
	room = new ChatForm(Q_NULLPTR, nickname, roomID, QString::fromStdString(hostStrGlobal));
	room->setWindowTitle("Pop Chat [Chat Room: " + roomID + "]");
	room->show();
	hide();

}