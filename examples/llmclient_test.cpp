//
// Created by 31305 on 2025/10/18.
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
        conversation.push_message({"user", content});
        if (const auto r = client.
                no_streaming_request(conversation.get_messages()))
        {
            const auto& assistant_message = r.value().choices[0].message;
            conversation.push_message(assistant_message);
            std::cout << "A: " << assistant_message.content << std::endl;
        }
        else
        {
            std::cerr << "A: Error: API request failed." << std::endl;
        }
    }
    return 0;
}
