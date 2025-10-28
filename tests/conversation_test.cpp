//
// Created by 31305 on 2025/10/22.
//
#include <catch2/catch_test_macros.hpp>
#include <llm/llm_conversation.h>

TEST_CASE("Conversation history is managed correctly", "[conversation]")
{
    QA::Core::Message system_prompt{"system", "You are an assistant."};
    QA::Core::LLMConversation conversation(system_prompt);

    conversation.push_message({"user", "Hello"});
    conversation.push_message({"assistant", "Hi!"});

    const auto& history = conversation.get_messages();

    REQUIRE(history.size() == 3);

    CHECK(history[0].role == "system");
    CHECK(history[1].role == "user");
    CHECK(history[1].content == "Hello");
    CHECK(history[2].role == "assistant");
}

TEST_CASE("Conversation start time is set", "[conversation]")
{
    const QA::Core::LLMConversation conversation({"system", "test"});
    const auto start_time = conversation.get_start_time();
    REQUIRE(start_time.year > 2024);
}
