//
// Created by 31305 on 2025/11/11.
//
#pragma once
#include <MessageListModel/MessageListModel.h>
#include <QObject>
#include <QPointer>
#include <llm/LLMClientBase.h>
#include <llm/LLMConversation.h>
#include <memory>
#include "ParamsConfig.h"

namespace QA::Service
{
class ChatService final : public QObject
{
    Q_OBJECT
public:
    explicit ChatService(ParamsConfig* params, QObject* parent = nullptr);

    void init();

Q_SIGNALS:
    void signalLLMResponse(const QA::Service::MessageBody& message);

public Q_SLOTS:
    void postPrompt(const QA::Service::MessageBody& message);

private:
    std::unique_ptr<Core::LLMConversation> m_conversation;
    std::unique_ptr<Core::LLMClientBase> m_client;
    QPointer<ParamsConfig> m_params;
};
} // namespace QA::Service
