//
// Created by 31305 on 2025/11/11.
//
#include <ChatService/ChatService.h>
#include <MessageListModel/MessageListModel.h>
#include <QDebug>
namespace QA::Service
{

ChatService::ChatService(QObject* parent) : QObject(parent) {}

void ChatService::init()
{
    const char* api_key_env = std::getenv("DEEPSEEK_API_KEY");
    if (!api_key_env)
    {
        qDebug() << "DEEPSEEK_API_KEY not set";
        return;
    }
    const std::string api_key{api_key_env};
    m_client = std::make_unique<Core::LLMClient>("deepseek-chat", api_key);
    m_conversation = std::make_unique<Core::LLMConversation>();
}

void ChatService::postPrompt(const MessageBody& message)
{
    const Core::Message userMessage{message.role.toStdString(),
                                    message.content.toStdString()};
    m_conversation->push_message(userMessage);
    if (const auto r =
                m_client->no_streaming_request(m_conversation->get_context()))
    {
        const auto& assistant_message = r.value();
        const Core::Message responseMsg = assistant_message.message;
        const int tokens = assistant_message.usage.total_tokens;
        const MessageBody responseMessageBody{
                QString::fromStdString(responseMsg.role),
                QString::fromStdString(responseMsg.content),
                tokens};
        m_conversation->push_message(responseMsg);
        Q_EMIT signalLLMResponse(responseMessageBody);
    }
}


} // namespace QA::Service
