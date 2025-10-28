//
// Created by 31305 on 2025/10/17.
//
#pragma once
#include "llm_adapter_interface.h"
#include <optional>

namespace QA::Core
{

struct NoStreamingResponsePacket
{
    struct Choice
    {
        std::string finish_reason;
        Message message;
    };

    std::string id;
    std::vector<Choice> choices;
    std::string model;
    Usage usage;
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RequestPacket
                                   , messages
                                   , model
                                   , frequency_penalty
                                   , max_tokens
                                   , presence_penalty
                                   , stream
                                   , temperature
                                   , top_p)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NoStreamingResponsePacket::Choice
                                   , finish_reason
                                   , message)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NoStreamingResponsePacket
                                   , id
                                   , choices
                                   , model
                                   , usage)

class DeepSeekAdapter final : public LLMAdapterInterface
{
public:
    explicit DeepSeekAdapter(std::string_view model, std::string_view api_key);

    explicit DeepSeekAdapter(std::string_view model, std::string_view api_key, std::string_view url);

    std::optional<CommonChatResponse> no_streaming_request(
        const std::vector<Message>& messages) override;

    std::optional<CommonChatResponse> streaming_request(
        const std::vector<Message>& messages
        , const content_callback& content_call) override;

private:
    std::string m_api_key;
    std::string m_model;
    std::string m_url{"https://api.deepseek.com/chat/completions"};
};
}
