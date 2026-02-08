//
// Created by 31305 on 2025/12/25.
//

#pragma once
#include <UserSettings/UserSettings.h>

namespace QA::Service
{
class SettingsRepository final : public QObject
{
    Q_OBJECT
public:
    explicit SettingsRepository(const QString& filePath, QObject* parent = nullptr);

    std::optional<UserSettings> loadConfig();

    void saveConfig(const UserSettings& config);

private:
    QString m_configFilePath;
};
}
