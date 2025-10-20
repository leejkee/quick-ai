//
// Created by 31305 on 2025/10/20.
//
#include <llm/client.h>
#include <llm/conversation.h>
#include <iostream>
#include <string>
#include <cstdlib>

int main()
{
    const char* api_key_env = std::getenv("DEEPSEEK_API_KEY");
    if (!api_key_env)
    {
        std::cerr << "Error: DEEPSEEK_API_KEY environment variable not set." <<
                std::endl;
        return 1;
    }
    const std::string api_key{api_key_env};
    const QA::Core::Message system_prompt{
        "system"
        , "You are a translation expert."
    };
    QA::Core::Conversation conversation{system_prompt};
    QA::Core::LLMClient client(api_key);
    std::cout << "Time: " << conversation.get_start_time() << std::endl;
    std::cout << "Starting chat session (system prompt: '" << system_prompt.
            content << "'). Type 'exit' to end." << std::endl;

    while (true)
    {
        std::string content;
        std::cout << "Q: ";
        std::getline(std::cin, content);
        if (content == "exit")
        {
            break;
        }
        std::cout << "A: ";
        conversation.push_message({"user", content});
        QA::Core::Message a_message;
        a_message.role = "assistant";

        auto on_content_received = [&a_message](
            const std::string_view& content_chunk)
        {
            a_message.content += content_chunk;
            std::cout << content_chunk << std::flush;
        };

        auto on_stream_complete = [](const std::string_view& reason
                                     , const QA::Core::Usage& usage)
        {
            std::cout << "\n[Stream finished. Reason: " << reason << "]";
            std::cout << " [Total Tokens: " << usage.total_tokens << "]\n";
        };

        if (!client.streaming_request(conversation.get_messages()
                                      , on_content_received
                                      , on_stream_complete))
        {
            std::cerr << "request failed." << std::endl;
            break;
        }
        conversation.push_message(a_message);
    }
    return 0;
}
