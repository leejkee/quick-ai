//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <ChatService/ChatService.h>
#include <MessageListModel/MessageListModel.h>
#include <QPointer>
#include <QObject>

namespace QA::Service
{
class ChatViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* messageListModel READ messageListModel CONSTANT)
public:
    explicit ChatViewModel(MessageListModel* model, const ChatService* service, QObject* parent = nullptr);

Q_SIGNALS:
    void signalSendPrompt(const QA::Service::MessageBody& prompt);

public Q_SLOTS:
    void handleUserRequest(const QString& prompt);

    void handleLLMResponse(const QA::Service::MessageBody& message);

    Q_INVOKABLE [[nodiscard]] QObject* messageListModel() const;

private:
    QPointer<MessageListModel> m_messageListModel;
};

} // namespace QA::Service
