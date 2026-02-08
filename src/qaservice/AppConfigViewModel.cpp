//
// Created by 31305 on 2025/12/24.
//
#include <ConfigService/ConfigService.h>
#include <ConfigViewModel/AppConfigViewModel.h>

namespace QA::Service
{
AppConfigViewModel::AppConfigViewModel(ConfigService* service, QObject* parent)
    : QObject(parent), m_service(service)
{
}

void AppConfigViewModel::setTheme(const QString& themeMode)
{
    m_service->setThemeMode(themeMode);
}

QString AppConfigViewModel::getTheme() const
{
    return m_service->getCurrentTheme();
}

QStringList AppConfigViewModel::getThemeList() const
{
    return m_service->getThemeList();
}
} // namespace QA::Service
