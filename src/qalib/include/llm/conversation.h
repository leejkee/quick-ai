//
// Created by 31305 on 2025/10/18.
//
#pragma once
#include "models.h"
#include <vector>

namespace QA::Core
{
class Conversation
{
public:
    explicit Conversation(const Message& system_prompt);
    void push_message(const Message& message);
    const std::vector<Message>& get_messages() const;
private:
    std::vector<Message> m_messages_history;
};

}
