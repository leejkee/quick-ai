//
// Created by 31305 on 2025/11/21.
//
#include <iostream>
#include <llm/llm_client_base.h>

namespace QA::Core
{

LLMClientBase::~LLMClientBase() = default;

std::optional<ChatResponseBody>
LLMClientBase::no_streaming_chat(const ModelParams& model_params,
                                 const std::vector<Message>& user_prompt)
{
    if (user_prompt.empty())
    {
        return std::nullopt;
    }

    auto post_body =
            cpr::Body{get_request_body(model_params, user_prompt).dump()};

    auto post_header = get_request_header();

    const cpr::Response r =
            cpr::Post(cpr::Url{get_model_url()}, post_header, post_body);

    if (r.status_code != 200)
    {
        std::cerr << "Request failed with status code: " << r.status_code
                  << std::endl;
        std::cerr << "Error message: " << r.text << std::endl;
        return std::nullopt;
    }

    return parse_response(r.text);
}
} // namespace QA::Core
