//
// Created by 31305 on 2026/2/24.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <llm/LLMModels.h>

namespace QA::Service
{
class SettingsRepository;
class LLMRuntimeContext final : public QObject
{
    Q_OBJECT
public:
    explicit LLMRuntimeContext(QObject* parent = nullptr);

    QString m_selectedModel;
    QString m_selectedProviderId;
    QString m_systemPrompt;
    Core::ModelParams m_modelParams;

    QString getAPIKey() const;
    QString getFinalUrl() const;

public Q_SLOTS:
    void handleSettingsUpdate();

private:
    QPointer<SettingsRepository> m_repo;
};
} // namespace QA::Service
