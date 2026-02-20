//
// Created by 31305 on 2025/10/31.
//
#include <QApplication>
#include "AppManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QA::App::AppManager appManager;
    appManager.initApp();
    return app.exec();
}