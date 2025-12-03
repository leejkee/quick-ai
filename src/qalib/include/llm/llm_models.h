//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>
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

    bool operator==(const ModelParams& rhs) const
    {
        return std::tie(frequency_penalty,
                        max_tokens,
                        presence_penalty,
                        temperature,
                        top_p,
                        stream) ==
                std::tie(rhs.frequency_penalty,
                         rhs.max_tokens,
                         rhs.presence_penalty,
                         rhs.temperature,
                         rhs.top_p,
                         rhs.stream);
    }

    bool operator!=(const ModelParams& rhs) const
    {
        return !(*this == rhs);
    }
};

struct ChatResponseBody
{
    Message message;
    int total_tokens = 0;
};

struct ModelMeta
{
    std::string model;
    std::string api_key;
    std::string url;

    bool operator==(const ModelMeta& rhs) const
    {
        return std::tie(model,
                api_key,
                url) ==
            std::tie(rhs.model,
                rhs.api_key);
    }

    bool operator!=(const ModelMeta& rhs) const
    {
        return !(*this == rhs);
    }
};

} // namespace QA::Core
