//
// Test OpenAIClient with mock server
//
#include <catch2/catch_all.hpp>
#include <llm/OpenAIClient.h>
#include <llm/LLMModels.h>
#include <QCoreApplication>
#include <llm_http_server.h>

using namespace QA::Core;
using namespace QA::Test;

// Helper to create QCoreApplication for tests
class QtTestApp
{
public:
    QtTestApp()
    {
        if (!QCoreApplication::instance())
        {
            static int argc = 1;
            static char* argv[] = {const_cast<char*>("test")};
            app = std::make_unique<QCoreApplication>(argc, argv);
        }
    }

private:
    std::unique_ptr<QCoreApplication> app;
};

TEST_CASE("OpenAIClient sends correct request format", "[OpenAIClient]")
{
    QtTestApp qtApp;

    // Start mock server
    LLMHttpServer server("127.0.0.1", 18080);
    bool requestReceived = false;
    std::string receivedBody;

    server.on_post("/v1/chat/completions", [&](const httplib::Request& req, httplib::Response& res) {
        requestReceived = true;
        receivedBody = req.body;

        // Return mock OpenAI response
        res.set_content(R"({
            "id": "chatcmpl-test",
            "object": "chat.completion",
            "created": 1234567890,
            "model": "gpt-4",
            "choices": [{
                "index": 0,
                "message": {
                    "role": "assistant",
                    "content": "Hello! How can I help you today?"
                },
                "finish_reason": "stop"
            }],
            "usage": {
                "prompt_tokens": 10,
                "completion_tokens": 20,
                "total_tokens": 30
            }
        })", "application/json");
    });

    REQUIRE(server.start());

    // Create client
    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-api-key";
    initData.url = QString::fromStdString(server.get_url() + "/v1/chat/completions");

    OpenAIClient client(initData);

    // Prepare request
    ModelParams params;
    params.temperature = 0.7;
    params.max_tokens = 100;

    QList<Message> messages;
    messages.append(Message{"user", "Hello!"});

    // Send request
    auto response = client.noStreamingChat(params, messages);

    // Verify request was received
    REQUIRE(requestReceived);
    REQUIRE(!receivedBody.empty());

    // Parse request body to verify format
    json requestJson = json::parse(receivedBody);
    REQUIRE(requestJson["model"] == "gpt-4");
    REQUIRE(requestJson["temperature"] == 0.7);
    REQUIRE(requestJson["maxTokens"] == 100);
    REQUIRE(requestJson["messages"].is_array());
    REQUIRE(requestJson["messages"].size() == 1);
    REQUIRE(requestJson["messages"][0]["role"] == "user");
    REQUIRE(requestJson["messages"][0]["content"] == "Hello!");

    // Verify response
    REQUIRE(response.has_value());
    REQUIRE(response->role == "assistant");
    REQUIRE(response->content == "Hello! How can I help you today?");
    REQUIRE(response->totalTokens == 30);

    server.stop();
}

TEST_CASE("OpenAIClient handles multiple messages", "[OpenAIClient]")
{
    QtTestApp qtApp;

    LLMHttpServer server("127.0.0.1", 18081);

    server.on_post("/v1/chat/completions", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content(R"({
            "id": "chatcmpl-test",
            "object": "chat.completion",
            "created": 1234567890,
            "model": "gpt-4",
            "choices": [{
                "index": 0,
                "message": {
                    "role": "assistant",
                    "content": "I understand your conversation."
                },
                "finish_reason": "stop"
            }],
            "usage": {
                "prompt_tokens": 50,
                "completion_tokens": 10,
                "total_tokens": 60
            }
        })", "application/json");
    });

    REQUIRE(server.start());

    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-key";
    initData.url = QString::fromStdString(server.get_url() + "/v1/chat/completions");

    OpenAIClient client(initData);

    ModelParams params;
    QList<Message> messages;
    messages.append(Message{"system", "You are helpful."});
    messages.append(Message{"user", "Hello"});
    messages.append(Message{"assistant", "Hi!"});
    messages.append(Message{"user", "How are you?"});

    auto response = client.noStreamingChat(params, messages);

    REQUIRE(response.has_value());
    REQUIRE(response->content == "I understand your conversation.");

    server.stop();
}

TEST_CASE("OpenAIClient handles API error", "[OpenAIClient]")
{
    QtTestApp qtApp;

    LLMHttpServer server("127.0.0.1", 18082);

    server.on_post("/v1/chat/completions", [&](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        res.set_content(R"({
            "error": {
                "message": "Invalid API key",
                "type": "authentication_error"
            }
        })", "application/json");
    });

    REQUIRE(server.start());

    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "invalid-key";
    initData.url = QString::fromStdString(server.get_url() + "/v1/chat/completions");

    OpenAIClient client(initData);

    ModelParams params;
    QList<Message> messages;
    messages.append(Message{"user", "Hello"});

    auto response = client.noStreamingChat(params, messages);

    // Should return nullopt on error
    REQUIRE(!response.has_value());

    server.stop();
}

TEST_CASE("OpenAIClient handles empty messages", "[OpenAIClient]")
{
    QtTestApp qtApp;

    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-key";
    initData.url = "http://localhost:8080/v1/chat/completions";

    OpenAIClient client(initData);

    ModelParams params;
    QList<Message> messages;

    auto response = client.noStreamingChat(params, messages);

    // Should return nullopt for empty messages
    REQUIRE(!response.has_value());
}

TEST_CASE("OpenAIClient handles server connection error", "[OpenAIClient]")
{
    QtTestApp qtApp;

    // Use a port where no server is running
    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-key";
    initData.url = "http://127.0.0.1:59999/v1/chat/completions";

    OpenAIClient client(initData);

    ModelParams params;
    QList<Message> messages;
    messages.append(Message{"user", "Hello"});

    auto response = client.noStreamingChat(params, messages);

    // Should return nullopt on connection error
    REQUIRE(!response.has_value());
}

TEST_CASE("OpenAIClient handles missing choices in response", "[OpenAIClient]")
{
    QtTestApp qtApp;

    LLMHttpServer server("127.0.0.1", 18083);

    server.on_post("/v1/chat/completions", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content(R"({
            "id": "chatcmpl-test",
            "object": "chat.completion",
            "created": 1234567890,
            "model": "gpt-4",
            "choices": []
        })", "application/json");
    });

    REQUIRE(server.start());

    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-key";
    initData.url = QString::fromStdString(server.get_url() + "/v1/chat/completions");

    OpenAIClient client(initData);

    ModelParams params;
    QList<Message> messages;
    messages.append(Message{"user", "Hello"});

    auto response = client.noStreamingChat(params, messages);

    // Should return nullopt when choices are empty
    REQUIRE(!response.has_value());

    server.stop();
}

TEST_CASE("OpenAIClient handles response without usage info", "[OpenAIClient]")
{
    QtTestApp qtApp;

    LLMHttpServer server("127.0.0.1", 18084);

    server.on_post("/v1/chat/completions", [&](const httplib::Request& req, httplib::Response& res) {
        res.set_content(R"({
            "id": "chatcmpl-test",
            "object": "chat.completion",
            "created": 1234567890,
            "model": "gpt-4",
            "choices": [{
                "index": 0,
                "message": {
                    "role": "assistant",
                    "content": "Response without usage"
                },
                "finish_reason": "stop"
            }]
        })", "application/json");
    });

    REQUIRE(server.start());

    ModelInitData initData;
    initData.model = "gpt-4";
    initData.apiKey = "test-key";
    initData.url = QString::fromStdString(server.get_url() + "/v1/chat/completions");

    OpenAIClient client(initData);

    ModelParams params;
    QList<Message> messages;
    messages.append(Message{"user", "Hello"});

    auto response = client.noStreamingChat(params, messages);

    REQUIRE(response.has_value());
    REQUIRE(response->content == "Response without usage");
    REQUIRE(response->totalTokens == 0); // Should default to 0

    server.stop();
}
