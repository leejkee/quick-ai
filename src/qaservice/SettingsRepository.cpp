//
// Created by 31305 on 2025/12/25.
//
#include <QALog.h>
#include <QFile>
#include <QJsonDocument>
#include <UserSettings/SettingsRepository.h>

namespace QA::Service
{
SettingsRepository::SettingsRepository(const QString& filePath, QObject* parent)
    : QObject(parent), m_configFilePath(filePath)
{
}

std::optional<UserSettings> SettingsRepository::loadConfig()
{
    if (m_configFilePath.isEmpty())
    {
        QA_LOG_ERR("Settings file is empty: " + m_configFilePath);
        return std::nullopt;
    }
    QFile file(m_configFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QA_LOG_ERR("Failed to open config file");
        return std::nullopt;
    }
    UserSettings settings;
    const QByteArray jsonData = file.readAll();
    file.close();
    QJsonParseError parseError;
    const QJsonDocument jsonDoc =
            QJsonDocument::fromJson(jsonData, &parseError);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        QA_LOG_ERR("Failed to parse JSON" + parseError.errorString());
        return std::nullopt;
    }
    QA_LOG_INFO("Parse JSON successfully");

    const QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj.contains("providers") && jsonObj["providers"].isArray())
    {
        QJsonArray providers = jsonObj["providers"].toArray();
        for (const auto& provider : providers)
        {
            settings.m_providers.append(
                    ConfigProvider::fromJson(provider.toObject()));
        }
    }
    else
    {
        QA_LOG_INFO("No key called [providers] in json");
    }

    if (jsonObj.contains("activeConfig") && jsonObj["activeConfig"].isObject())
    {
        settings.m_activeModel =
                ConfigActiveModel::fromJson(jsonObj["activeConfig"].toObject());
    }
    else
    {
        QA_LOG_INFO("No key called [activeConfig] in json");
    }

    if (jsonObj.contains("modelParams") && jsonObj["modelParams"].isObject())
    {
        settings.m_modelParams =
                ConfigModelParams::fromJson(jsonObj["modelParams"].toObject());
    }
    else
    {
        QA_LOG_INFO("No key called [modelParams] in json");
    }

    if (jsonObj.contains("appConfig") && jsonObj["appConfig"].isObject())
    {
        settings.m_appConfig =
                AppSettings::fromJson(jsonObj["appConfig"].toObject());
    }
    else
    {
        QA_LOG_INFO("No key called [appConfig] in json");
    }

    if (jsonObj.contains("systemPrompt") && jsonObj["systemPrompt"].isString())
    {
        settings.m_systemPrompt = jsonObj["systemPrompt"].toString();
    }
    else
    {
        QA_LOG_INFO("No key called [systemPrompt] in json");
    }

    return settings;
}

void SettingsRepository::saveConfig(const UserSettings& config)
{
    if (m_configFilePath.isEmpty())
    {
        return;
    }

    QJsonArray providersArr;
    for (const auto& p : config.m_providers)
    {
        providersArr.append(p.toJson());
    }

    QJsonObject activeObj = config.m_activeModel.toJson();

    QJsonObject paramsObj = config.m_modelParams.toJson();

    QJsonObject systemPrompt;
    systemPrompt["systemPrompt"] = config.m_systemPrompt;

    QJsonObject appObj = config.m_appConfig.toJson();

    QJsonObject rootObj;
    rootObj["providers"] = providersArr;
    rootObj["activeConfig"] = activeObj;
    rootObj["modelParams"] = paramsObj;
    rootObj["systemPrompt"] = systemPrompt;
    rootObj["appConfig"] = appObj;
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


} // namespace QA::Service
