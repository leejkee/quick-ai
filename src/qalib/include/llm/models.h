//
// Created by 31305 on 2025/10/16.
//
#pragma once
#include <nlohmann/json.hpp>
#include <string>
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

struct Model
{
    inline static const std::string deepseek_chat = "deepseek-chat";
    inline static const std::string deepseek_reasoner = "deepseek-reasoner";
};

struct Message
{
    std::string role;
    std::string content;
};

struct RequestPacket
{
    std::vector<Message> messages;
    std::string model;
    int frequency_penalty = 0;
    int max_tokens = 4096;
    int presence_penalty = 0;

    bool stream = false;
    double temperature = 0;
    double top_p = 1;
};

struct Usage
{
    int completion_tokens;
    int prompt_tokens;
    int total_tokens;
};

struct NoStreamingResponsePacket
{
    struct Choice
    {
        std::string finish_reason;
        int index;
        Message message;
    };

    std::string id;
    std::vector<Choice> choices;
    int created;
    std::string model;
    Usage usage;
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Message, role, content)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RequestPacket
                                   , messages
                                   , model
                                   , frequency_penalty
                                   , max_tokens
                                   , presence_penalty
                                   , stream
                                   , temperature
                                   , top_p)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Usage
                                   , completion_tokens
                                   , prompt_tokens
                                   , total_tokens)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NoStreamingResponsePacket::Choice
                                   , finish_reason
                                   , index
                                   , message)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NoStreamingResponsePacket
                                   , id
                                   , choices
                                   , created
                                   , model
                                   , usage)
}
