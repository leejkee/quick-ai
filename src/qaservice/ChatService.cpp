//
// Created by 31305 on 2025/11/11.
//
#include <llm/LLMClientFactory.h>
#include <ChatService/ChatService.h>
#include <MessageListModel/MessageListModel.h>
#include <qalog/Log.h>
namespace QA::Service
{

ChatService::ChatService(ParamsConfig* params, QObject* parent) : QObject(parent), m_params(params) {}

void ChatService::init()
{
    const char* APIKeyENV = std::getenv("QWEN_API_KEY");
    if (!APIKeyENV)
    {
        QA_LOG_DEBUG("DEEPSEEK_API_KEY not set");
        return;
    }
    const QString APIKey{APIKeyENV};
    const Core::PostBody postBody{"qwen3-max", APIKey, "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions"};
    m_client = Core::LLMClientFactory::createLLMClient(postBody);
    m_conversation = std::make_unique<Core::LLMConversation>();
}

void ChatService::postPrompt(const MessageBody& message)
{
    const Core::Message userMessage{message.role,
                                    message.content};
    m_conversation->pushMessage(userMessage);
    if (const auto r =
                m_client->noStreamingChat(m_params->getParams(), m_conversation->getContext()))
    {
        QA_LOG_DEBUG(QString("Temperature: %1").arg(m_params->getParams().temperature, 0, 'f', 1));
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
