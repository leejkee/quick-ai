//
// Created by 31305 on 2026/2/25.
//
#include <SessionService/LLMRuntimeContext.h>
#include <UserSettings/SettingsRepository.h>

namespace QA::Service
{
LLMRuntimeContext::LLMRuntimeContext(QObject* parent) : QObject(parent)
{

}

QString LLMRuntimeContext::getAPIKey() const
{
    return {};

}

QString LLMRuntimeContext::getFinalUrl() const
{
    return {};

}

void LLMRuntimeContext::handleSettingsUpdate()
{
    m_modelParams = m_repo->getSettings().m_modelParams;
    m_selectedModel = m_repo->getSettings().m_selectedModel;
    m_selectedProviderId = m_repo->getSettings().m_selectedProviderId;
    m_systemPrompt = m_repo->getSettings().m_systemPrompt;
}
} // namespace QA::Service
