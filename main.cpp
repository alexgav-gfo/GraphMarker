#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QIcon appIcon(":/icons/app.png");
    MainWindow w;
    w.setMinimumSize(1024, 750);
    w.setWindowIcon(appIcon);
    w.showMaximized();

    return a.exec();
}
