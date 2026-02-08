//
// Created by 31305 on 2025/11/24.
//
#include <ConfigService/ConfigService.h>
#include <QALog.h>
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>

namespace QA::Service
{
ConfigService::ConfigService(QObject* parent)
    : QObject(parent)
{
}

Core::ModelMeta ConfigService::getModelMeta() const
{
    Core::ModelMeta modelMeta;
    modelMeta.model = m_activeModelId.toStdString();

    if (const auto rP = getDataFromVector(m_providers,
                                          [this](const ConfigProvider& p)
                                          { return p.id == m_activeModelId; });
        rP.has_value())
    {
        const auto activeProviderConfig = rP.value();
        modelMeta.api_key = activeProviderConfig.apiKey.toStdString();

        if (const auto rM =
                    getDataFromVector(activeProviderConfig.models,
                                      [this](const ConfigModel& m)
                                      { return m.name == m_activeModelId; });
            rM.has_value())
        {
            modelMeta.url = (activeProviderConfig.baseUrl + rM.value().endpoint)
                                    .toStdString();
        }
    }
    return modelMeta;
}

Core::ModelParams ConfigService::getModelParams() const
{
    auto [frequencyPenalty,
          maxTokens,
          presencePenalty,
          temperature,
          topP,
          stream] = m_modelParams;
    return {frequencyPenalty,
            maxTokens,
            presencePenalty,
            temperature,
            topP,
            stream};
}




void ConfigService::applyConfig() {}

void ConfigService::setActiveModel(const QString& model)
{
    if (model.isEmpty())
    {
        QA_LOG_ERR("Model name is empty");
        return;
    }
    if (m_activeModelId != model)
    {
        m_activeModelId = model;
    }
}

void ConfigService::setActiveModelParams(const ConfigModelParams& params)
{
    if (m_modelParams != params)
    {
        m_modelParams = params;
    }
}

void ConfigService::setActiveProvider(const QString& provider)
{
    if (provider.isEmpty())
    {
        QA_LOG_ERR("Provider name is empty");
        return;
    }
    if (m_activeProviderId != provider)
    {
        m_activeProviderId = provider;
    }
}

} // namespace QA::Service
