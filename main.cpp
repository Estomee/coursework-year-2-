#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    a.setWindowIcon(QIcon(":/Images/Add_Icon.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
