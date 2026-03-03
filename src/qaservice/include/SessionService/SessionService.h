//
// Created by 31305 on 2026/2/10.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <QThreadPool>
#include <llm/LLMModels.h>

namespace QA::Service
{
class SettingsRepository;
class MessageModel;
class LLMRuntimeContext;

class SessionService final : public QObject
{
    Q_OBJECT
public:
    explicit SessionService(SettingsRepository* settingsRepo,
                            LLMRuntimeContext* runtimeContext,
                            MessageModel* messageModel,
                            QObject* parent = nullptr);

    void chatNoStreaming();

    [[nodiscard]] MessageModel* getMessageModel() const noexcept
    {
        return m_messageModel;
    };

    void pushMessage(const Core::Message& msg);

    void clearMessage();

public Q_SLOTS:
    void handleUserChat(const QString& prompt);

Q_SIGNALS:
    void signalLLMResponse(const QString& response);

private:
    QThreadPool m_threadPool;

    QPointer<SettingsRepository> m_settingsRepo;
    QPointer<LLMRuntimeContext> m_runtimeContext;
    QPointer<MessageModel> m_messageModel;
};

} // namespace QA::Service
