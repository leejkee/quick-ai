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

    bool loadSettingsFromFile();

    bool saveSettingsToFile();

    [[nodiscard]] const UserSettings& getSettings() const noexcept
    {
        return m_settings;
    }

    template <typename Func>
    void updateSettings(Func&& modifier)
    {
        UserSettings copy = m_settings;
        modifier(copy);
        if (copy != m_settings)
        {
            m_settings = std::move(copy);
            m_settings.sanitize();
            saveSettingsToFile();
            Q_EMIT signalSettingsChanged();
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
