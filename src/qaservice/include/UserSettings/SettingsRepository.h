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
    explicit SettingsRepository(const QString& filePath,
                                QObject* parent = nullptr);

    [[nodiscard]] std::optional<UserSettings> loadSettingsFromFile();

    bool saveSettingsToFile();

    UserSettings getSettings() const { return m_settings; }

    template <typename Func>
    void updateSettings(Func&& modifier)
    {
        UserSettings copy = m_settings;
        modifier(copy);
        if (copy != m_settings)
        {
            m_settings = std::move(copy);
        }
    }

    void generateSettingsFile(const QString& filePath);

Q_SIGNALS:
    void signalSettingsChanged();

private:
    UserSettings m_settings;
    QString m_filePath;
};
} // namespace QA::Service
