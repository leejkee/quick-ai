//
// Created by 31305 on 2025/10/17.
//
#include <llm/deepseekadapter.h>
#include <llm/models.h>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <iostream>

namespace QA::Core
{
DeepSeekAdapter::DeepSeekAdapter(const std::string_view model
                                 , const std::string_view api_key)
    : m_api_key(api_key),
      m_model(model)
{
}

DeepSeekAdapter::DeepSeekAdapter(const std::string_view model
                                 , const std::string_view api_key
                                 , const std::string_view url)
    : m_api_key(api_key),
      m_model(model),
      m_url(url)
{
}


std::optional<CommonChatResponse> DeepSeekAdapter::no_streaming_request(
    const std::vector<Message>& messages)
{
    if (messages.empty())
    {
        return std::nullopt;
    }
    RequestPacket request{};
    request.model = m_model;
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
        const auto [id, choices, model, usage] = response_json.get<
            NoStreamingResponsePacket>();
        CommonChatResponse response;
        response.message = choices[0].message;
        response.finish_reason = choices[0].finish_reason;
        response.id = id;
        response.model_name = model;
        response.usage = usage;
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

std::optional<CommonChatResponse> DeepSeekAdapter::streaming_request(
    const std::vector<Message>& messages
    , const content_callback& content_call)
{
    if (messages.empty())
    {
        std::cerr << "Empty messages" << std::endl;
        return std::nullopt;
    }

    RequestPacket request_packet;
    request_packet.model = m_model;
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
    CommonChatResponse response;
    Message message;
    message.role = "assistant";

    session.SetWriteCallback(cpr::WriteCallback([&](const std::string_view data
                                                    , intptr_t userdata)
    {
        buffer += data;
        size_t pos;
        while ((pos = buffer.find("\n\n")) != std::string::npos)
        {
            std::string sse_chunk = buffer.substr(0, pos);
            // "...}<pos>\n\ndata: {..."
            buffer.erase(0, pos + 2); // "...}\n\n<split>data: {..."
            if (const std::string prefix{"data: "};
                sse_chunk.rfind(prefix, 0) == 0)
            {
                std::string jsonData = sse_chunk.substr(prefix.length());
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
                        response.finish_reason = choices["finish_reason"];
                        response.usage = unknow_json["usage"].get<Usage>();
                        response.id = unknow_json["id"];
                        response.model_name = unknow_json["model"];
                    }
                    else if (choices["delta"].contains("content") && !choices[
                        "delta"]["content"].is_null())
                    {
                        const std::string content = choices["delta"]["content"];
                        message.content += content;
                        if (!content.empty() && content_call)
                        {
                            content_call(content);
                        }
                    }
                } catch (const nlohmann::json::parse_error& e)
                {
                    std::cerr << "\n[JSON parse error]: " << e.what() <<
                            std::endl;
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
        return std::nullopt;
    }

    response.message = message;
    return response;
}
}
