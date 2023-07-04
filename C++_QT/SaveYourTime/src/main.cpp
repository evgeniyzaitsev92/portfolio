#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // always 125% scaling
    if (qgetenv("QT_FONT_DPI").isEmpty()) {
        qputenv("QT_FONT_DPI", "120");
    }

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("://Other.ico"));
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
