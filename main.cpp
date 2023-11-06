#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

#ifdef __DEBUG
#include <QDebug>
#include <fstream>
#include <string>
#include <iostream>
#include "configmanager.h"
#include "json.hpp"
using namespace std;
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    w.setImage("resource/page_2.jpg");
    w.show();
    return a.exec();
}
