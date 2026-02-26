//
// Created by 31305 on 2025/10/31.
//
#include <QApplication>
#include "AppManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QA::App::AppManager appManager;
    appManager.initApp();
    return QApplication::exec();
}