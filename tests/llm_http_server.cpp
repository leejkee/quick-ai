//
// Created by 31305 on 2025/10/26.
//
#include <chrono>
#include <iostream>
#include <llm_http_server/llm_http_server.h>
#include <vector>

namespace QA::Test
{
LLMHttpServer::LLMHttpServer(const std::string_view host, const int port)
    : m_host(host), m_port(port)
{
}

LLMHttpServer::~LLMHttpServer() { stop(); }

bool LLMHttpServer::start()
{
    if (m_server_thread)
    {
        std::cerr << "Mock server is already running." << std::endl;
        return false;
    }

    m_server_thread = std::make_unique<std::thread>(
            [this]()
            {
                std::cout << "[Mock Server] Starting on " << get_url()
                          << std::endl;
                if (!m_server.listen(m_host, m_port))
                {
                    if (m_server.is_running())
                    {
                        std::cerr << "[Mock Server] Failed to listen."
                                  << std::endl;
                    }
                }
                std::cout << "[Mock Server] Stopped." << std::endl;
            });

    int retry_count = 3;
    while (!m_server.is_running() && retry_count--)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!m_server.is_running())
    {
        std::cerr << "[Mock Server] Failed to start." << std::endl;
        stop();
        return false;
    }
    return true;
}

void LLMHttpServer::stop()
{
    if (m_server.is_running())
    {
        m_server.stop();
    }
    if (m_server_thread && m_server_thread->joinable())
    {
        m_server_thread->join();
    }
    m_server_thread.reset();
}

std::string LLMHttpServer::get_url() const
{
    return "http://" + m_host + ":" + std::to_string(m_port);
}

void LLMHttpServer::on_post(const std::string& api_path,
                            std::function<void(const httplib::Request&,
                                               httplib::Response&)> handler)
{
    if (m_server.is_running())
    {
        std::cerr << "[Mock Server] Cannot register handler while server is "
                     "running."
                  << std::endl;
        return;
    }
    m_server.Post(api_path, std::move(handler));
}
} // namespace QA::Test
