#include "chatRoom.h"
#include<qmessagebox.h>

chatRoom::chatRoom(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

chatRoom::~chatRoom()
{
}

void chatRoom::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("Leave"), QString(tr("Do you want to leave the chat room?")), 
        QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::No)
    {
        event->ignore(); 
    }
    else if (button == QMessageBox::Yes)
    {
        event->accept(); 
    }

    //TODO: 在退出窗口之前，实现希望做的操作  
    
}
