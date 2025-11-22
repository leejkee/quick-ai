//
// Created by 31305 on 2025/11/21.
//
#pragma once
#include <llm/llm_client_base.h>
#include <memory>

namespace QA::Core
{

class LLMClientFactory
{
public:
    static std::unique_ptr<LLMClientBase>
    create_llm_client(const ModelMeta& model_meta);
};

} // namespace QA::Core
