//
// Created by 31305 on 2026/1/26.
//
#include <SessionService/LLMRuntimeContext.h>
#include <SessionService/ModelParamsViewModel.h>

namespace QA::Service
{

ModelParamsViewModel::ModelParamsViewModel(LLMRuntimeContext* context,
                                           QObject* parent)
    : QObject(parent), m_context(context)
{
    if (m_context)
    {
        connect(m_context,
                &LLMRuntimeContext::signalModelParamsChanged,
                this,
                [this]
                {
                    Q_EMIT signalFrequencyPenaltyChanged();
                    Q_EMIT signalTemperatureChanged();
                    Q_EMIT signalTopPChanged();
                    Q_EMIT signalStreamChanged();
                    Q_EMIT signalTemperatureChanged();
                    Q_EMIT signalPresencePenaltyChanged();
                });
    }
}

double ModelParamsViewModel::getFrequencyPenalty() const noexcept
{
    return m_context ? m_context->getModelParams().frequency_penalty : 0.0;
}

int ModelParamsViewModel::getMaxTokens() const noexcept
{
    return m_context ? m_context->getModelParams().max_tokens : 0;
}

double ModelParamsViewModel::getPresencePenalty() const noexcept
{
    return m_context ? m_context->getModelParams().presence_penalty : 0.0;
}

double ModelParamsViewModel::getTemperature() const noexcept
{
    return m_context ? m_context->getModelParams().temperature : 0.0;
}

double ModelParamsViewModel::getTopP() const noexcept
{
    return m_context ? m_context->getModelParams().top_p : 0.0;
}

bool ModelParamsViewModel::getStream() const noexcept
{
    return m_context ? m_context->getModelParams().stream : false;
}

void ModelParamsViewModel::setFrequencyPenalty(const double value)
{
    if (value < -2 || value > 2)
        return;

    if (m_context)
    {
        m_context->setModelParams([value](Core::ModelParams& modelParams)
                                  { modelParams.frequency_penalty = value; });
    }
}

void ModelParamsViewModel::setMaxTokens(const int value)
{
    if (value < 0 || value > 8192)
        return;

    if (m_context)
    {
        m_context->setModelParams([value](Core::ModelParams& modelParams)
                                  { modelParams.max_tokens = value; });
    }
}

void ModelParamsViewModel::setPresencePenalty(const double value)
{
    if (value < -2 || value > 2)
        return;

    if (m_context)
    {
        m_context->setModelParams([value](Core::ModelParams& modelParams)
                                  { modelParams.presence_penalty = value; });
    }
}

void ModelParamsViewModel::setTemperature(const double value)
{
    if (value < 0.0 || value > 2.0)
        return;

    if (m_context)
    {
        m_context->setModelParams([value](Core::ModelParams& modelParams)
                                  { modelParams.temperature = value; });
    }
}

void ModelParamsViewModel::setTopP(const double value)
{
    if (value < 0.0 || value > 1.0)
        return;

    if (m_context)
    {
        m_context->setModelParams([value](Core::ModelParams& modelParams)
                                  { modelParams.top_p = value; });
    }
}

void ModelParamsViewModel::setStream(const bool value)
{
    if (m_context)
    {
        m_context->setModelParams([value](Core::ModelParams& modelParams)
                                  { modelParams.stream = value; });
    }
}

void ModelParamsViewModel::resetToDefaults() {}

} // namespace QA::Service
