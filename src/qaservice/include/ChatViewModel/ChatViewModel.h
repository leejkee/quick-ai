//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <ChatService/ChatService.h>
#include <MessageListModel/MessageListModel.h>
#include <QObject>
#include <QPointer>

namespace QA::Service
{
class ChatViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* messageListModel READ getMessageListModel CONSTANT)
    Q_PROPERTY(QString statusMessage READ getStatusMessage NOTIFY
                       signalStatusMessageChanged)

public:
    explicit ChatViewModel(MessageListModel* model,
                           const ChatService* service,
                           QObject* parent = nullptr);

    Q_INVOKABLE [[nodiscard]] QString getStatusMessage() const
    {
        return m_statusMessage;
    }

    Q_INVOKABLE [[nodiscard]] QObject* getMessageListModel() const
    {
        return m_messageListModel.data();
    };

Q_SIGNALS:
    void signalStatusMessageChanged();

    void signalSendPrompt(const QA::Service::MessageBody& prompt);

public Q_SLOTS:
    void handleUserRequest(const QString& prompt);

    void handleClearSession();

    void handleLLMResponse(const QA::Service::MessageBody& message);

private:
    QPointer<MessageListModel> m_messageListModel;

    QString m_statusMessage;

    void setStatusMessage(const QString& message);
};

} // namespace QA::Service
