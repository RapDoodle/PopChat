#include "hostConnsct.h"
#include "roomEnter.h"
#include <qmessagebox.h>

hostConnsct::hostConnsct(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Connect");
    ui.setupUi(this);
    connect(ui.historyButton, SIGNAL(clicked()),this, SLOT(toHistory()));
    connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(toRoomEnter()));
}

void hostConnsct::toRoomEnter() 
{
    QString host = ui.hostfield->text();
    QString port = ui.portfield->text();
    if (host.isEmpty() || port.isEmpty())
        QMessageBox::warning(this, "Invalid Input", "Please input proper host and port number");
    else {
        //这里还要有个检查是否连接成功的 function


        re = new roomEnter;
        re->setWindowTitle("Enter A Chat Room");
        re->show();
        hide();
    }
}


void hostConnsct::toHistory() 
{


}