//
// Created by 31305 on 2025/10/18.
//
#include <llm/client.h>
int main()
{
    const std::string user_content{"一句话介绍一下deepseek"};
    QA::Core::Message message{"user", user_content};
    // QA::Core::LLMClient client();
    // const auto r = client.no_streaming_request({message});
    // QA::Core::LLMClient::print_response(r.value());
}