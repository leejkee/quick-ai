//
// Created by 31305 on 2025/10/16.
//
#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace QA::Core
{

struct Message
{
    std::string role;
    std::string content;
};

struct Usage
{
    int completion_tokens{0};
    int prompt_tokens{0};
    int total_tokens{0};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Message, role, content)


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Usage
                                   , completion_tokens
                                   , prompt_tokens
                                   , total_tokens)

}
