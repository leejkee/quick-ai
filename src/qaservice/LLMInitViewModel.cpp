//
// Created by 31305 on 2025/12/3.
//
#include <UserSettings/LLMInitViewModel.h>
#include <UserSettings/SettingsRepository.h>

namespace QA::Service
{

LLMInitViewModel::LLMInitViewModel(SettingsRepository* settingsRepo,
                                   QObject* parent)
    : QObject(parent), m_settingsRepo(settingsRepo)
{
}

double LLMInitViewModel::getFrequencyPenalty() const
{
    if (!m_settingsRepo)
    {
        return 0;
    }
    return m_settingsRepo->getSettings().m_modelParams.frequency_penalty;
}

int LLMInitViewModel::getMaxTokens() const
{
    if (!m_settingsRepo)
    {
        return 4096;
    }
    return m_settingsRepo->getSettings().m_modelParams.max_tokens;
}

double LLMInitViewModel::getPresencePenalty() const
{
    if (!m_settingsRepo)
    {
        return 0;
    }
    return m_settingsRepo->getSettings().m_modelParams.presence_penalty;
}

QList<Provider> LLMInitViewModel::getProviderList() const
{
    if (!m_settingsRepo)
    {
        return {};
    }
    return m_settingsRepo->getSettings().m_providers;
}

QString LLMInitViewModel::getSelectedModel() const
{
    if (!m_settingsRepo)
    {
        return {};
    }
    return m_settingsRepo->getSettings().m_selectedModel;
}

QString LLMInitViewModel::getSelectedProvider() const
{
    if (!m_settingsRepo)
    {
        return {};
    }
    return m_settingsRepo->getSettings().m_selectedProviderId;
}

bool LLMInitViewModel::getStream() const
{
    if (!m_settingsRepo)
    {
        return false;
    }
    return m_settingsRepo->getSettings().m_modelParams.stream;
}

QString LLMInitViewModel::getSystemPrompt() const
{
    if (!m_settingsRepo)
    {
        return {};
    }
    return m_settingsRepo->getSettings().m_systemPrompt;
}

double LLMInitViewModel::getTemperature() const
{
    if (!m_settingsRepo)
    {
        return 0.0;
    }
    return m_settingsRepo->getSettings().m_modelParams.temperature;
}

double LLMInitViewModel::getTopP() const
{
    if (!m_settingsRepo)
    {
        return 0.0;
    }
    return m_settingsRepo->getSettings().m_modelParams.top_p;
}

void LLMInitViewModel::setFrequencyPenalty(const double value)
{
    if (m_settingsRepo && value != getFrequencyPenalty())
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_modelParams.frequency_penalty = value; });
        Q_EMIT signalFrequencyPenaltyChanged(value);
    }
}

void LLMInitViewModel::setMaxTokens(int value)
{
    if (m_settingsRepo && value != getMaxTokens())
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_modelParams.max_tokens = value; });
        Q_EMIT signalMaxTokensChanged(value);
    }
}

void LLMInitViewModel::setPresencePenalty(const double value)
{
    if (m_settingsRepo && value != getPresencePenalty())
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_modelParams.presence_penalty = value; });
        Q_EMIT signalPresencePenaltyChanged(value);
    }
}

void LLMInitViewModel::setTemperature(double value)
{
    if (m_settingsRepo && !qFuzzyCompare(value, getTemperature()))
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_modelParams.temperature = value; });
        Q_EMIT signalTemperatureChanged(value);
    }
}

void LLMInitViewModel::setTopP(double value)
{
    if (m_settingsRepo && !qFuzzyCompare(value, getTopP()))
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_modelParams.top_p = value; });
        Q_EMIT signalTopPChanged(value);
    }
}

void LLMInitViewModel::setStream(bool value)
{
    if (m_settingsRepo && value != getStream())
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_modelParams.stream = value; });
        Q_EMIT signalStreamChanged(value);
    }
}

void LLMInitViewModel::setSystemPrompt(const QString& value)
{
    if (m_settingsRepo && value != getSystemPrompt())
    {
        m_settingsRepo->updateSettings([value](UserSettings& settings)
                                       { settings.m_systemPrompt = value; });
        Q_EMIT signalSystemPromptChanged(value);
    }
}

void LLMInitViewModel::setSelectedModel(const QString& value)
{
    if (m_settingsRepo && value != getSelectedModel())
    {
        m_settingsRepo->updateSettings([value](UserSettings& settings)
                                       { settings.m_selectedModel = value; });
        Q_EMIT signalSelectedModelChanged(value);
    }
}

void LLMInitViewModel::setSelectedProvider(const QString& value)
{
    if (m_settingsRepo && value != getSelectedProvider())
    {
        m_settingsRepo->updateSettings(
                [value](UserSettings& settings)
                { settings.m_selectedProviderId = value; });
        Q_EMIT signalSelectedProviderChanged(value);
    }
}

} // namespace QA::Service
