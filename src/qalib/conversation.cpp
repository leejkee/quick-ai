//
// Created by 31305 on 2025/10/18.
//
#include <llm/conversation.h>
namespace QA::Core
{

Conversation::Conversation(const Message& system_prompt)
{
    m_messages_history.push_back(system_prompt);
}


void Conversation::push_message(const Message& message)
{
    m_messages_history.push_back(message);
}

const std::vector<Message>& Conversation::get_messages() const
{
    return m_messages_history;
}


}