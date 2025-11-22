//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <cpr/cpr.h>
#include <llm/llm_models.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

namespace QA::Core
{
class LLMClientBase
{
public:
    virtual ~LLMClientBase();

    virtual std::optional<ChatResponseBody>
    no_streaming_chat(const ModelParams& model_params,
                      const std::vector<Message>& user_prompt);

protected:
    [[nodiscard]] virtual nlohmann::json
    get_request_body(const ModelParams& params,
                     const std::vector<Message>& user_prompt) const = 0;

    [[nodiscard]] virtual cpr::Header get_request_header() const = 0;

    [[nodiscard]] virtual std::optional<ChatResponseBody>
    parse_response(std::string_view response_text) const = 0;

    [[nodiscard]] virtual std::string get_model_url() const = 0;
};

} // namespace QA::Core
