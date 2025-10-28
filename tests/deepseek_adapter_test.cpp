//
// Created by 31305 on 2025/10/23.
//
#include "fakehttp/include/fakehttpserver.h"
#include <llm/deepseekadapter.h>
#include <catch2/catch_test_macros.hpp>
#include <llm/models.h>
#include <string>
#include <vector>
#include <iostream>

namespace
{
    const std::string MOCK_HOST = "localhost";
    constexpr int MOCK_PORT = 8080; // 确保这个端口在测试时是空闲的
    const std::string MOCK_API_KEY = "test_key";
    const std::string MOCK_MODEL = "mock-deepseek-model";
}


TEST_CASE("DeepSeekAdapter streaming against Fake Server", "[adapter_e2e]")
{
    // --- 准备 ---
    // 1. 启动模拟服务器
    // RAII: server 对象在 TEST_CASE 结束时会自动调用析构函数，从而停止服务器。
    MockDeepSeekServer server(MOCK_HOST, "/chat/completions");
    REQUIRE(server.start(MOCK_PORT)); // 确保服务器成功启动

    // 2. 配置 Adapter 指向模拟服务器
    std::string mock_url = server.get_url();
    QA::Core::DeepSeekAdapter adapter(MOCK_MODEL, MOCK_API_KEY, mock_url);

    // 3. 准备请求数据和回调
    std::vector<QA::Core::Message> messages = {{"user", "Hello stream"}};

    std::string accumulated_content;
    auto callback = [&](const std::string_view content)
    {
        // (可选) 实时打印流式输出，便于调试
        // std::cout << content << std::flush;
        accumulated_content += content;
    };

    // --- 执行 ---
    auto response = adapter.streaming_request(messages, callback);
    // std::cout << std::endl;

    // --- 验证 ---
    // 1. 检查是否收到了有效的响应对象
    REQUIRE(response.has_value());

    // 2. 验证通过 callback 累积的内容
    // (这些值必须与 fakehttpserver.h 中模拟的数据一致)
    REQUIRE(accumulated_content == "Hello! This is a stream.");

    // 3. 验证 adapter 返回的最终 CommonChatResponse 对象
    REQUIRE(response->message.role == "assistant");
    REQUIRE(response->message.content == "Hello! This is a stream.");
    REQUIRE(response->finish_reason == "stop");
    REQUIRE(response->usage.total_tokens == 20);
    REQUIRE(response->model_name == "mock-deepseek-model");
    REQUIRE(response->id == "cmpl-mock-stream-456");
}

TEST_CASE("DeepSeekAdapter no-streaming against Fake Server", "[adapter_e2e]")
{
    // --- 准备 ---
    MockDeepSeekServer server(MOCK_HOST, "/chat/completions");
    REQUIRE(server.start(MOCK_PORT));

    std::string mock_url = server.get_url();
    QA::Core::DeepSeekAdapter adapter(MOCK_MODEL, MOCK_API_KEY, mock_url);

    std::vector<QA::Core::Message> messages = {{"user", "Hello"}};

    // --- 执行 ---
    auto response = adapter.no_streaming_request(messages);

    // --- 验证 ---
    // 1. 检查是否收到了有效的响应对象
    REQUIRE(response.has_value());

    // 2. 验证响应内容
    // (这些值必须与 fakehttpserver.h 中模拟的数据一致)
    REQUIRE(response->message.role == "assistant");
    REQUIRE(response->message.content == "This is a non-streaming response.");
    REQUIRE(response->finish_reason == "stop");
    REQUIRE(response->usage.total_tokens == 15);
    REQUIRE(response->model_name == "mock-deepseek-model");
    REQUIRE(response->id == "cmpl-mock-no-stream-123");
}

TEST_CASE("DeepSeekAdapter error handling", "[adapter_e2e]")
{
    // --- 准备 ---
    // 1. 故意使用错误的端口启动服务器，然后立即停止
    MockDeepSeekServer server(MOCK_HOST, "/chat/completions");
    REQUIRE(server.start(MOCK_PORT));
    std::string correct_url = server.get_url();
    server.stop(); // 立刻停止服务器

    // 2. 测试一个无效的 API Key
    MockDeepSeekServer server_bad_key(MOCK_HOST, "/chat/completions");
    REQUIRE(server_bad_key.start(MOCK_PORT + 1));
    std::string bad_key_url = server_bad_key.get_url();


    SECTION("Request to a closed port (connection failure)")
    {
        QA::Core::DeepSeekAdapter adapter(MOCK_MODEL, MOCK_API_KEY, correct_url); // URL 是对的，但服务器已停止
        auto response_no_stream = adapter.no_streaming_request({{"user", "Hi"}});
        REQUIRE_FALSE(response_no_stream.has_value());

        auto response_stream = adapter.streaming_request({{"user", "Hi"}}, nullptr);
        REQUIRE_FALSE(response_stream.has_value());
    }

    SECTION("Request with invalid API key (401 Unauthorized)")
    {
        QA::Core::DeepSeekAdapter adapter(MOCK_MODEL, "wrong_key", bad_key_url);
        auto response = adapter.no_streaming_request({{"user", "Hi"}});
        // 模拟服务器会返回 401，我们的 adapter 应该将其视作失败
        REQUIRE_FALSE(response.has_value());
    }
}