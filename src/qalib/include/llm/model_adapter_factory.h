//
// Created by 31305 on 2025/10/22.
//
#pragma once
#include "llm_adapter_interface.h"
#include "deepseek_adapter.h"
#include <memory>

namespace QA::Core
{
class ModelAdapterFactory
{
public:
    static std::shared_ptr<LLMAdapterInterface> createModelAdapter(
        const std::string_view model
        , std::string_view api_key)
    {
        if (model.rfind("deepseek", 0) == 0)
        {
            return std::make_shared<DeepSeekAdapter>(model, api_key);
        }
        return {};
    }
};
}
