#include "connectForm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connectForm w;
    w.show();
    return a.exec();
}
