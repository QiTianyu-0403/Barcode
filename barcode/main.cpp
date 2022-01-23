#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("条形码识别工具   by：BIT-齐天宇");
    w.show();
    
    return a.exec();
}

