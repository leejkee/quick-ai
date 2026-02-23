//
// Created by 31305 on 2025/12/24.
//

#pragma once
#include <QObject>
#include <QPointer>

namespace QA::Service
{
class SettingsRepository;

}

namespace QA::Service
{
class AppConfigViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList themeList READ getThemeList CONSTANT)
    Q_PROPERTY(QString theme READ getTheme WRITE setTheme NOTIFY
                       signalThemeChanged)
public:
    explicit AppConfigViewModel(SettingsRepository* settingsRepo,
                                QObject* parent = nullptr);

    [[nodiscard]] QString getTheme() const;
    [[nodiscard]] static QStringList getThemeList();

    void setTheme(const QString& themeMode);
Q_SIGNALS:
    void signalThemeChanged(const QString& theme);

private:
    QPointer<SettingsRepository> m_settingsRepo;
};
} // namespace QA::Service
