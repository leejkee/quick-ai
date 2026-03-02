//
// Created by 31305 on 2025/10/18.
//
#include <llm/LLMConversation.h>

namespace QA::Core
{

LLMConversation::LLMConversation(const QString& systemPrompt)
{
    m_startTime = QDateTime::currentDateTime();

    if (!systemPrompt.isEmpty())
    {
        const Message sysPrompt{"system", systemPrompt};
        pushMessage(sysPrompt);
    }
}


Message LLMConversation::at(const qsizetype index) const
{
    return m_messagesHistory.at(index);
}

void LLMConversation::pushMessage(const Message& message)
{
    m_messagesHistory.append(message);
}

Message LLMConversation::getSystemPrompt() const
{
    return m_messagesHistory.first();
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

void LLMConversation::clearHistory()
{
    m_messagesHistory.clear();
}

} // namespace QA::Core
