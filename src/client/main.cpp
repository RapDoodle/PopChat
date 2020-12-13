#include "ConnectForm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectForm w;
    w.show();
    return a.exec();
}
