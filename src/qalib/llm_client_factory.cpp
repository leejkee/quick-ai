//
// Created by 31305 on 2025/11/21.
//
#include <llm/llm_client_factory.h>
#include <llm/openai_client.h>

namespace QA::Core
{
enum class ProviderType
{
    OpenAI,
    Gemini,
    Unknown
};

static ProviderType inferProvider(std::string_view name)
{
    if (name.find("gpt") != std::string_view::npos ||
        name.find("deepseek") != std::string_view::npos ||
        name.find("qwen") != std::string_view::npos)
    {
        return ProviderType::OpenAI;
    }
    if (name.find("gemini") != std::string_view::npos)
    {
        return ProviderType::Gemini;
    }
    return ProviderType::Unknown;
}

std::unique_ptr<LLMClientBase>
LLMClientFactory::create_llm_client(const ModelMeta& model_meta)
{
    switch (inferProvider(model_meta.model))
    {
    case ProviderType::OpenAI:
    default:
        {
            return std::make_unique<OpenAIClient>(model_meta);
        }
    }
}


} // namespace QA::Core
