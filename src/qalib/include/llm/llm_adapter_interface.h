//
// Created by 31305 on 2025/10/21.
//
#pragma once

#include <optional>
#include "models.h"

namespace QA::Core
{
using content_callback = std::function<void(std::string_view content_chunk)>;

struct CommonChatResponse
{
    std::string id;
    Message message;
    Usage usage;
    std::string finish_reason;
    std::string model_name;
};

class LLMAdapterInterface
{
public:
    virtual ~LLMAdapterInterface();

    virtual std::optional<CommonChatResponse>
    no_streaming_request(const std::vector<Message>& messages) = 0;

    virtual std::optional<CommonChatResponse>
    streaming_request(const std::vector<Message>& messages,
                      const content_callback& content_call) = 0;
};
} // namespace QA::Core
