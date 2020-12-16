#include "ConnectForm.h"

ConnectForm::ConnectForm(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Connect");
    ui.setupUi(this);
    connect(ui.historyButton, SIGNAL(clicked()),this, SLOT(toHistory()));
    connect(ui.connectButton, SIGNAL(clicked()), this, SLOT(btnLoginHandler()));
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

void ConnectForm::btnLoginHandler() 
{
    QString host = ui.hostField->text();
    QString port = ui.portField->text();
    if (host.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Host and port number cannot be empty.");

    } else {
        /* Connect to the server */
        if (clientStartup(host.toStdString().c_str(), atoi(port.toStdString().c_str())) < 0)
            return;

        /*  */
        ef = new EntranceForm;
        ef->hostStrGlobal = host.toStdString();
        ef->setWindowTitle("Enter a Chat Room");
        ef->show();
        hide();
    }
}


void ConnectForm::toHistory() 
{


}