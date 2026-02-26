//
// Created by 31305 on 2025/11/9.
//
#include <SessionService/MessageModel.h>
#include <SessionService/MessageViewModel.h>
#include <SessionService/SessionService.h>


namespace QA::Service
{
MessageViewModel::MessageViewModel(MessageModel* model, QObject* parent)
    : QObject(parent), m_model(model)
{
}

void MessageViewModel::handleUserRequest(const QString& prompt)
{
    if (prompt.isEmpty())
    {
        return;
    }
    Q_EMIT signalMessageAdded(prompt);
    // Core::Message promptMsg;
    // promptMsg.role = "user";
    // promptMsg.content = prompt;
    // // TODO
    // // 多行代码作为prompt输入，对话框显示不完整
    //
    // m_service->pushMessage(promptMsg);
    // m_service->chatNoStreaming();
}

void MessageViewModel::handleClearSession()
{
    Q_EMIT signalClearConversation();
}

void MessageViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message)
    {
        m_statusMessage = message;
        Q_EMIT signalStatusMessageChanged();
    }
}

QObject* MessageViewModel::getMessageListModel() const { return m_model; };

} // namespace QA::Service
