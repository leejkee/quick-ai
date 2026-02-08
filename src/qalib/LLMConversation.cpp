//
// Created by 31305 on 2025/10/18.
//
#include <llm/LLMConversation.h>

namespace QA::Core
{

LLMConversation::LLMConversation(const Message& systemPrompt)
{
    m_startTime = QDateTime::currentDateTime();

    if (!systemPrompt.content.isEmpty())
    {
        pushMessage(systemPrompt);
    }
}

void LLMConversation::pushMessage(const Message& message)
{
    m_messagesHistory.append(message);
}

const QList<Message>& LLMConversation::getMessages() const
{
    return m_messagesHistory;
}

QList<Message> LLMConversation::getContext() const
{
    constexpr int messageLimit = MAX_CONTEXT_WINDOW * 2 + 2;
    const auto size = m_messagesHistory.size();

    if (size <= messageLimit)
    {
        return m_messagesHistory;
    }

    QList<Message> context;
    context.reserve(MAX_CONTEXT_WINDOW * 2 + 1);

    if (!m_messagesHistory.isEmpty())
    {
        context.append(m_messagesHistory.first());
    }
    auto startIndex = size - (MAX_CONTEXT_WINDOW * 2) - 1;
    if (startIndex < 1)
    {
        startIndex = 1;
    }
    for (auto i = startIndex; i < size; ++i)
    {
        context.append(m_messagesHistory.at(i));
    }

    return context;
}

QString LLMConversation::getStartTimeStr() const
{
    return m_startTime.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
}

QDateTime LLMConversation::getStartTime() const { return m_startTime; }

} // namespace QA::Core
