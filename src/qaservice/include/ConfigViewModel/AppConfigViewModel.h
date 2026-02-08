//
// Created by 31305 on 2025/12/24.
//

#pragma once
#include <QObject>
#include <QPointer>

namespace QA::Service
{
class ConfigService;
}

namespace QA::Service
{
class AppConfigViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList themeList READ getThemeList)
    Q_PROPERTY(QString theme READ getTheme WRITE setTheme NOTIFY
                       signalThemeChanged)
public:
    explicit AppConfigViewModel(ConfigService* service, QObject* parent = nullptr);
    ~AppConfigViewModel() override = default;

    [[nodiscard]] QString getTheme() const;
    [[nodiscard]] QStringList getThemeList() const;

    void setTheme(const QString& themeMode);

private:
    QPointer<ConfigService> m_service;
};
}
