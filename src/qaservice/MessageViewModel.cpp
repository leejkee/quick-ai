//
// Created by 31305 on 2025/11/9.
//
#include <MessageViewModel/MessageViewModel.h>
#include <QDebug>

namespace QA::Service
{
MessageViewModel::MessageViewModel(MessageListModel* model,
                             const ChatService* service,
                             QObject* parent)
    : QObject(parent), m_messageListModel(model)
{
    connect(this,
            &MessageViewModel::signalSendPrompt,
            service,
            &ChatService::postPrompt);
    connect(service,
            &ChatService::signalLLMResponse,
            this,
            &MessageViewModel::handleLLMResponse);
}

void MessageViewModel::handleUserRequest(const QString& prompt)
{
    if (prompt.isEmpty())
    {
        qDebug() << "prompt is empty";
        return;
    }
    MessageBody promptMsg;
    promptMsg.role = "user";
    promptMsg.content = prompt;
    // TODO
    // 多行代码作为prompt输入，对话框显示不完整
//     std::unique_ptr<LLMClientBase>
//
// LLMClientFactory::create_llm_client(const ModelMeta& model_meta)
//
//     {
//
//         switch (inferProvider(model_meta.model))
//
//         {
//
//         case ProviderType::OpenAI:
//
//         default:
//
//         {
//
//             return std::make_unique<OpenAIClient>(model_meta);
//
//         }
//
//         }
//
//     }
//
//     然后调用m_client = Core::LLMClientFactory::create_llm_client(model_meta);
//
//     这里从函数内部的智能指针对象到m_client，是发生了所有权转移吗
    m_messageListModel->pushMessage(promptMsg);
    Q_EMIT signalSendPrompt(promptMsg);
}

void MessageViewModel::handleLLMResponse(const MessageBody& message)
{
    m_messageListModel->pushMessage(message);
    if (message.role == "assistant" && message.tokens.has_value())
    {
        setStatusMessage(QString("Model: %1\nTotal tokens: %2").arg("QWEN").arg(message.tokens.value()));
    }
}

void MessageViewModel::handleClearSession()
{
    m_messageListModel.clear();
}

void MessageViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message)
    {
        m_statusMessage = message;
        Q_EMIT signalStatusMessageChanged();
    }
}


} // namespace QA::Service
