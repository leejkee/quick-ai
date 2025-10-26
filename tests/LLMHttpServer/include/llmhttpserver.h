//
// Created by 31305 on 2025/10/23.
//
#pragma once
#include "httplib.h"
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <memory>
#include <iostream>
#include <vector>
#include <chrono>

using json = nlohmann::json;

/**
 * @brief 使用 httplib 创建一个模拟 DeepSeek API 的本地服务器。
 *
 * 这个类在一个单独的线程中启动一个HTTP服务器，
 * 侦听POST请求，并根据请求中的 "stream" 字段
 * 返回模拟的JSON响应（流式或非流式）。
 */
class LLMHttpServer
{
public:
    explicit LLMHttpServer(const std::string_view host = "localhost",
                       const std::string_view path = "/chat/completions")
        : m_host(host),
          m_api_path(path)
    {
        setup_handlers();
    }

    ~LLMHttpServer()
    {
        stop();
    }

    bool start(const int port)
    {
        m_port = port;
        if (m_server_thread)
        {
            std::cerr << "Mock server is already running." << std::endl;
            return false;
        }

        // 在新线程中运行 httplib::Server::listen
        m_server_thread = std::make_unique<std::thread>([this]()
        {
            std::cout << "[Mock Server] Starting on " << get_url() << std::endl;
            if (!m_svr.listen(m_host, m_port))
            {
                std::cerr << "[Mock Server] Failed to listen." << std::endl;
            }
            std::cout << "[Mock Server] Stopped." << std::endl;
        });

        // 等待服务器线程启动
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return m_svr.is_running();
    }

    /**
     * @brief 停止服务器并等待线程结束。
     */
    void stop()
    {
        if (m_svr.is_running())
        {
            m_svr.stop();
        }
        if (m_server_thread && m_server_thread->joinable())
        {
            m_server_thread->join();
        }
        m_server_thread.reset();
    }

    /**
     * @brief 获取服务器的完整 URL（用于客户端配置）。
     * @return 类似 "http://localhost:8080/chat/completions" 的字符串。
     */
    std::string get_url() const
    {
        return "http://" + m_host + ":" + std::to_string(m_port) + m_api_path;
    }

private:
    void setup_handlers()
    {
        m_svr.Post(m_api_path, [this](const httplib::Request& req,
                                             httplib::Response& res)
        {
            // 1. 检查认证 (用于测试)
            if (req.get_header_value("Authorization") != "Bearer test_key")
            {
                res.status = 401;
                res.set_content(R"({"error": "Invalid API key."})",
                                "application/json");
                std::cerr << "[Mock Server] Received bad auth header." << std::endl;
                return;
            }

            // 2. 解析请求体以检查 'stream' 字段
            bool is_streaming = false;
            try
            {
                json req_json = json::parse(req.body);
                if (req_json.contains("stream") && req_json["stream"].is_boolean())
                {
                    is_streaming = req_json["stream"].get<bool>();
                }
            }
            catch (const json::parse_error& e)
            {
                res.status = 400;
                res.set_content(R"({"error": "Invalid JSON."})",
                                "application/json");
                std::cerr << "[Mock Server] Failed to parse request JSON: " << e.
                        what() << std::endl;
                return;
            }

            // 3. 根据 'stream' 字段调用不同的处理器
            if (is_streaming)
            {
                std::cout << "[Mock Server] Handling STREAMING request." <<
                        std::endl;
                handle_streaming(req, res);
            }
            else
            {
                std::cout << "[Mock Server] Handling NO-STREAMING request." <<
                        std::endl;
                handle_no_streaming(req, res);
            }
        });
    }

    // 处理非流式请求
    void handle_no_streaming(const httplib::Request& req, httplib::Response& res)
    {
        json response_json = {
            {"id", "cmpl-mock-no-stream-123"},
            {"model", "mock-deepseek-model"},
            {"choices", {{
                {
                    "index", 0
                },
                {
                    "message", {
                        {"role", "assistant"},
                        {"content", "This is a non-streaming response."}
                    }
                },
                {
                    "finish_reason", "stop"
                }
            }}},
            {"usage", {
                {"prompt_tokens", 10},
                {"completion_tokens", 5},
                {"total_tokens", 15}
            }}
        };
        res.status = 200;
        res.set_content(response_json.dump(), "application/json");
    }

    // 处理流式请求 (SSE)
    void handle_streaming(const httplib::Request& req, httplib::Response& res)
    {
        res.set_content_provider(
            "text/event-stream",
            [this](size_t offset, httplib::DataSink& sink)
            {
                if (offset > 0)
                {
                    sink.done();
                    return true;
                }

                // 定义要发送的数据块
                std::vector<json> chunks = {
                    // 1. 发送角色
                    {
                        {"id", "cmpl-mock-stream-456"},
                        {"model", "mock-deepseek-model"},
                        {"choices", {{{ "index", 0 }, { "delta", { {"role", "assistant"} } }}}}
                    },
                    // 2. 发送内容
                    {{"choices", {{{ "index", 0 }, { "delta", { {"content", "Hello! "} } }}}}},
                    {{"choices", {{{ "index", 0 }, { "delta", { {"content", "This is "} } }}}}},
                    {{"choices", {{{ "index", 0 }, { "delta", { {"content", "a stream."} } }}}}},
                    // 3. 发送结束原因和 usage
                    {
                        {"id", "cmpl-mock-stream-456"},
                        {"model", "mock-deepseek-model"},
                        {"choices", {{{ "index", 0 }, { "delta", {} }, { "finish_reason", "stop" }}}
                        },
                        {"usage", {
                            {"prompt_tokens", 12},
                            {"completion_tokens", 8},
                            {"total_tokens", 20}
                        }}
                    }
                };

                // 依次发送数据块
                for (const auto& chunk_json : chunks)
                {
                    std::string sse_data = "data: " + chunk_json.dump() + "\n\n";
                    if (!sink.write(sse_data.c_str(), sse_data.length()))
                    {
                        return false; // 客户端断开连接
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 模拟延迟
                }

                // 4. 发送 [DONE] 标记
                const std::string done_data = "data: [DONE]\n\n";
                sink.write(done_data.c_str(), done_data.length());

                sink.done(); // 告知 httplib 数据发送完毕
                return true;
            },
            [](bool success)
            {
                // 可选的清理回调
            }
        );
    }

    httplib::Server m_svr;
    std::unique_ptr<std::thread> m_server_thread;
    std::string m_host;
    int m_port = 0;
    std::string m_api_path;
};