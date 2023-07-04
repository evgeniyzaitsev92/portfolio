#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Resources/CheckYourself.png"));
    Widget w;
    w.show();
    return a.exec();
}
