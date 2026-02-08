//
// Created by 31305 on 2025/11/21.
//
#pragma once
#include <llm/LLMClientBase.h>
#include <memory>

namespace QA::Core
{

class LLMClientFactory
{
public:
    static std::unique_ptr<LLMClientBase>
    createLLMClient(const PostBody& postBody);
};

} // namespace QA::Core
