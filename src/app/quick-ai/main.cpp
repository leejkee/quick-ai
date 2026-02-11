//
// Created by 31305 on 2025/10/31.
//
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <ChatViewModel/ChatViewModel.h>
#include <ChatService/ChatService.h>
#include <ChatService/ParamsConfig.h>
#include <ChatService/SelectionConfig.h>
#include <qalog/Log.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // auto* paramsConfigManager = new QA::Service::ParamsConfig(QA::Service::ConfigModelParams(), &app);
    // auto* selectionConfigManager = new QA::Service::SelectionConfig(&app);
    auto* service = new QA::Service::ChatService(paramsConfigManager);
    service->init();
    const auto model = new QA::Service::MessageListModel(&app);
    auto* chatViewModel = new QA::Service::ChatViewModel(model, service, &app);

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [](QObject *obj, const QUrl &) {
            if (!obj) {
                QA_LOG_WARN("Failed to load QML from module. Exiting.");
                QCoreApplication::exit(-1);
            }
        }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("chatViewModel", chatViewModel);
    // engine.rootContext()->setContextProperty("paramsConfigManager", paramsConfigManager);
    // engine.rootContext()->setContextProperty("selectionConfigManager", selectionConfigManager);

    engine.loadFromModule("qaui.sessionwindow", "Main");
    // engine.loadFromModule("qaui.settingswindow", "MainView");
    return app.exec();
}