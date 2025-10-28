//
// Created by 31305 on 2025/10/18.
//
#pragma once
#include <iomanip>
#include <sstream>
#include <vector>
#include "models.h"

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

    MessageTime() : year(2000), month(1), day(1), hour(0), minute(0), second(0)
    {
    }

    MessageTime(const int year,
                const int month,
                const int day,
                const int hour,
                const int minute,
                const int second)
        : year(year), month(month), day(day), hour(hour), minute(minute),
          second(second)
    {
    }

    [[nodiscard]] std::string to_string() const
    {
        std::stringstream ss;
        ss << year << "-" << std::setfill('0') << std::setw(2) << month << "-"
           << std::setfill('0') << std::setw(2) << day << " "
           << std::setfill('0') << std::setw(2) << hour << ":"
           << std::setfill('0') << std::setw(2) << minute << ":"
           << std::setfill('0') << std::setw(2) << second;
        return ss.str();
    }
};

class LLMConversation
{
public:
    explicit LLMConversation(const Message& system_prompt);
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
} // namespace QA::Core
