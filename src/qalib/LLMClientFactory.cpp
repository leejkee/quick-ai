//
// Created by 31305 on 2025/11/21.
//
#include <llm/LLMClientFactory.h>
#include <llm/OpenAIClient.h>

namespace QA::Core
{
enum class ProviderType
{
    OpenAI,
    Gemini,
    Unknown
};

static ProviderType inferProvider(QStringView name)
{
    if (name.contains(QStringLiteral("gpt"), Qt::CaseInsensitive) ||
        name.contains(QStringLiteral("deepseek"), Qt::CaseInsensitive) ||
        name.contains(QStringLiteral("qwen"), Qt::CaseInsensitive))
    {
        return ProviderType::OpenAI;
    }

    if (name.contains(QStringLiteral("gemini"), Qt::CaseInsensitive))
    {
        return ProviderType::Gemini;
    }

    return ProviderType::Unknown;
}

LLMClientBase* LLMClientFactory::createLLMClient(const PostBody& postBody,
                                                 QObject* parent)
{
    switch (inferProvider(postBody.model))
    {
    case ProviderType::OpenAI:
    default:
        {
            return new OpenAIClient(postBody, parent);
        }
    }
}


} // namespace QA::Core
