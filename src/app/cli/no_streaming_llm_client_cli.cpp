//
// Created by 31305 on 2025/10/18.
//
#include <cstdlib>
#include <iostream>
#include <llm/LLMClientFactory.h>
#include <llm/LLMConversation.h>
#include <string>

int main()
{
    const char* api_key_env = std::getenv("DEEPSEEK_API_KEY");
    // const char* api_key_env = std::getenv("QWEN_API_KEY");
    if (!api_key_env)
    {
        std::cerr << "Error: DEEPSEEK_API_KEY environment variable not set."
                  << std::endl;
        return 1;
    }
    const QString api_key{api_key_env};
    const QString api_url = "https://api.deepseek.com/chat/completions";
    // const QString api_url = "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions";
    QA::Core::LLMConversation conversation;
    // const QA::Core::ModelMeta model_meta{"deepseek-chat", api_key, api_url};
    const QA::Core::PostBody model_meta{"deepseek-chat", api_key, api_url};
    const auto client =
            QA::Core::LLMClientFactory::createLLMClient(model_meta);

    while (true)
    {
        std::string content;
        std::cout << "Q: ";
        std::getline(std::cin, content);
        if (content == "exit")
        {
            break;
        }
        conversation.pushMessage({"user", QString::fromStdString(content)});
        if (const auto r = client->noStreamingChat(
                    QA::Core::ModelParams(), conversation.getContext()))
        {
            const auto& [message, total_tokens] = r.value();
            conversation.pushMessage(message);
            std::cout << "A: " << message.content.toStdString() << '\n';
            std::cout << "[Total tokens]: " << total_tokens << '\n';
        }
        else
        {
            std::cerr << "A: Error: API request failed." << std::endl;
        }
    }
    return 0;
}
