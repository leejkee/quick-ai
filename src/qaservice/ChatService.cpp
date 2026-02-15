//
// Created by 31305 on 2025/11/11.
//
#include <llm/LLMClientFactory.h>
#include <ChatService/ChatService.h>
#include <qalog/Log.h>
namespace QA::Service
{

ChatService::ChatService(QObject* parent) : QObject(parent){}

void ChatService::init()
{
    // const char* APIKeyENV = std::getenv("DEEPSEEK_API_KEY");
    // if (!APIKeyENV)
    // {
    //     QA_LOG_DEBUG("DEEPSEEK_API_KEY not set");
    //     return;
    // }
    const QString APIKey{"sk-1d00763b38184cfaafb5b3ea8cfd3b7e"};
    const Core::PostBody postBody{"deepseek-chat", APIKey, "https://api.deepseek.com/chat/completions"};
    m_client = Core::LLMClientFactory::createLLMClient(postBody);
    m_conversation = std::make_unique<Core::LLMConversation>();
}

void ChatService::postPrompt(const Core::ModelParams& params, const MessageBody& message)
{
    const Core::Message userMessage{message.role,
                                    message.content};
    m_conversation->pushMessage(userMessage);
    if (const auto r =
                m_client->noStreamingChat(params, m_conversation->getContext()))
    {
        QA_LOG_DEBUG(QString("Temperature: %1").arg(params.temperature, 0, 'f', 1));
        const auto& [message, total_tokens] = r.value();
        const int tokens = total_tokens;
        const MessageBody responseMessageBody{
                message.role,
                message.content,
                tokens};
        m_conversation->pushMessage(message);

        Q_EMIT signalLLMResponse(responseMessageBody);
    }
}


} // namespace QA::Service
