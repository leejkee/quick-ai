//
// Created by 31305 on 2025/11/23.
//
// ConfigService.h

#pragma once
#include <UserSettings/SettingsRepository.h>
#include <UserSettings/UserSettings.h>
#include <llm/LLMModels.h>

namespace QA::Service
{

class ConfigService final : public QObject
{
    Q_OBJECT
public:
    explicit ConfigService(QObject* parent = nullptr);

    // provider->panel
    [[nodiscard]] Core::ModelMeta getModelMeta() const;
    [[nodiscard]] Core::ModelParams getModelParams() const;

    [[nodiscard]] QStringList getProviders() const;
    [[nodiscard]] QString getCurrentTheme() const;
    [[nodiscard]] QStringList getThemeList() const;
    [[nodiscard]] QString getSystemPrompt() const;
    [[nodiscard]] QStringList getModels(const QString& providerId) const;
    [[nodiscard]] QString getActiveModel() const;
    [[nodiscard]] QString getActiveProvider() const;

    void setThemeMode(const QString& theme);
    void setSystemPrompt(const QString& prompt);

public Q_SLOTS:
    void setActiveModelParams(const ConfigModelParams& params);
    void setActiveModel(const QString& model);
    void setActiveProvider(const QString& provider);

Q_SIGNALS:

private:
    void readConfig();
    void saveConfig();
    void applyConfig();

    UserSettings m_settings;
    SettingsRepository m_settingsRepo;
};
} // namespace QA::Service
