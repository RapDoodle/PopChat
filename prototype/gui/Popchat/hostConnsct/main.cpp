#include "hostConnsct.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    hostConnsct w;
    w.show();
    return a.exec();
}
