//
// Created by 31305 on 2025/10/18.
//
#pragma once
#include "models.h"
#include <vector>
#include <sstream>
#include <iomanip>

namespace QA::Core
{
struct MessageTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    [[nodiscard]] std::string to_string() const
    {
        std::stringstream ss;
        ss << year << "-"
           << std::setfill('0') << std::setw(2) << month << "-"
           << std::setfill('0') << std::setw(2) << day << " "
           << std::setfill('0') << std::setw(2) << hour << ":"
           << std::setfill('0') << std::setw(2) << minute << ":"
           << std::setfill('0') << std::setw(2) << second;
        return ss.str();
    }
};

class Conversation
{
public:
    explicit Conversation(const Message& system_prompt);
    void push_message(const Message& message);
    [[nodiscard]] const std::vector<Message>& get_messages() const;

    [[nodiscard]] std::string get_start_time_str() const
    {
        return m_start_time.to_string();
    }

    [[nodiscard]] MessageTime get_start_time() const;

private:
    MessageTime m_start_time;
    std::vector<Message> m_messages_history;
};
}
