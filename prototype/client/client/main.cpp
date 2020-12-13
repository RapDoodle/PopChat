#include <QtWidgets/QApplication>

#include "ClientForm.h"

#ifndef __INTELLISENSE__
#include <QMutex>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientForm w;
    w.show();
    return a.exec();
}
