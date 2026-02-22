//
// Created by 31305 on 2025/11/21.
//
#pragma once
#include "LLMClientBase.h"

namespace QA::Core
{

class LLMClientFactory
{
public:
    static LLMClientBase* createLLMClient(const PostBody& postBody,
                                          QObject* parent = nullptr);
};

} // namespace QA::Core
