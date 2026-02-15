//
// Created by 31305 on 2026/2/10.
//
#pragma once
#include <QObject>
#include <UserSettings/SettingsRepository.h>
#include "ChatService.h"

namespace QA::Service
{

class SessionService : public QObject
{
    Q_OBJECT
public:
    explicit SessionService(ChatService* chatService,
                            SettingsRepository* settingsRepo,
                            QObject* parent = nullptr);

    void chatNoStreaming();

    MessageModel* getMessageModel() const
    {
        return m_messageModel;
    };

    Q_SIGNALS:
    void signalLLMResponse(const QString& response);
public Q_SLOTS:

private:
    QPointer<ChatService> m_chatService;
    QPointer<SettingsRepository> m_settings;
    MessageModel* m_messageModel;

    QString m_selectedProviderId;
    QString m_selectedModel;
    Core::ModelParams m_modelParams;
};

} // namespace QA::Service
