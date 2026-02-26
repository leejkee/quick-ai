//
// Created by 31305 on 2025/12/25.
//
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <UserSettings/SettingsRepository.h>
#include <qalog/Log.h>
#include <stdexcept>

namespace QA::Service
{
SettingsRepository::SettingsRepository(const QString& filePath, QObject* parent)
    : QObject(parent), m_filePath(filePath)
{
    if (m_filePath.isEmpty())
    {
        QA_LOG_ERR << "Settings file is empty";
        throw std::invalid_argument("Settings file path cannot be empty.");
    }

    if (const QFileInfo fileInfo(m_filePath); !fileInfo.exists())
    {
        m_settings = UserSettings::createDefault();
        generateSettingsFile(filePath);
    }
    else
    {
        if (loadSettingsFromFile())
        {
            QA_LOG_INFO << "Load settings from file: " << m_filePath;
        }
        else
        {
            QA_LOG_ERR << "Failed to load settings from file: " << m_filePath;
            throw std::runtime_error(
                    "Failed to load settings file. JSON might be corrupted.");
        }
    }
}

bool SettingsRepository::loadSettingsFromFile()
{
    if (m_filePath.isEmpty())
    {
        QA_LOG_ERR << "Settings file is empty: " << m_filePath;
        return false;
    }
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QA_LOG_ERR << "Failed to open config file";
        return false;
    }
    const QByteArray jsonData = file.readAll();
    file.close();
    QJsonParseError parseError;
    const QJsonDocument jsonDoc =
            QJsonDocument::fromJson(jsonData, &parseError);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        QA_LOG_ERR << "Failed to parse JSON" << parseError.errorString();
        return false;
    }
    QA_LOG_INFO << "Parse JSON successfully";
    const QJsonObject jsonObj = jsonDoc.object();
    m_settings = UserSettings::fromJson(jsonObj);
    return true;
}

bool SettingsRepository::saveSettingsToFile()
{
    if (m_filePath.isEmpty())
    {
        QA_LOG_ERR << "Settings file is empty: " << m_filePath;
        return false;
    }

    const QJsonObject rootObj = m_settings.toJson();

    if (QFile file(m_filePath); file.open(QIODevice::WriteOnly))
    {
        const QJsonDocument doc(rootObj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        QA_LOG_INFO << "Config saved to" + m_filePath;
        return true;
    }
    QA_LOG_ERR << "Failed to save config file";
    return false;
}

void SettingsRepository::generateSettingsFile(const QString& filePath)
{
    if (filePath.isEmpty())
    {
        QA_LOG_ERR << "Config file path is empty, cannot generate settings.";
        return;
    }

    const QFileInfo fileInfo(filePath);
    if (const QDir dir = fileInfo.absoluteDir(); !dir.exists())
    {
        if (!dir.mkpath("."))
        {
            QA_LOG_ERR << "Failed to create directory: " << dir.absolutePath();
            return;
        }
    }

    if (QFile file(filePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        const QJsonObject rootObj = m_settings.toJson();
        const QJsonDocument doc(rootObj);
        if (const qint64 bytesWritten =
                    file.write(doc.toJson(QJsonDocument::Indented));
            bytesWritten == -1)
        {
            QA_LOG_ERR << "Failed to write to config file: "
                       << file.errorString();
        }
        else
        {
            QA_LOG_INFO << "Generated default settings file at: %1" << filePath;
        }
        file.close();
    }
    else
    {
        QA_LOG_ERR << "Failed to open config file for writing: " << filePath
                   << ", Error: " << file.errorString();
    }
}

} // namespace QA::Service
