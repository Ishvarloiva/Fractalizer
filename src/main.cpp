#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //Everything happens in MainWindow but Qt requires QApplication for some reasons
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
