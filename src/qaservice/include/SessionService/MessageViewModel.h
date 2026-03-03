//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <QObject>
#include <QPointer>

namespace QA::Service
{
class MessageModel;
class MessageViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* messageListModel READ getMessageListModel CONSTANT)
    Q_PROPERTY(QString statusMessage READ getStatusMessage NOTIFY
                       signalStatusMessageChanged)

public:
    explicit MessageViewModel(MessageModel* model, QObject* parent = nullptr);

    Q_INVOKABLE [[nodiscard]] QString getStatusMessage() const noexcept
    {
        return m_statusMessage;
    }

    Q_INVOKABLE [[nodiscard]] QObject* getMessageListModel() const;

Q_SIGNALS:
    void signalStatusMessageChanged();

    void signalMessageAdded(const QString&);

    void signalClearConversation();

public Q_SLOTS:
    void handleUserRequest(const QString& prompt);

    void handleClearSession();

private:
    QString m_statusMessage;

    void setStatusMessage(const QString& message);

    QPointer<MessageModel> m_model;
};

} // namespace QA::Service
