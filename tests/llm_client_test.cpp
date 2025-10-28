//
// Created by 31305 on 2025/10/22.
//
#include <catch2/catch_test_macros.hpp>
#include <llm/llm_client.h>


class MockAdapter final : public QA::Core::LLMAdapterInterface {
public:
    bool streaming_was_called = false;
    std::string received_prompt;

    std::optional<QA::Core::CommonChatResponse> no_streaming_request(
        const std::vector<QA::Core::Message>& messages) override
    {
        QA::Core::CommonChatResponse response;
        response.message = {"assistant", "Mocked non-streaming response"};
        response.usage.total_tokens = 5;
        return response;
    }

    std::optional<QA::Core::CommonChatResponse> streaming_request(
        const std::vector<QA::Core::Message>& messages,
        const QA::Core::content_callback& on_content) override
    {
        streaming_was_called = true;
        received_prompt = messages.back().content;

        on_content("Hello, ");
        on_content("mocked ");
        on_content("world!");

        QA::Core::CommonChatResponse response;
        response.message = {"assistant", "Hello, mocked world!"}; // 完整的回复
        response.usage.total_tokens = 3;
        response.finish_reason = "stop";
        return response;
    }
};


TEST_CASE("LLMClient streaming request uses adapter and callback", "[llmclient]") {
    auto mock_adapter = std::make_shared<MockAdapter>();
    QA::Core::LLMClient client(mock_adapter);

    std::vector<QA::Core::Message> messages = {{"user", "test prompt"}};
    std::string accumulated_reply;

    auto result = client.streaming_request(messages,
        [&](const std::string_view chunk){
            accumulated_reply += chunk;
        });

    CHECK(mock_adapter->streaming_was_called == true);
    CHECK(mock_adapter->received_prompt == "test prompt");
    CHECK(accumulated_reply == "Hello, mocked world!");

    REQUIRE(result.has_value());
    CHECK(result->message.content == "Hello, mocked world!");
    CHECK(result->finish_reason == "stop");
}

TEST_CASE("LLMClient no-streaming request uses adapter", "[llmclient]") {
    auto mock_adapter = std::make_shared<MockAdapter>();
    QA::Core::LLMClient client(mock_adapter);

    auto result = client.no_streaming_request({{"user", "test"}});

    REQUIRE(result.has_value());
    CHECK(result->message.content == "Mocked non-streaming response");
}