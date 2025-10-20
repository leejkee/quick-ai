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

bool LLMClient::streaming_request(const std::vector<Message>& messages
                                  , const std::function<void(
                                      const std::string_view& content_chunk)>&
                                  content_chunk_received
                                  , const std::function<void(
                                      const std::string_view& finish_reason
                                      , const Usage& usage_info)>&
                                  completion_chunk_received)
{
    if (messages.empty())
    {
        std::cerr << "Empty messages" << std::endl;
        return false;
    }

    RequestPacket request_packet{};
    request_packet.model = Model::deepseek_chat;
    request_packet.stream = true;
    request_packet.messages = messages;

    const nlohmann::json request_json = request_packet;

    cpr::Session session;
    session.SetUrl(m_url);
    session.SetHeader({
                          {"Content-Type", "application/json"}
                          , {
                              "Authorization"
                              , "Bearer " + m_api_key
                          }
                      });
    session.SetBody(request_json.dump());

    std::string buffer;
    session.SetWriteCallback(cpr::WriteCallback([&](const std::string_view data
                                                    , intptr_t userdata)
    {
        buffer += data;
        size_t pos;
        while ((pos = buffer.find("\n\n")) != std::string::npos)
        {
            std::string message = buffer.substr(0, pos);
            // "...}<pos>\n\ndata: {..."
            buffer.erase(0, pos + 2); // "...}\n\n<split>data: {..."
            if (const std::string prefix = "data: "; message.rfind(prefix, 0) ==
                0)
            {
                std::string jsonData = message.substr(prefix.length());
                // ...data: <L>{xxx}<R>...

                if (jsonData == "[DONE]")
                {
                    return true;
                }


                try
                {
                    const auto unknow_json = nlohmann::json::parse(jsonData);

                    if (const auto choices = unknow_json["choices"][0]; choices.
                        contains("finish_reason") && !choices[
                            "finish_reason"].is_null() && unknow_json.
                        contains("usage"))
                    {
                        std::string finish_reason = choices["finish_reason"];
                        const Usage usage = unknow_json["usage"].get<Usage>();
                        completion_chunk_received(finish_reason, usage);
                    }
                    else if (choices["delta"].contains("content") && !choices[
                        "delta"]["content"].is_null())
                    {
                        std::string content = choices["delta"]["content"];
                        if (!content.empty() && content_chunk_received)
                        {
                            content_chunk_received(content);
                        }
                    }
                } catch (const nlohmann::json::parse_error& e)
                {
                    std::cerr << "\n[JSON parse error]: " << e.what() << std::endl;
                }
                catch (const nlohmann::json::type_error& e)
                {
                    std::cerr << "\n[JSON type error]: " << e.what() << "\n";
                }
            }
        }
        return true;
    }));

    if (const cpr::Response r = session.Post(); r.status_code != 200)
    {
        std::cerr << "Request failed with status code: " << r.status_code <<
                std::endl;
        std::cerr << "Error message: " << r.text << std::endl;
        return false;
    }

    return true;
}


void LLMClient::print_response(const NoStreamingResponsePacket& response)
{
    std::string debug_info{"A: "};
    debug_info += response.choices[0].message.content;
    debug_info += "\nTotal tokens: " +
            std::to_string(response.usage.total_tokens);
    std::cout << debug_info << std::endl;
}
}
