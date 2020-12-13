#include "connectForm.h"
#include "entranceForm.h"
#include <qmessagebox.h>

connectForm::connectForm(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Connect");
    ui.setupUi(this);
    connect(ui.historyButton, SIGNAL(clicked()),this, SLOT(toHistory()));
    connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(btnLoginHandler()));
}

void connectForm::btnLoginHandler() 
{
    QString host = ui.hostField->text();
    QString port = ui.portField->text();
    if (host.isEmpty() || port.isEmpty())
        QMessageBox::warning(this, "Invalid Input", "Host and port number cannot be empty.");
    else {
        /* Connect to the server */

        /*  */
        ef = new entranceForm;
        ef->setWindowTitle("Enter a Chat Room");
        ef->show();
        hide();
    }
}


void connectForm::toHistory() 
{


}