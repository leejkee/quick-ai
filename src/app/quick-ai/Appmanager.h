//
// Created by 31305 on 2026/2/13.
//
#pragma once
#include <QObject>
#include <ChatService/SessionService.h>

namespace QA::App
{
class AppManager final: public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject* parent);
private:
    Service::SessionService* m_sessionService;
    Service::SettingsRepository* m_settingsRepo;
    Service::MessageModel* m_messageListModel;
};

}
