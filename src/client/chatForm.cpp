#include "chatForm.h"
#include <qmessagebox.h>

chatForm::chatForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

chatForm::~chatForm()
{

}

//void chatForm::closeEvent(QCloseEvent* event)
//{
//	QMessageBox::StandardButton button;
//	button = QMessageBox::question(this, tr("Leave"), QString(tr("Do you want to leave the chat room?")), 
//		QMessageBox::Yes | QMessageBox::No);
//	if (button == QMessageBox::No)
//	{
//		event->ignore();
//	}
//	else if (button == QMessageBox::Yes)
//	{
//		event->accept(); 
//	}
//
//	/* Code below: before exiting the window */
//	
//}
