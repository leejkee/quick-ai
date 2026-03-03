//
// Created by 31305 on 2025/10/31.
//
#include <QGuiApplication>
#include "AppManager.h"
#include <IPCManager/IPCManager.h>
#include <QLocalSocket>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // Prevent the application from quitting when the last window is closed, since we want it to run in the system tray.
    QGuiApplication::setQuitOnLastWindowClosed(false);
    QLocalSocket socket;
    socket.connectToServer(QA::App::IPCManager::getDefaultServerName());
    if (socket.waitForConnected(500)) {
        QString cmd = (app.arguments().contains("--toggle")) ? "--toggle" : "default";
        socket.write(cmd.toUtf8());
        socket.flush();
        socket.waitForBytesWritten();
        socket.waitForDisconnected(200);
        return 0;
    }

    QA::App::AppManager appManager;
    return QGuiApplication::exec();
}