//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <ChatService/ChatService.h>
#include <MessageListModel/MessageListModel.h>
#include <QObject>

namespace QA::Service
{
class ChatViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* messageListModel READ messageListModel CONSTANT)
public:
    explicit ChatViewModel(const ChatService* service, QObject* parent = nullptr);

Q_SIGNALS:
    void signalSendPrompt(const QA::Core::Message& prompt);

public Q_SLOTS:
    void handleUserRequest(const QString& prompt);

    void handleLLMResponse(const QA::Core::Message& message);

    Q_INVOKABLE [[nodiscard]] QObject* messageListModel() const;

private:
    MessageListModel* m_messageListModel;
};

} // namespace QA::Service
