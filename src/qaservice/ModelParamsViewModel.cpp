//
// Created by 31305 on 2026/1/26.
//
#include <SessionService/ModelParamsViewModel.h>
#include <UserSettings/SettingsRepository.h>

namespace QA::Service
{

ModelParamsViewModel::ModelParamsViewModel(SettingsRepository* settingsRepo,
                                           QObject* parent)
    : QObject(parent), m_settingsRepo(settingsRepo)
{
    if (m_settingsRepo)
    {
        m_params = m_settingsRepo->getSettings().m_modelParams;
    }
}

Core::ModelParams ModelParamsViewModel::getParams() const { return m_params; }

int ModelParamsViewModel::getFrequencyPenalty() const
{
    return m_params.frequency_penalty;
}

int ModelParamsViewModel::getMaxTokens() const { return m_params.max_tokens; }

int ModelParamsViewModel::getPresencePenalty() const
{
    return m_params.presence_penalty;
}

double ModelParamsViewModel::getTemperature() const
{
    return m_params.temperature;
}

double ModelParamsViewModel::getTopP() const { return m_params.top_p; }

bool ModelParamsViewModel::getStream() const { return m_params.stream; }

void ModelParamsViewModel::setFrequencyPenalty(const int value)
{
    if (value < -2 || value > 2)
        return;

    if (m_params.frequency_penalty != value)
    {
        m_params.frequency_penalty = value;
        Q_EMIT signalFrequencyPenaltyChanged(value);
    }
}

void ModelParamsViewModel::setMaxTokens(const int value)
{
    if (value < 0 || value > 8192)
        return;

    if (m_params.max_tokens != value)
    {
        m_params.max_tokens = value;
        Q_EMIT signalMaxTokensChanged(value);
    }
}

void ModelParamsViewModel::setPresencePenalty(const int value)
{
    if (value < -2 || value > 2)
        return;

    if (m_params.presence_penalty != value)
    {
        m_params.presence_penalty = value;
        Q_EMIT signalPresencePenaltyChanged(value);
    }
}

void ModelParamsViewModel::setTemperature(const double value)
{
    if (value < 0.0 || value > 2.0)
        return;

    if (!qFuzzyCompare(m_params.temperature, value))
    {
        m_params.temperature = value;
        Q_EMIT signalTemperatureChanged(value);
    }
}

void ModelParamsViewModel::setTopP(const double value)
{
    if (value < 0.0 || value > 1.0)
        return;

    if (!qFuzzyCompare(m_params.top_p, value))
    {
        m_params.top_p = value;
        Q_EMIT signalTopPChanged(value);
    }
}

void ModelParamsViewModel::setStream(const bool value)
{
    if (m_params.stream != value)
    {
        m_params.stream = value;
        Q_EMIT signalStreamChanged(value);
    }
}

void ModelParamsViewModel::resetToDefaults()
{
    if (m_settingsRepo)
    {
        const auto [frequency_penalty,
                    max_tokens,
                    presence_penalty,
                    temperature,
                    top_p,
                    stream] = m_settingsRepo->getSettings().m_modelParams;

        setFrequencyPenalty(frequency_penalty);
        setMaxTokens(max_tokens);
        setPresencePenalty(presence_penalty);
        setTemperature(temperature);
        setTopP(top_p);
        setStream(stream);
    }
}

} // namespace QA::Service
