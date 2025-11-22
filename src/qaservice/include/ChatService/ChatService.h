//
// Created by 31305 on 2025/11/11.
//
#pragma once
#include <MessageListModel/MessageListModel.h>
#include <QObject>
#include <llm/llm_client_factory.h>
#include <llm/llm_conversation.h>
#include <memory>

namespace QA::Service
{
class ChatService final : public QObject
{
    Q_OBJECT
public:
    explicit ChatService(QObject* parent = nullptr);

    void init();

Q_SIGNALS:
    void signalLLMResponse(const MessageBody& message);

public Q_SLOTS:
    void postPrompt(const MessageBody& message);

private:
    std::unique_ptr<Core::LLMConversation> m_conversation;
    std::unique_ptr<Core::LLMClientBase> m_client;
};
} // namespace QA::Service
