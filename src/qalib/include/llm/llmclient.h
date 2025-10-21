//
// Created by 31305 on 2025/10/22.
//
#pragma once
#include "llminterface.h"
#include <memory>
#include <string_view>

namespace QA::Core
{

class LLMClient
{
public:
    struct Model
    {
        inline static const std::string deepseek_chat = "deepseek-chat";
        inline static const std::string deepseek_reasoner = "deepseek-reasoner";
    };

    explicit LLMClient(std::string_view model, std::string_view api_key);

    std::optional<CommonChatResponse> no_streaming_request(const std::vector<Message>& messages);

    std::optional<CommonChatResponse> streaming_request(
        const std::vector<Message>& messages
        , const content_callback& content_call);


private:
    std::shared_ptr<LLMAdapterInterface> m_adapter;
};

}
