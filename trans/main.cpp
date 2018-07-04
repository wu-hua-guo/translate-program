#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);
    QWidget* parent = new QWidget;
    MainWindow *w=new MainWindow(parent);
    w->show();
    w->activateWindow();
    int ret=a.exec();

    delete parent;


    return ret;
}
