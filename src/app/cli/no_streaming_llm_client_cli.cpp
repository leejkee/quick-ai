//
// Created by 31305 on 2025/10/18.
//
#include <cstdlib>
#include <iostream>
#include <llm/llm_client_factory.h>
#include <llm/llm_conversation.h>
#include <string>

int main()
{
    // const char* api_key_env = std::getenv("DEEPSEEK_API_KEY");
    const char* api_key_env = std::getenv("QWEN_API_KEY");
    if (!api_key_env)
    {
        std::cerr << "Error: DEEPSEEK_API_KEY environment variable not set."
                  << std::endl;
        return 1;
    }
    const std::string api_key{api_key_env};
    // const std::string api_url = "https://api.deepseek.com/chat/completions";
    const std::string api_url = "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions";
    QA::Core::LLMConversation conversation;
    // const QA::Core::ModelMeta model_meta{"deepseek-chat", api_key, api_url};
    const QA::Core::ModelMeta model_meta{"qwen3-max", api_key, api_url};
    const auto client =
            QA::Core::LLMClientFactory::create_llm_client(model_meta);
    std::cout << "Time: " << conversation.get_start_time_str() << std::endl;

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
        if (const auto r = client->no_streaming_chat(
                    QA::Core::ModelParams(), conversation.get_context()))
        {
            const auto& [message, total_tokens] = r.value();
            conversation.push_message(message);
            std::cout << "A: " << message.content << '\n';
            std::cout << "[Total tokens]: " << total_tokens << '\n';
        }
        else
        {
            std::cerr << "A: Error: API request failed." << std::endl;
        }
    }
    return 0;
}
