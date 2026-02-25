//
// Created by 31305 on 2026/1/26.
//
#include <SessionService/ModelParamsViewModel.h>
#include <SessionService/SessionService.h>

namespace QA::Service
{

ModelParamsViewModel::ModelParamsViewModel(SessionService* service,
                                           QObject* parent)
    : QObject(parent), m_service(service)
{
    connect(m_service, &SessionService::signalModelParamsChanged, this, [this]
    {
        Q_EMIT signalFrequencyPenaltyChanged();
        Q_EMIT signalTemperatureChanged();
        Q_EMIT signalTopPChanged();
        Q_EMIT signalStreamChanged();
        Q_EMIT signalTemperatureChanged();
        Q_EMIT signalPresencePenaltyChanged();
    });
}

double ModelParamsViewModel::getFrequencyPenalty() const
{
    return m_service->getModelParams().frequency_penalty;
}

int ModelParamsViewModel::getMaxTokens() const { return m_service->getModelParams().max_tokens; }

double ModelParamsViewModel::getPresencePenalty() const
{
    return m_service->getModelParams().presence_penalty;
}

double ModelParamsViewModel::getTemperature() const
{
    return m_service->getModelParams().temperature;
}

double ModelParamsViewModel::getTopP() const { return m_service->getModelParams().top_p; }

bool ModelParamsViewModel::getStream() const { return m_service->getModelParams().stream; }

void ModelParamsViewModel::setFrequencyPenalty(const double value)
{
    if (value < -2 || value > 2)
        return;

    m_service->setModelParams([value](Core::ModelParams& modelParams)
    {
        modelParams.frequency_penalty = value;
    });

}

void ModelParamsViewModel::setMaxTokens(const int value)
{
    if (value < 0 || value > 8192)
        return;

    m_service->setModelParams([value](Core::ModelParams& modelParams)
    {
        modelParams.max_tokens = value;
    });
}

void ModelParamsViewModel::setPresencePenalty(const double value)
{
    if (value < -2 || value > 2)
        return;

    m_service->setModelParams([value](Core::ModelParams& modelParams)
    {
        modelParams.presence_penalty = value;
    });
}

void ModelParamsViewModel::setTemperature(const double value)
{
    if (value < 0.0 || value > 2.0)
        return;

    m_service->setModelParams([value](Core::ModelParams& modelParams)
    {
        modelParams.temperature = value;
    });
}

void ModelParamsViewModel::setTopP(const double value)
{
    if (value < 0.0 || value > 1.0)
        return;
    m_service->setModelParams([value](Core::ModelParams& modelParams)
    {
        modelParams.top_p = value;
    });

}

void ModelParamsViewModel::setStream(const bool value)
{
    m_service->setModelParams([value](Core::ModelParams& modelParams)
    {
        modelParams.stream = value;
    });
}

void ModelParamsViewModel::resetToDefaults()
{

}

} // namespace QA::Service
