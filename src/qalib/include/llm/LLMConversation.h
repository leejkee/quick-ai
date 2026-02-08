//
// Created by 31305 on 2025/10/18.
//
#pragma once
#include <QDateTime>
#include <QList>
#include <QString>
#include <llm/LLMModels.h>

namespace QA::Core
{

class LLMConversation
{
public:
    explicit LLMConversation(const Message& systemPrompt = {});

    void pushMessage(const Message& message);

    [[nodiscard]] const QList<Message>& getMessages() const;

    [[nodiscard]] QList<Message> getContext() const;

    [[nodiscard]] QString getStartTimeStr() const;

    [[nodiscard]] QDateTime getStartTime() const;

private:
    static constexpr int MAX_CONTEXT_WINDOW = 3;
    QDateTime m_startTime;
    QList<Message> m_messagesHistory;
};

} // namespace QA::Core
