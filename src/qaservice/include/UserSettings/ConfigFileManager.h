//
// Created by 31305 on 2025/12/29.
//
#pragma once
#include <QObject>
#include "SettingsData.h"
namespace QA::Service
{
class ConfigFileManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AppTheme theme READ getTheme WRITE setTheme NOTIFY themeChanged)

    Q_PROPERTY(RuntimeParams defaultRuntimeParams READ getDefaultRuntimeParams WRITE
                    setDefaultRuntimeParams NOTIFY defaultRuntimeParamsChanged)

    Q_PROPERTY(LaunchSelection defaultLaunchSelection READ
                       getDefaultLaunchSelection WRITE setDefaultLaunchSelection
                               NOTIFY defaultLaunchSelectionChanged)

    Q_PROPERTY(QList<Provider> providers READ getProviders NOTIFY
                       providersChanged)

public:
    void loadConfigFromFile();
    void saveConfigToFile();
    void validateSettings();

public Q_SLOTS:

    void addProvider(const Provider& provider);
    void removeProvider(const QString& providerId);
    void updateProvider(const QString& providerId, const Provider& provider);

    void addModelForProvider(const QString& providerId, const Model& model);
    void removeModelForProvider(const QString& providerId, const QString& modelName);
    void updateDefaultSelection(const QString& providerId, const QString& modelId);

Q_SIGNALS:
    void themeChanged();
    void defaultRuntimeParamsChanged();
    void defaultLaunchSelectionChanged();
    void providersChanged();

private:
    AppTheme m_theme = AppTheme::Light;
    RuntimeParams m_defaultRuntimeParams;
    LaunchSelection m_defaultLaunchSelection;
    QList<Provider> m_providers;

    int getProviderIndex(const QString& providerId);
};
} // namespace QA::Service
