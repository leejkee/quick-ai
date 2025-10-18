//
// Created by 31305 on 2025/10/17.
//
#include <llm/client.h>
#include <llm/models.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <iostream>

namespace QA::Core
{
LLMClient::LLMClient(const std::string_view api_key)
    : m_api_key(api_key)
{
}

std::optional<NoStreamingResponsePacket> LLMClient::no_streaming_request(
    const std::vector<Message>& messages)
{
    if (messages.empty())
    {
        return std::nullopt;
    }
    RequestPacket request{};
    request.model = Model::deepseek_chat;
    request.messages = messages;
    const nlohmann::json request_json = request;
    const cpr::Response r = cpr::Post(cpr::Url{m_url}
                                      , cpr::Header{
                                          {"Content-Type", "application/json"}
                                          , {
                                              "Authorization"
                                              , "Bearer " + m_api_key
                                          }
                                      }
                                      , cpr::Body{request_json.dump()});
    if (r.status_code != 200)
    {
        std::cerr << "Request failed with status code: " << r.status_code <<
                std::endl;
        std::cerr << "Error message: " << r.text << std::endl;
        return std::nullopt;
    }

    try
    {
        const nlohmann::json response_json = nlohmann::json::parse(r.text);
        auto response = response_json.get<NoStreamingResponsePacket>();
        return response;
    } catch (const nlohmann::json::parse_error& e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return std::nullopt;
    }
    catch (const nlohmann::json::type_error& e)
    {
        std::cerr << "JSON type error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

void LLMClient::print_response(const NoStreamingResponsePacket& response)
{
    std::string debug_info{"A: "};
    debug_info += response.choices[0].message.content;
    debug_info += "\nTotal tokens: " + std::to_string(response.usage.total_tokens);
    std::cout << debug_info << std::endl;
}

}
