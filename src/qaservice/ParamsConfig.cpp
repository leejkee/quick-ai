//
// Created by 31305 on 2026/1/26.
//
#include <ChatService/ParamsConfig.h>
namespace QA::Service
{
ParamsConfig::ParamsConfig(const ConfigModelParams& params, QObject* parent)
    : QObject(parent), m_params(params)
{
}

void ParamsConfig::setMaxTokens(int tokens)
{
    if (tokens < 0 || tokens > 8192)
    {
        return;
    }
    m_params.data.max_tokens = tokens;
}

void ParamsConfig::setTemp(double t)
{
    if (t < 0 || t > 2)
    {
        return;
    }
    m_params.data.temperature = t;
}

void ParamsConfig::setTopP(double p)
{
    if (p < 0 || p > 1)
    {
        return;
    }
    m_params.data.top_p = p;
}


} // namespace QA::Service
