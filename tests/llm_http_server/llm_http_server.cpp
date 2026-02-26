//
// Created by 31305 on 2025/10/23.
//
#include "llm_http_server.h"
#include <chrono>
#include <iostream>

namespace QA::Test
{

LLMHttpServer::LLMHttpServer(std::string_view host, int port)
    : m_host(host), m_port(port)
{
}

LLMHttpServer::~LLMHttpServer()
{
    stop();
}

bool LLMHttpServer::start()
{
    if (m_server_thread)
    {
        return true;
    }

    m_server_thread = std::make_unique<std::thread>([this]() {
        m_server.listen(m_host.c_str(), m_port);
    });

    // Wait a bit for the server to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return true;
}

void LLMHttpServer::stop()
{
    if (m_server_thread && m_server_thread->joinable())
    {
        m_server.stop();
        m_server_thread->join();
        m_server_thread.reset();
    }
}

std::string LLMHttpServer::get_url() const
{
    return "http://" + m_host + ":" + std::to_string(m_port);
}

void LLMHttpServer::on_post(const std::string& api_path,
                            std::function<void(const httplib::Request&,
                                               httplib::Response&)> handler)
{
    m_server.Post(api_path, handler);
}

} // namespace QA::Test
