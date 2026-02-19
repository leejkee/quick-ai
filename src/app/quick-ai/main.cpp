//
// Created by 31305 on 2025/10/31.
//
#include <QGuiApplication>
#include "Appmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QA::App::AppManager appManager;
    appManager.initApp();
    return app.exec();
}