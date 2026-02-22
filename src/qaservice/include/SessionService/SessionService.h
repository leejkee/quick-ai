//
// Created by 31305 on 2026/2/10.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <llm/LLMClientBase.h>
#include <llm/LLMConversation.h>
#include <memory>

namespace QA::Service
{
class SettingsRepository;
class MessageModel;

class SessionService final : public QObject
{
    Q_OBJECT
public:
    explicit SessionService(SettingsRepository* settingsRepo,
                            QObject* parent = nullptr);

    Core::ChatResponseBody chatNoStreaming();

    [[nodiscard]] MessageModel* getMessageModel() const
    {
        return m_messageModel;
    };

    void pushMessage(const Core::Message& msg);

    void clearMessage();

    Q_SIGNALS:
    void signalLLMResponse(const QString& response);

    void signalConversationChanged();

private:
    Core::LLMClientBase* m_client;
    Core::LLMConversation* m_conversation;

    QPointer<SettingsRepository> m_settingsRepo;
    MessageModel* m_messageModel;

    QString m_selectedProviderId;
    QString m_selectedModel;
    Core::ModelParams m_modelParams;
};

} // namespace QA::Service
