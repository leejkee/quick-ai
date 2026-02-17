//
// Created by 31305 on 2026/2/10.
//
#pragma once
#include <QObject>
#include <llm/LLMClientBase.h>
#include <UserSettings/SettingsRepository.h>
#include <memory>
#include "llm/LLMConversation.h"
#include "MessageModel.h"

namespace QA::Service
{

class SessionService : public QObject
{
    Q_OBJECT
public:
    explicit SessionService(SettingsRepository* settingsRepo,
                            QObject* parent = nullptr);

    Core::ChatResponseBody chatNoStreaming();

    MessageModel* getMessageModel() const
    {
        return m_messageModel;
    };


    Q_SIGNALS:
    void signalLLMResponse(const QString& response);
public Q_SLOTS:

private:
    std::unique_ptr<Core::LLMClientBase> m_client;
    std::shared_ptr<Core::LLMConversation> m_conversation;

    QPointer<SettingsRepository> m_settings;
    MessageModel* m_messageModel;

    QString m_selectedProviderId;
    QString m_selectedModel;
    Core::ModelParams m_modelParams;
};

} // namespace QA::Service
