//
// Created by 31305 on 2026/2/10.
//
#pragma once
#include "ChatService.h"
#include <UserSettings/UserSettings.h>
#include <QObject>

namespace QA::Service
{

class SessionService : public QObject
{
    Q_OBJECT
public:
    explicit SessionService(ChatService* chatService, QObject *parent = nullptr);

    void chatNoStreaming();

private:
    QPointer<ChatService> m_chatService;
    Provider m_selectedProvider;
    QString m_selectedModel;
    Core::ModelParams m_modelParams;
};

}
