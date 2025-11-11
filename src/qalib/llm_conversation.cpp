//
// Created by 31305 on 2025/10/18.
//
#include <chrono>
#include <ctime>
#include <llm/llm_conversation.h>

namespace QA::Core
{
LLMConversation::LLMConversation(const Message& system_prompt)
{
    std::time_t time_t_value = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
    std::tm tm_struct = {};
#if defined(_WIN32)
    localtime_s(&tm_struct, &time_t_value);
#else
    localtime_r(&time_t_value, &tm_struct);
#endif
    m_start_time = {tm_struct.tm_year + 1900,
                    tm_struct.tm_mon + 1,
                    tm_struct.tm_mday,
                    tm_struct.tm_hour,
                    tm_struct.tm_min,
                    tm_struct.tm_sec};
    if (!system_prompt.content.empty())
    {
        m_messages_history.push_back(system_prompt);
    }
}

LLMConversation::LLMConversation() : LLMConversation({{}, {}}) {}


void LLMConversation::push_message(const Message& message)
{
    m_messages_history.push_back(message);
}

const std::vector<Message>& LLMConversation::get_messages() const
{
    return m_messages_history;
}

MessageTime LLMConversation::get_start_time() const { return m_start_time; }
} // namespace QA::Core
