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
ConfigService::ConfigService(const QString& configFile, QObject* parent)
    : QObject(parent), m_configFilePath(configFile)
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

void ConfigService::readConfig()
{
    if (m_configFilePath.isEmpty())
    {
        QA_LOG_ERR("Config file path is empty");
        return;
    }
    QFile file(m_configFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QA_LOG_ERR("Failed to open config file");
        return;
    }

    const QByteArray jsonData = file.readAll();
    file.close();
    QJsonParseError parseError;
    const QJsonDocument jsonDoc =
            QJsonDocument::fromJson(jsonData, &parseError);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        QA_LOG_ERR("Failed to parse JSON" + parseError.errorString());
        return;
    }
    QA_LOG_INFO("Parse JSON successfully");

    const QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj.contains("providers") && jsonObj["providers"].isArray())
    {
        QJsonArray providers = jsonObj["providers"].toArray();
        for (const auto& provider : providers)
        {
            m_providers.append(ConfigProvider::fromJson(provider.toObject()));
        }
    }
    else
    {
        QA_LOG_INFO("No key called [providers] in json");
    }

    if (jsonObj.contains("activeConfig") && jsonObj["activeConfig"].isObject())
    {
        const QJsonObject activeConfig = jsonObj["activeConfig"].toObject();
        if (activeConfig.contains("activeModel") &&
            activeConfig["activeModel"].isString())
        {
            m_activeModelId = activeConfig["activeModel"].toString();
        }
        if (activeConfig.contains("activeProvider") &&
            activeConfig["activeProvider"].isString())
        {
            m_activeProviderId = activeConfig["activeProvider"].toString();
        }
    }
    else
    {
        QA_LOG_INFO("No key called [activeConfig] in json");
    }

    if (jsonObj.contains("modelParams") && jsonObj["modelParams"].isObject())
    {
        m_modelParams =
                ConfigModelParams::fromJson(jsonObj["modelParams"].toObject());
    }
    else
    {
        QA_LOG_INFO("No key called [modelParams] in json");
    }
}

void ConfigService::saveConfig()
{
    if (m_configFilePath.isEmpty())
        return;

    QJsonArray providersArr;
    for (const auto& p : m_providers)
    {
        providersArr.append(p.toJson());
    }

    QJsonObject activeObj;
    activeObj["provider"] = m_activeProviderId;
    activeObj["modelName"] = m_activeModelId;

    QJsonObject paramsObj = m_modelParams.toJson();

    QJsonObject rootObj;
    rootObj["providers"] = providersArr;
    rootObj["activeConfig"] = activeObj;
    rootObj["modelParams"] = paramsObj;

    if (QFile file(m_configFilePath); file.open(QIODevice::WriteOnly))
    {
        const QJsonDocument doc(rootObj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        QA_LOG_INFO("Config saved to" + m_configFilePath);
    }
    else
    {
        QA_LOG_ERR("Failed to save config file");
    }
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
