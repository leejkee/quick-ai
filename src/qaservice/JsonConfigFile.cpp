//
// Created by 31305 on 2025/12/29.
//

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <UserSettings/JsonConfigFile.h>
#include <QALog.h>

namespace QA::Service
{
std::optional<QJsonObject> JsonConfigFile::loadFromFile(const QString& filePath)
{
    if (filePath.isEmpty())
    {
        QA_LOG_ERR("Settings file is empty: " + filePath);
        return std::nullopt;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QA_LOG_ERR("Failed to open config file");
        return std::nullopt;
    }
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
    return jsonDoc.object();
}

bool JsonConfigFile::saveToFile(const QString& filePath,
                                const QJsonObject& jsonObj)
{
    if (QFile file(filePath); file.open(QIODevice::WriteOnly))
    {
        const QJsonDocument doc(jsonObj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        QA_LOG_INFO("Config saved to" + filePath);
        return true;
    }
    QA_LOG_ERR("Failed to save config file");
    return false;
}


}
