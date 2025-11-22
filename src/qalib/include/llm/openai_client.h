//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <llm/llm_client_base.h>

namespace QA::Core
{
class OpenAIClient final : public LLMClientBase
{
public:
    explicit OpenAIClient(const ModelMeta& model_meta);

protected:
    nlohmann::json
    get_request_body(const ModelParams& params,
                     const std::vector<Message>& user_prompt) const override;

    cpr::Header get_request_header() const override;

    std::optional<ChatResponseBody>
    parse_response(std::string_view response_text) const override;

    std::string get_model_url() const override;

private:
    std::string m_model;
    std::string m_api_key;
    std::string m_model_url;
};

} // namespace QA::Core
