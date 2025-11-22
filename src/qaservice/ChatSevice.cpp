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
    const char* api_key_env = std::getenv("QWEN_API_KEY");
    if (!api_key_env)
    {
        qDebug() << "DEEPSEEK_API_KEY not set";
        return;
    }
    const std::string api_key{api_key_env};
    const Core::ModelMeta model_meta{"qwen3-max", api_key, "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions"};
    m_client = Core::LLMClientFactory::create_llm_client(model_meta);
    m_conversation = std::make_unique<Core::LLMConversation>();
}

void ChatService::postPrompt(const MessageBody& message)
{
    const Core::Message userMessage{message.role.toStdString(),
                                    message.content.toStdString()};
    m_conversation->push_message(userMessage);
    if (const auto r =
                m_client->no_streaming_chat(Core::ModelParams(), m_conversation->get_context()))
    {
        const auto& [message, total_tokens] = r.value();
        const int tokens = total_tokens;
        const MessageBody responseMessageBody{
                QString::fromStdString(message.role),
                QString::fromStdString(message.content),
                tokens};
        m_conversation->push_message(message);
        Q_EMIT signalLLMResponse(responseMessageBody);
    }
}


} // namespace QA::Service
