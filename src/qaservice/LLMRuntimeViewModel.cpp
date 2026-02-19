//
// Created by 31305 on 2026/1/29.
//
#include <SessionService/LLMRuntimeViewModel.h>
#include <UserSettings/SettingsRepository.h>
namespace QA::Service
{
LLMRuntimeViewModel::LLMRuntimeViewModel(SettingsRepository* settingsRepo, QObject* parent) : QObject(parent), m_settingsRepo(settingsRepo)
{
    Provider provider_deepseek = {
            "DeepSeek",
            "https://api.deepseek.com",
            "sk-1d00763b38184cfaafb5b3ea8cfd3b7e",
            {{"deepseek-chat", "/chat/completions"},
             {"deepseek-reasoner", "/chat/completions"}}};

    Provider provider_qwen = {
            "Qwen3",
            "https://dashscope.aliyuncs.com",
            "sk-c14ea95925184cf2a4609f0931a1a9c4",
            {{"qwen3-max", "/compatible-mode/v1/chat/completions/test"}}};

    m_providers = {provider_deepseek, provider_qwen};
    m_selectedProviderId = "Qwen3";
    m_selectedModel = "qwen3-max";
}


Provider LLMRuntimeViewModel::getSelectedProvider() const
{
    if (const auto r = getDataFromVector(
                m_providers,
                [this](const Provider& provider)
                { return provider.id == m_selectedProviderId; });
        r.has_value())
    {
        return r.value();
    }
    return {};
}

QString LLMRuntimeViewModel::getAPIKey() const
{
    return getSelectedProvider().apiKey;
}

QString LLMRuntimeViewModel::getBaseURL() const
{
    return getSelectedProvider().baseUrl;
}

QStringList LLMRuntimeViewModel::getModelList() const
{
    auto models = getSelectedProvider().models;
    QStringList modelNameList;
    for (const auto& [name, endpoint] : models)
    {
        modelNameList.append(name);
    }
    return modelNameList;
}

QStringList LLMRuntimeViewModel::getProviderList() const
{
    QStringList providerIdList;
    for (const auto& provider : m_providers)
    {
        providerIdList.append(provider.id);
    }
    return providerIdList;
}

void LLMRuntimeViewModel::setSelectedModel(const QString& model)
{
    if (m_selectedModel != model)
    {
        m_selectedModel = model;
        Q_EMIT signalSelectedModelChanged(model);
    }
}

void LLMRuntimeViewModel::setSelectedProviderId(const QString& providerId)
{
    if (m_selectedProviderId != providerId)
    {
        m_selectedProviderId = providerId;
        Q_EMIT signalSelectedProviderIdChanged(providerId);
        if (QStringList modelList = getModelList(); !modelList.isEmpty())
        {
            setSelectedModel(modelList.first());
        }
        else
        {
            setSelectedModel({});
        }
    }
}

QString LLMRuntimeViewModel::getUrl() const
{
    return getSelectedProvider().getUrl(m_selectedModel);
}


} // namespace QA::Service
