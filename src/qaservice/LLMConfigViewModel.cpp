//
// Created by 31305 on 2025/12/24.
//

#include <ConfigService/ConfigService.h>
#include <ConfigViewModel/LLMConfigViewModel.h>

#include "ConfigViewModel/ConfigViewModel.h"

namespace QA::Service
{
LLMConfigViewModel::LLMConfigViewModel(ConfigViewModel* parentVM, QObject* parent)
    : QObject(parent), m_parentVM(parentVM)
{
}


QString LLMConfigViewModel::getCurrentProvider() const
{
    return m_currentProvider;
}

QStringList LLMConfigViewModel::getModels() const { return m_models; }

QStringList LLMConfigViewModel::getProviders() const
{
    QStringList pId;
    for (const auto& p: m_parentVM->draft().m_llmSettings.m_providers)
    {
        pId.append(p.id);
    }
    return pId;
}

QString LLMConfigViewModel::getSystemPrompt() const
{
    return m_parentVM->draft().m_llmSettings.m_systemPrompt;
}


void LLMConfigViewModel::setCurrentProvider(const QString& providerId)
{
    if (m_currentProvider != providerId)
    {
        m_currentProvider = providerId;
    }
    refreshModels();
}

void LLMConfigViewModel::refreshModels()
{
    if (m_parentVM)
    {

    }
    else
    {
        m_models.clear();
    }
    // notify qml to update the models list
    Q_EMIT signalModelsChanged();
}

void LLMConfigViewModel::setSystemPrompt(const QString& systemPrompt)
{
    m_service->setSystemPrompt(systemPrompt);
}


QString LLMConfigViewModel::getActiveModel() const
{
    return m_service->getActiveModel();
}

QString LLMConfigViewModel::getActiveProvider() const
{
    return m_service->getActiveProvider();
}

void LLMConfigViewModel::setActiveModel(const QString& model)
{
    m_service->setActiveModel(model);
}

void LLMConfigViewModel::setActiveProvider(const QString& providerId)
{
    m_service->setActiveProvider(providerId);
}

} // namespace QA::Service
