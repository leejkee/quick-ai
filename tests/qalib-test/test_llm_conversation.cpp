//
// Test LLMConversation
//
#include <catch2/catch_all.hpp>
#include <llm/LLMConversation.h>

using namespace QA::Core;

TEST_CASE("LLMConversation initializes with current time", "[LLMConversation]")
{
    LLMConversation conversation;

    REQUIRE(!conversation.getStartTimeStr().isEmpty());
    REQUIRE(conversation.getMessageSize() == 0);
}

TEST_CASE("LLMConversation initializes with system prompt", "[LLMConversation]")
{
    QString systemPrompt = "You are a helpful assistant.";
    LLMConversation conversation(systemPrompt);

    REQUIRE(conversation.getMessageSize() == 1);
    REQUIRE(conversation.getSystemPrompt().role == "system");
    REQUIRE(conversation.getSystemPrompt().content == systemPrompt);
}

TEST_CASE("LLMConversation pushMessage adds message to history", "[LLMConversation]")
{
    LLMConversation conversation;

    Message msg;
    msg.role = "user";
    msg.content = "Hello, AI!";

    conversation.pushMessage(msg);

    REQUIRE(conversation.getMessageSize() == 1);
    REQUIRE(conversation.at(0).role == "user");
    REQUIRE(conversation.at(0).content == "Hello, AI!");
}

TEST_CASE("LLMConversation at returns correct message", "[LLMConversation]")
{
    LLMConversation conversation;

    Message msg1{"user", "Message 1"};
    Message msg2{"assistant", "Message 2"};

    conversation.pushMessage(msg1);
    conversation.pushMessage(msg2);

    REQUIRE(conversation.at(0).content == "Message 1");
    REQUIRE(conversation.at(1).content == "Message 2");
}

TEST_CASE("LLMConversation operator[] works like at", "[LLMConversation]")
{
    LLMConversation conversation;

    Message msg{"user", "Test message"};
    conversation.pushMessage(msg);

    REQUIRE(conversation[0].content == "Test message");
}

TEST_CASE("LLMConversation getMessages returns all messages", "[LLMConversation]")
{
    LLMConversation conversation("System prompt");

    conversation.pushMessage({"user", "Hello"});
    conversation.pushMessage({"assistant", "Hi there!"});

    const auto& messages = conversation.getMessages();
    REQUIRE(messages.size() == 3);
    REQUIRE(messages[0].role == "system");
    REQUIRE(messages[1].role == "user");
    REQUIRE(messages[2].role == "assistant");
}

TEST_CASE("LLMConversation clearHistory removes all messages", "[LLMConversation]")
{
    LLMConversation conversation("System");
    conversation.pushMessage({"user", "Hello"});

    REQUIRE(conversation.getMessageSize() == 2);

    conversation.clearHistory();

    REQUIRE(conversation.getMessageSize() == 0);
}

TEST_CASE("LLMConversation getContext returns limited context window", "[LLMConversation]")
{
    // MAX_CONTEXT_WINDOW = 3, so limit is 3*2+2 = 8 messages
    // Plus system prompt = up to 9 messages in context
    LLMConversation conversation("System prompt");

    // Add 10 messages
    for (int i = 0; i < 10; ++i)
    {
        conversation.pushMessage({i % 2 == 0 ? "user" : "assistant", QString("Message %1").arg(i)});
    }

    auto context = conversation.getContext();

    // Should have system prompt + last 6 messages (MAX_CONTEXT_WINDOW * 2) + the lastest question
    REQUIRE(context.size() == 8);
    REQUIRE(context.first().role == "system");
}

TEST_CASE("LLMConversation getContext returns all messages when under limit", "[LLMConversation]")
{
    LLMConversation conversation("System");

    // Add 5 messages (under the limit)
    for (int i = 0; i < 5; ++i)
    {
        conversation.pushMessage({"user", QString("Message %1").arg(i)});
    }

    auto context = conversation.getContext();

    // Should return all messages
    REQUIRE(context.size() == 6); // system + 5 messages
}

TEST_CASE("LLMConversation handles empty conversation", "[LLMConversation]")
{
    LLMConversation conversation;

    REQUIRE(conversation.getMessageSize() == 0);
    REQUIRE(conversation.getMessages().isEmpty());
    REQUIRE(conversation.getContext().isEmpty());
}
