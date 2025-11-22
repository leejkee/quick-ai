//
// Created by 31305 on 2025/11/21.
//
#include <iostream>
#include <llm/openai_client.h>

namespace QA::Core
{

OpenAIClient::OpenAIClient(const ModelMeta& model_meta)
    : m_model(model_meta.model), m_api_key(model_meta.api_key),
      m_model_url(model_meta.url)
{
}

nlohmann::json
OpenAIClient::get_request_body(const ModelParams& params,
                               const std::vector<Message>& user_prompt) const
{
    nlohmann::json request_json;
    request_json["model"] = m_model;
    request_json["messages"] = user_prompt;
    request_json["temperature"] = params.temperature;
    request_json["max_tokens"] = params.max_tokens;
    request_json["frequency_penalty"] = params.frequency_penalty;
    request_json["presence_penalty"] = params.presence_penalty;
    request_json["top_p"] = params.top_p;
    request_json["stream"] = params.stream;
    return request_json;
}

cpr::Header OpenAIClient::get_request_header() const
{
    return {{"Content-Type", "application/json"},
            {"Authorization", "Bearer " + m_api_key}};
}

std::optional<ChatResponseBody>
OpenAIClient::parse_response(std::string_view response_text) const
{
    try
    {
        const nlohmann::json response_json =
                nlohmann::json::parse(response_text);

        if (response_json.contains("error"))
        {
            std::cerr << "API Error: " << response_json["error"].dump()
                      << std::endl;
            return std::nullopt;
        }

        if (!response_json.contains("choices") ||
            response_json["choices"].empty())
        {
            std::cerr << "Parse Error: 'choices' is missing or empty."
                      << std::endl;
            return std::nullopt;
        }

        const std::string content =
                response_json["choices"][0]["message"]["content"]
                        .get<std::string>();
        int tokens = 0;
        if (response_json.contains("usage") &&
            response_json["usage"].contains("total_tokens"))
        {
            tokens = response_json["usage"]["total_tokens"].get<int>();
        }
        ChatResponseBody r{{"assistant", content}, tokens};

        return r;
    } catch (const std::exception& e)
    {
        std::cerr << "JSON Parse Exception: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::string OpenAIClient::get_model_url() const { return m_model_url; }

} // namespace QA::Core
