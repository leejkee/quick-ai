//
// Created by 31305 on 2025/10/23.
//
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <llm/deepseek_adapter.h>
#include <llm/models.h>
#include <llm_http_server/llm_http_server.h>
#include <numeric>
#include <string>
#include <vector>

namespace
{
using json = nlohmann::json;
const std::string MOCK_HOST = "localhost";
constexpr int MOCK_PORT = 8080;
const std::string MOCK_API_KEY = "sk-mock-deepseek-api-key-test";
const std::string MOCK_MODEL = "deepseek-chat";
const std::string MOCK_API_PATH = "/chat/completions";
const std::string MOCK_RESPONSE_CONTENT =
        "Hello! How can I assist you today? 😊";
const std::vector<std::string> MOCK_RESPONSE_CONTENT_VECTOR = {"Hello",
                                                               "!",
                                                               " 👋",
                                                               " How",
                                                               " can",
                                                               " I",
                                                               " help"
                                                               " you",
                                                               " today",
                                                               "?",
                                                               " Feel",
                                                               " free",
                                                               " to",
                                                               " ask",
                                                               " me",
                                                               " anything",
                                                               "—",
                                                               "I",
                                                               "'m",
                                                               " here",
                                                               " to",
                                                               " assist",
                                                               "!"};


void handle_mock_deepseek_streaming_completion(const httplib::Request& req,
                                               httplib::Response& res);
void handle_mock_deepseek_non_streaming_completion(const httplib::Request& req,
                                                   httplib::Response& res);
const auto MOCK_HANDLER =
        [](const httplib::Request& req, httplib::Response& res)
{
    if (req.get_header_value("Authorization") != "Bearer " + MOCK_API_KEY)
    {
        res.status = 401;
        res.set_content(R"({"error": "Invalid API key."})", "application/json");
        std::cerr << "[Mock Server] Received bad auth header." << std::endl;
        return;
    }

    bool is_streaming = false;
    try
    {
        if (const auto req_json = nlohmann::json::parse(req.body);
            req_json.contains("stream") && req_json["stream"].is_boolean())
        {
            is_streaming = req_json["stream"].get<bool>();
        }
    } catch (const nlohmann::json::parse_error& e)
    {
        res.status = 400;
        res.set_content(R"({"error": "Invalid JSON."})", "application/json");
        std::cerr << "[Mock Server] Failed to parse request JSON: " << e.what()
                  << std::endl;
        return;
    }

    if (is_streaming)
    {
        std::cout << "[Mock Server] Handling STREAMING request." << std::endl;
        handle_mock_deepseek_streaming_completion(req, res);
    }
    else
    {
        std::cout << "[Mock Server] Handling NO-STREAMING request."
                  << std::endl;
        handle_mock_deepseek_non_streaming_completion(req, res);
    }
};

void handle_mock_deepseek_streaming_completion(const httplib::Request& req,
                                               httplib::Response& res)
{

    res.set_content_provider(
            "text/event-stream",
            [](const size_t offset, httplib::DataSink& sink)
            {
                if (offset > 0)
                {
                    sink.done();
                    return true;
                }

                auto create_base_chunk = []() -> json
                {
                    return {{"id", "00321d2f-08e2-46ae-91e7-2fc66660e4b4"},
                            {"object", "chat.completion.chunk"},
                            {"created", 1761623546},
                            {"model", "deepseek-chat"},
                            {"system_fingerprint",
                             "fp_ffc7281d48_prod0820_fp8_kvcache"}};
                };

                size_t index = 0;
                std::vector<json> chunks;

                {
                    json j = create_base_chunk();
                    j["choices"] = {{{"index", 0},
                                     {"delta",
                                      {{"role", "assistant"}, {"content", ""}}},
                                     {"logprobs", nullptr},
                                     {"finish_reason", nullptr}}};
                    chunks.push_back(std::move(j));
                }

                for (const auto& content_token : MOCK_RESPONSE_CONTENT_VECTOR)
                {
                    json j = create_base_chunk();
                    j["choices"] = {{{"index", 0},
                                     {"delta", {{"content", content_token}}},
                                     {"logprobs", nullptr},
                                     {"finish_reason", nullptr}}};
                    chunks.push_back(std::move(j));
                }

                {
                    json j = create_base_chunk();
                    j["choices"] = {{{"index", 0},
                                     {"delta", {{"content", ""}}},
                                     {"logprobs", nullptr},
                                     {"finish_reason", "stop"}}};
                    j["usage"] = {
                            {"prompt_tokens", 10},
                            {"completion_tokens", 24},
                            {"total_tokens", 34},
                            {"prompt_tokens_details", {{"cached_tokens", 0}}},
                            {"prompt_cache_hit_tokens", 0},
                            {"prompt_cache_miss_tokens", 10}};
                    chunks.push_back(std::move(j));
                }

                for (const auto& chunk_json : chunks)
                {
                    std::string sse_data =
                            "data: " + chunk_json.dump() + "\n\n";
                    if (!sink.write(sse_data.c_str(), sse_data.length()))
                    {
                        return false;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }

                const std::string done_data = "data: [DONE]\n\n";
                if (!sink.write(done_data.c_str(), done_data.length()))
                {
                    return false;
                }
                sink.done();
                return true;
            },
            [](bool) {});
}

void handle_mock_deepseek_non_streaming_completion(const httplib::Request& req,
                                                   httplib::Response& res)
{
    const nlohmann::json response_json = {
            {"id", "d018ff29-7ea0-44c0-8094-a945efdb29a7"},
            {"object", "chat.completion"},
            {"created", 1761623390},
            {"model", MOCK_MODEL},
            {"choices",
             {{{"index", 0},
               {"message",
                {{"role", "assistant"}, {"content", MOCK_RESPONSE_CONTENT}}},
               {"logprobs", nullptr},
               {"finish_reason", "stop"}}}},
            {"usage",
             {{"prompt_tokens", 10},
              {"completion_tokens", 11},
              {"total_tokens", 21},
              {"prompt_tokens_details", {{"cached_tokens", 0}}},
              {"prompt_cache_hit_tokens", 0},
              {"prompt_cache_miss_tokens", 10}}},
            {"system_fingerprint", "fp_ffc7281d48_prod0820_fp8_kvcache"}};
    res.status = 200;
    res.set_content(response_json.dump(), "application/json");
}
} // namespace

TEST_CASE("DeepSeekAdapter against Local Server", "[adapter_e2e]")
{
    QA::Test::LLMHttpServer server(MOCK_HOST, MOCK_PORT);
    std::string base_api_url = server.get_url();

    server.on_post(MOCK_API_PATH, MOCK_HANDLER);
    REQUIRE(server.start());

    std::string mock_url = server.get_url() + MOCK_API_PATH;
    QA::Core::DeepSeekAdapter adapter(MOCK_MODEL, MOCK_API_KEY, mock_url);
    std::vector<QA::Core::Message> messages = {{"user", "Hi"}};

    SECTION("No streaming request Test")
    {
        auto response = adapter.no_streaming_request(messages);
        REQUIRE(response.has_value());
        REQUIRE(response->message.role == "assistant");
        REQUIRE(response->message.content == MOCK_RESPONSE_CONTENT);
        REQUIRE(response->finish_reason == "stop");
        REQUIRE(response->usage.total_tokens == 21);
        REQUIRE(response->model_name == MOCK_MODEL);
        REQUIRE(response->id == "d018ff29-7ea0-44c0-8094-a945efdb29a7");
    }

    SECTION("Streaming request Test")
    {
        std::string accumulated_content;
        auto callback = [&](const std::string_view content)
        { accumulated_content += content; };
        auto response = adapter.streaming_request(messages, callback);
        REQUIRE(response.has_value());
        std::string expected_content =
                std::accumulate(MOCK_RESPONSE_CONTENT_VECTOR.begin(),
                                MOCK_RESPONSE_CONTENT_VECTOR.end(),
                                std::string{});
        REQUIRE(accumulated_content == expected_content);

        REQUIRE(response->message.role == "assistant");
        REQUIRE(response->message.content == expected_content);
        REQUIRE(response->finish_reason == "stop");
        REQUIRE(response->usage.total_tokens == 34);
        REQUIRE(response->model_name == MOCK_MODEL);
        REQUIRE(response->id == "00321d2f-08e2-46ae-91e7-2fc66660e4b4");
    }
}

TEST_CASE("DeepSeekAdapter error handling", "[adapter_e2e]")
{
    QA::Test::LLMHttpServer server(MOCK_HOST, MOCK_PORT);
    server.on_post(MOCK_API_PATH, MOCK_HANDLER);
    REQUIRE(server.start());
    std::string correct_url = server.get_url();
    server.stop();

    QA::Test::LLMHttpServer server_bad_key(MOCK_HOST, MOCK_PORT + 1);
    server_bad_key.on_post(MOCK_API_PATH, MOCK_HANDLER);
    REQUIRE(server_bad_key.start());
    std::string bad_key_url = server_bad_key.get_url();

    SECTION("Request to a closed port (connection failure)")
    {
        QA::Core::DeepSeekAdapter adapter(
                MOCK_MODEL, MOCK_API_KEY, correct_url);
        auto response_no_stream =
                adapter.no_streaming_request({{"user", "Hi"}});
        REQUIRE_FALSE(response_no_stream.has_value());

        auto response_stream =
                adapter.streaming_request({{"user", "Hi"}}, nullptr);
        REQUIRE_FALSE(response_stream.has_value());
    }

    SECTION("Request with invalid API key (401 Unauthorized)")
    {
        QA::Core::DeepSeekAdapter adapter(MOCK_MODEL, "wrong_key", bad_key_url);
        auto response = adapter.no_streaming_request({{"user", "Hi"}});
        REQUIRE_FALSE(response.has_value());
    }
}
