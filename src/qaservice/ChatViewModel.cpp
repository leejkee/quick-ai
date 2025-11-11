//
// Created by 31305 on 2025/11/9.
//
#include <ChatViewModel/ChatViewModel.h>
#include <QDebug>

namespace QA::Service
{
ChatViewModel::ChatViewModel(const ChatService* service, QObject* parent)
    : QObject(parent)
{
    m_messageListModel = new MessageListModel(this);
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
    Core::Message promptMsg;
    promptMsg.role = "user";
    promptMsg.content = prompt.toStdString();

    m_messageListModel->pushMessage(promptMsg);
    Q_EMIT signalSendPrompt(promptMsg);
}

QObject* ChatViewModel::messageListModel() const { return m_messageListModel; }

void ChatViewModel::handleLLMResponse(const Core::Message& message)
{
    m_messageListModel->pushMessage(message);
}

} // namespace QA::Service
