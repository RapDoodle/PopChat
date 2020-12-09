#include <QtWidgets/QApplication>

#include "ClientForm.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientForm w;
    w.show();
    return a.exec();
}
