//
// Created by 31305 on 2026/2/25.
//
#include <SessionService/LLMRuntimeContext.h>
#include <UserSettings/SettingsRepository.h>

namespace QA::Service
{
LLMRuntimeContext::LLMRuntimeContext(SettingsRepository* repo, QObject* parent)
    : QObject(parent), m_repo(repo)
{
    const auto settings = m_repo->getSettings();
    m_modelParams = settings.m_modelParams;
    m_selectedProviderId = settings.m_selectedProviderId;
    m_selectedModel = settings.m_selectedModel;
    m_providers = settings.m_providers;
    m_systemPrompt = settings.m_systemPrompt;

    connect(m_repo,
            &SettingsRepository::signalSettingsChanged,
            this,
            &LLMRuntimeContext::handleSettingsUpdate);
}

void LLMRuntimeContext::setSelectedModel(const QString& model)
{
    if (m_selectedModel != model)
    {
        m_selectedModel = model;
        Q_EMIT signalSelectedModelChanged();
    }
}

void LLMRuntimeContext::setSelectedProviderId(const QString& providerId)
{
    if (m_selectedProviderId != providerId)
    {
        m_selectedProviderId = providerId;
        Q_EMIT signalSelectedProviderIdChanged();
        Q_EMIT signalSelectedModelChanged();
    }
}

QString LLMRuntimeContext::getAPIKey() const
{
    const auto provider = getDataFromVector(
            m_providers,
            [this](const Provider& p) { return p.id == m_selectedProviderId; });
    if (!provider.has_value())
    {
        return {};
    }
    return provider->apiKey;
}

QString LLMRuntimeContext::getUrl() const
{
    const auto provider = getDataFromVector(
            m_providers,
            [this](const Provider& p) { return p.id == m_selectedProviderId; });
    if (!provider.has_value())
    {
        return {};
    }
    return provider->getUrl(m_selectedModel);
}

QStringList LLMRuntimeContext::getProviderList() const
{
    QStringList providerIdList;
    for (const auto& provider : m_providers)
    {
        providerIdList.append(provider.id);
    }
    return providerIdList;
}

QStringList LLMRuntimeContext::getModelList() const
{
    const auto provider = getDataFromVector(
            m_providers,
            [this](const Provider& p) { return p.id == m_selectedProviderId; });
    if (!provider.has_value())
    {
        return {};
    }
    return provider->getModelList();
}

void LLMRuntimeContext::handleSettingsUpdate()
{
    const auto& settings = m_repo->getSettings();
    
    if (m_selectedModel != settings.m_selectedModel)
    {
        m_selectedModel = settings.m_selectedModel;
        Q_EMIT signalSelectedModelChanged();
    }
    
    if (m_selectedProviderId != settings.m_selectedProviderId)
    {
        m_selectedProviderId = settings.m_selectedProviderId;
        Q_EMIT signalSelectedProviderIdChanged();
    }
    
    m_providers = settings.m_providers;
    m_systemPrompt = settings.m_systemPrompt;
    
    if (m_modelParams != settings.m_modelParams)
    {
        m_modelParams = settings.m_modelParams;
        Q_EMIT signalModelParamsChanged();
    }
    
    Q_EMIT signalProviderListChanged();
}

} // namespace QA::Service
