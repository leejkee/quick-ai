//
// Created by 31305 on 2025/10/18.
//
#include <cstdlib>
#include <iostream>
#include <llm/llm_client.h>
#include <llm/llm_conversation.h>
#include <string>

int main()
{
    const char* api_key_env = std::getenv("DEEPSEEK_API_KEY");
    if (!api_key_env)
    {
        std::cerr << "Error: DEEPSEEK_API_KEY environment variable not set."
                  << std::endl;
        return 1;
    }
    const std::string api_key{api_key_env};
    const QA::Core::Message system_prompt{"system",
                                          "You are a translation expert."};
    QA::Core::LLMConversation conversation{system_prompt};
    QA::Core::LLMClient client(QA::Core::LLMClient::Model::deepseek_chat,
                               api_key);
    std::cout << "Time: " << conversation.get_start_time_str() << std::endl;
    std::cout << "Starting chat session (system prompt: '"
              << system_prompt.content << "'). Type 'exit' to end."
              << std::endl;

    while (true)
    {
        std::string content;
        std::cout << "Q: ";
        std::getline(std::cin, content);
        if (content == "exit")
        {
            break;
        }
        conversation.push_message({"user", content});
        if (const auto r =
                    client.no_streaming_request(conversation.get_messages()))
        {
            const auto& assistant_message = r.value();
            conversation.push_message(assistant_message.message);
            std::cout << "A: " << assistant_message.message.content << '\n';
            std::cout << "[finish_reason]: " << assistant_message.finish_reason
                      << '\n';
            std::cout << "[Model]: " << assistant_message.model_name << '\n';
            std::cout << "[Total tokens]: "
                      << assistant_message.usage.total_tokens << '\n';
        }
        else
        {
            std::cerr << "A: Error: API request failed." << std::endl;
        }
    }
    return 0;
}
