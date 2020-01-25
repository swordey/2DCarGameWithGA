#include "ui/widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w(1024, 720);
    w.show();
    return a.exec();
}
