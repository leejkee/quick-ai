//
// Created by 31305 on 2025/10/31.
//
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <ChatViewModel/ChatViewModel.h>
#include <ChatService/ChatService.h>
#include <QThread>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    auto* service = new QA::Service::ChatService();
    auto* serviceThread = new QThread(&app);
    service->moveToThread(serviceThread);
    QObject::connect(serviceThread, &QThread::started, service, &QA::Service::ChatService::init);
    QObject::connect(
            serviceThread, &QThread::finished, service, &QObject::deleteLater);
    const auto model = new QA::Service::MessageListModel(&app);
    auto* chatViewModel = new QA::Service::ChatViewModel(model, service, &app);

    serviceThread->start();
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [](QObject *obj, const QUrl &) {
            if (!obj) {
                qWarning() << "Failed to load QML from module. Exiting.";
                QCoreApplication::exit(-1);
            }
        }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("chatViewModel", chatViewModel);
    engine.loadFromModule("qaui", "Main");
    return app.exec();
}