//
// Created by 31305 on 2026/2/24.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <UserSettings/UserSettings.h>

namespace QA::Service
{
class SettingsRepository;
class LLMRuntimeContext final : public QObject
{
    Q_OBJECT
public:
    explicit LLMRuntimeContext(SettingsRepository* repo,
                               QObject* parent = nullptr);

    [[nodiscard]] QString getSelectedModel() const noexcept
    {
        return m_selectedModel;
    }
    void setSelectedModel(const QString& model);

    [[nodiscard]] QString getSelectedProviderId() const noexcept
    {
        return m_selectedProviderId;
    }
    void setSelectedProviderId(const QString& providerId);

    [[nodiscard]] QString getSystemPrompt() const noexcept
    {
        return m_systemPrompt;
    }

    [[nodiscard]] Core::ModelParams getModelParams() const noexcept
    {
        return m_modelParams;
    }
    template <typename Func>
    void setModelParams(Func&& f)
    {
        Core::ModelParams copy = m_modelParams;
        f(copy);
        if (copy != m_modelParams)
        {
            m_modelParams = copy;
            Q_EMIT signalModelParamsChanged();
        }
    }

    [[nodiscard]] QStringList getProviderList() const;
    [[nodiscard]] QStringList getModelList() const;
    [[nodiscard]] QString getUrl() const;
    [[nodiscard]] QString getAPIKey() const;

public Q_SLOTS:
    void handleSettingsUpdate();

Q_SIGNALS:
    void signalSelectedModelChanged();
    void signalSelectedProviderIdChanged();
    void signalProviderListChanged();
    void signalModelParamsChanged();

private:
    QPointer<SettingsRepository> m_repo;
    QList<Provider> m_providers;
    QString m_selectedModel;
    QString m_selectedProviderId;
    QString m_systemPrompt;
    Core::ModelParams m_modelParams;
};
} // namespace QA::Service
