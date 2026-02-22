//
// Created by 31305 on 2025/10/18.
//
#pragma once
#include <QDateTime>
#include <QList>
#include <QString>
#include "LLMModels.h"

namespace QA::Core
{

class LLMConversation final : public QObject
{
public:
    explicit LLMConversation(const QString& systemPrompt = {}, QObject* parent = nullptr);

    void pushMessage(const Message& message);

    [[nodiscard]] Message at(qsizetype index) const;

    [[nodiscard]] const QList<Message>& getMessages() const
    {
        return m_messagesHistory;
    }

    [[nodiscard]] QList<Message> getContext() const;

    [[nodiscard]] QString getStartTimeStr() const
    {
        return m_startTime.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    }

    [[nodiscard]] QDateTime getStartTime() const { return m_startTime; }

    [[nodiscard]] qsizetype getMessageSize() const
    {
        return m_messagesHistory.size();
    }

    [[nodiscard]] Message getSystemPrompt() const;

    void clearHistory();

    Message operator[](const qsizetype index) const { return at(index); }

private:
    static constexpr int MAX_CONTEXT_WINDOW = 3;
    QDateTime m_startTime;
    QList<Message> m_messagesHistory;
};

} // namespace QA::Core
