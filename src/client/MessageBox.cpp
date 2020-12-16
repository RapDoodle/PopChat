#include "MessageBox.h"

void msgBoxInfo(QString s)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Infomation");
    msgBox.setText(s);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void msgBoxWarning(QString s)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Message");
    msgBox.setText(s);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void msgBoxCritical(QString s)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(s);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}