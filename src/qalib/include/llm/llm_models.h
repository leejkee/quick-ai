//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <nlohmann/json.hpp>
#include <string>
namespace QA::Core
{
struct Message
{
    std::string role;
    std::string content;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Message, role, content);
};

struct ModelParams
{
    int frequency_penalty = 0;
    int max_tokens = 4096;
    int presence_penalty = 0;
    double temperature = 0;
    double top_p = 1;
    bool stream = false;
};

struct ChatResponseBody
{
    Message message;
    int total_tokens;
};

struct ModelMeta
{
    std::string model;
    std::string api_key;
    std::string url;
};

} // namespace QA::Core
