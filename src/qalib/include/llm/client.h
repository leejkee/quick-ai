//
// Created by 31305 on 2025/10/17.
//
#pragma once
#include <optional>
#include "models.h"

namespace QA::Core
{
class LLMClient
{
public:
    explicit LLMClient(std::string_view api_key);

    std::optional<NoStreamingResponsePacket> no_streaming_request(
        const std::vector<Message>& messages);

    bool streaming_request(const std::vector<Message>& messages
                           , const std::function<void(
                               const std::string_view& content_chunk)>&
                           content_chunk_received
                           , const std::function<void(
                               const std::string_view& finish_reason
                               , const Usage& usage_info)>&
                           completion_chunk_received);

    static void print_response(
        const NoStreamingResponsePacket& response);

private:
    std::string m_api_key;
    const std::string m_url{"https://api.deepseek.com/chat/completions"};
};
}
