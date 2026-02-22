//
// Created by 31305 on 2025/10/18.
//
#include <QCoreApplication>
#include <cstdlib>
#include <iostream>
#include <string>
#include <llm/LLMConversation.h>
#include <llm/OpenAIClient.h>
#include <llm/LLMModels.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    const char* api_key_env = std::getenv("DEEPSEEK_API_KEY");
    if (!api_key_env)
    {
        std::cerr << "Error: DEEPSEEK_API_KEY environment variable not set."
                  << std::endl;
        return 1;
    }

    const QString api_key{api_key_env};
    const QString api_url = "https://api.deepseek.com/chat/completions";
    QA::Core::LLMConversation conversation;
    const QA::Core::PostBody model_meta{"deepseek-chat", api_key, api_url};
    auto client = QA::Core::OpenAIClient(model_meta);

    while (true)
    {
        constexpr QA::Core::ModelParams model_params{};
        std::string q_str;
        std::cout << "Q: ";
        std::getline(std::cin, q_str);
        if (q_str == "exit")
        {
            break;
        }
        conversation.pushMessage({"user", QString::fromStdString(q_str)});

        if (auto r = client.noStreamingChat(
                    model_params, conversation.getContext()))
        {
            const auto [role, content, totalTokens] = r.value();
            QA::Core::Message r_message{role, content};
            conversation.pushMessage(r_message);
            std::cout << "A: " << content.toStdString() << '\n';
            std::cout << "[Total tokens]: " << totalTokens << '\n';
        }
        else
        {
            std::cerr << "A: Error: API request failed." << std::endl;
        }

        QCoreApplication::processEvents();
    }
    return 0;
}
