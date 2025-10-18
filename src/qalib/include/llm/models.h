//
// Created by 31305 on 2025/10/16.
//
#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace QA::Core
{
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

struct NoStreamingResponsePacket
{
    struct Choice
    {
        std::string finish_reason;
        int index;
        Message message;
    };

    struct Usage
    {
        int completion_tokens;
        int prompt_tokens;
        int prompt_cache_hit_tokens;
        int prompt_cache_miss_tokens;
        int total_tokens;
    };

    std::string id;
    std::vector<Choice> choices;
    int created;
    std::string model;
    Usage usage;
};

struct StreamingResponsePacket
{
    struct Choices
    {
        struct Delta
        {
            std::string content;
            std::string reasoning_content;
            std::string role;
        };

        Delta delta;
        std::string finish_reason;
        int index;
    };

    std::string id;
    Choices choices;
    int created;
    std::string model;
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NoStreamingResponsePacket::Usage
                                   , completion_tokens
                                   , prompt_cache_hit_tokens
                                   , prompt_cache_miss_tokens
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StreamingResponsePacket::Choices::Delta
                                   , content
                                   , reasoning_content
                                   , role)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StreamingResponsePacket::Choices
                                   , delta
                                   , finish_reason
                                   , index)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StreamingResponsePacket
                                   , id
                                   , choices
                                   , created
                                   , model)
}
