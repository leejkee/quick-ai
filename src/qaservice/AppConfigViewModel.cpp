//
// Created by 31305 on 2025/12/24.
//
#include <SessionService/MessageModel.h>
#include <UserSettings/AppConfigViewModel.h>
#include <UserSettings/SettingsRepository.h>
#include <UserSettings/UserSettings.h>

namespace QA::Service
{
AppConfigViewModel::AppConfigViewModel(SettingsRepository* settingsRepo,
                                       QObject* parent)
    : QObject(parent), m_settingsRepo(settingsRepo)
{
}

QStringList AppConfigViewModel::getThemeList()
{
    return AppSettings::getAvailableThemeNames();
}

QString AppConfigViewModel::getTheme() const
{
    if (!m_settingsRepo)
        return {};
    return AppSettings::enumToString(
            m_settingsRepo->getSettings().m_appSettings.theme);
}

void AppConfigViewModel::setTheme(const QString& themeMode)
{
    if (getTheme() == themeMode)
        return;

    auto newTheme = AppSettings::stringToEnum(themeMode);

    if (m_settingsRepo)
    {
        m_settingsRepo->updateSettings([newTheme](UserSettings& s)
                                       { s.m_appSettings.theme = newTheme; });
        Q_EMIT signalThemeChanged(themeMode);
    }
}
} // namespace QA::Service
