//
// Created by 31305 on 2025/10/23.
//
#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include "httplib.h"

namespace QA::Test
{

using json = nlohmann::json;

class LLMHttpServer
{
public:
    LLMHttpServer(std::string_view host, int port);

    ~LLMHttpServer();

    bool start();

    void stop();

    std::string get_url() const;

    void on_post(const std::string& api_path,
                 std::function<void(const httplib::Request&,
                                    httplib::Response&)> handler);

private:
    httplib::Server m_server;
    std::unique_ptr<std::thread> m_server_thread;
    std::string m_host;
    int m_port = 0;
    std::string m_api_path;
};
} // namespace QA::Test
