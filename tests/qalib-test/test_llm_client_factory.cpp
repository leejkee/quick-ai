//
// Test LLMClientFactory
//
#include <catch2/catch_all.hpp>
#include <llm/LLMClientFactory.h>
#include <llm/OpenAIClient.h>

using namespace QA::Core;

TEST_CASE("LLMClientFactory creates OpenAIClient for GPT models", "[LLMClientFactory]")
{
    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-api-key";
    initData.url = "https://api.openai.com/v1/chat/completions";

    auto client = LLMClientFactory::createLLMClient(initData);

    REQUIRE(client != nullptr);
    REQUIRE(dynamic_cast<OpenAIClient*>(client.get()) != nullptr);
}

TEST_CASE("LLMClientFactory creates OpenAIClient for DeepSeek models", "[LLMClientFactory]")
{
    ModelInitData initData;
    initData.model = "deepseek-chat";
    initData.apiKey = "test-api-key";
    initData.url = "https://api.deepseek.com/v1/chat/completions";

    auto client = LLMClientFactory::createLLMClient(initData);

    REQUIRE(client != nullptr);
    REQUIRE(dynamic_cast<OpenAIClient*>(client.get()) != nullptr);
}

TEST_CASE("LLMClientFactory creates OpenAIClient for Qwen models", "[LLMClientFactory]")
{
    ModelInitData initData;
    initData.model = "qwen-turbo";
    initData.apiKey = "test-api-key";
    initData.url = "https://dashscope.aliyuncs.com/v1/chat/completions";

    auto client = LLMClientFactory::createLLMClient(initData);

    REQUIRE(client != nullptr);
    REQUIRE(dynamic_cast<OpenAIClient*>(client.get()) != nullptr);
}

TEST_CASE("LLMClientFactory handles case-insensitive model names", "[LLMClientFactory]")
{
    ModelInitData initData;
    initData.model = "GPT-4-TURBO";
    initData.apiKey = "test-api-key";
    initData.url = "https://api.openai.com/v1/chat/completions";

    auto client = LLMClientFactory::createLLMClient(initData);

    REQUIRE(client != nullptr);
    REQUIRE(dynamic_cast<OpenAIClient*>(client.get()) != nullptr);
}

TEST_CASE("LLMClientFactory creates default client for unknown models", "[LLMClientFactory]")
{
    ModelInitData initData;
    initData.model = "unknown-model";
    initData.apiKey = "test-api-key";
    initData.url = "https://example.com/api";

    auto client = LLMClientFactory::createLLMClient(initData);

    // Unknown models should default to OpenAIClient
    REQUIRE(client != nullptr);
    REQUIRE(dynamic_cast<OpenAIClient*>(client.get()) != nullptr);
}

TEST_CASE("LLMClientFactory preserves model initialization data", "[LLMClientFactory]")
{
    ModelInitData initData;
    initData.model = "gpt-3.5-turbo";
    initData.apiKey = "sk-test123456";
    initData.url = "https://custom.api.com/v1/chat";

    auto client = LLMClientFactory::createLLMClient(initData);

    REQUIRE(client != nullptr);
}
