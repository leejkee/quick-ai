//
// Created by 31305 on 2025/11/9.
//
#include <QDebug>
#include <SessionService/MessageViewModel.h>
#include <SessionService/SessionService.h>


namespace QA::Service
{
MessageViewModel::MessageViewModel(SessionService* service, QObject* parent)
    : QObject(parent), m_service(service)
{
}

void MessageViewModel::handleUserRequest(const QString& prompt)
{
    if (prompt.isEmpty())
    {
        return;
    }
    Core::Message promptMsg;
    promptMsg.role = "user";
    promptMsg.content = prompt;
    // TODO
    // 多行代码作为prompt输入，对话框显示不完整

    m_service->pushMessage(promptMsg);
    m_service->chatNoStreaming();
}

void MessageViewModel::handleClearSession() { m_service->clearMessage(); }

void MessageViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message)
    {
        m_statusMessage = message;
        Q_EMIT signalStatusMessageChanged();
    }
}


} // namespace QA::Service
