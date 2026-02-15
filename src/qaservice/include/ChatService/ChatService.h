//
// Created by 31305 on 2025/11/11.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <llm/LLMClientBase.h>
#include <llm/LLMConversation.h>
#include <memory>
#include "MessageModel.h"

namespace QA::Service
{
class ChatService final : public QObject
{
    Q_OBJECT
public:
    explicit ChatService(QObject* parent = nullptr);

    void init();

Q_SIGNALS:
    void signalLLMResponse(const QA::Service::MessageBody& message);

public Q_SLOTS:
    void postPrompt(const QA::Core::ModelParams& params, const QA::Service::MessageBody& message);

private:
    std::unique_ptr<Core::LLMConversation> m_conversation;
    std::unique_ptr<Core::LLMClientBase> m_client;
};
} // namespace QA::Service
