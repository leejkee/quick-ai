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

    // bool streaming_request(std::string_view prompt
    //                        , std::function<void(
    //                            const StreamingResponsePacket& chunk)>&
    //                        chunk_received);

    static void print_response(
        const NoStreamingResponsePacket& response);

private:
    std::string m_api_key;
    const std::string m_url{"https://api.deepseek.com/chat/completions"};
};
}
