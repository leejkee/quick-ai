//
// Created by 31305 on 2025/10/22.
//
#include <llm/llmclient.h>
#include <llm/modeladapterfactory.h>

namespace QA::Core
{
LLMClient::LLMClient(const std::string_view model, const std::string_view api_key)
{
    m_adapter = ModelAdapterFactory::createModelAdapter(model, api_key);
}

std::optional<CommonChatResponse> LLMClient::no_streaming_request(
    const std::vector<Message>& messages)
{
    return m_adapter->no_streaming_request(messages);
}


std::optional<CommonChatResponse> LLMClient::streaming_request(
    std::vector<Message>& messages
    , const content_callback& content_call)
{
    return m_adapter->streaming_request(messages, content_call);
}
}
