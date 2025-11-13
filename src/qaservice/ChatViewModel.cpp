//
// Created by 31305 on 2025/11/9.
//
#include <ChatViewModel/ChatViewModel.h>
#include <QDebug>

namespace QA::Service
{
ChatViewModel::ChatViewModel(MessageListModel* model,
                             const ChatService* service,
                             QObject* parent)
    : QObject(parent), m_messageListModel(model)
{
    connect(this,
            &ChatViewModel::signalSendPrompt,
            service,
            &ChatService::postPrompt);
    connect(service,
            &ChatService::signalLLMResponse,
            this,
            &ChatViewModel::handleLLMResponse);
}

void ChatViewModel::handleUserRequest(const QString& prompt)
{
    if (prompt.isEmpty())
    {
        qDebug() << "prompt is empty";
        return;
    }
    MessageBody promptMsg;
    promptMsg.role = "user";
    promptMsg.content = prompt;
    m_messageListModel->pushMessage(promptMsg);
    Q_EMIT signalSendPrompt(promptMsg);
}

void ChatViewModel::handleLLMResponse(const MessageBody& message)
{
    m_messageListModel->pushMessage(message);
    if (message.role == "assistant" && message.tokens.has_value())
    {
        setStatusMessage(QString("Total tokens: %1").arg(message.tokens.value()));
    }
}

void ChatViewModel::handleClearSession()
{
    m_messageListModel.clear();
}

void ChatViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message)
    {
        m_statusMessage = message;
        Q_EMIT signalStatusMessageChanged();
    }
}


} // namespace QA::Service
