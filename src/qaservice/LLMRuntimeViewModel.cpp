//
// Created by 31305 on 2026/1/29.
//
#include <SessionService/LLMRuntimeContext.h>
#include <SessionService/LLMRuntimeViewModel.h>
namespace QA::Service
{
LLMRuntimeViewModel::LLMRuntimeViewModel(LLMRuntimeContext* context,
                                         QObject* parent)
    : QObject(parent), m_context(context)
{
    connect(m_context,
            &LLMRuntimeContext::signalSelectedModelChanged,
            this,
            [this] { Q_EMIT signalSelectedModelChanged(); });
    connect(m_context,
            &LLMRuntimeContext::signalSelectedProviderIdChanged,
            this,
            [this] { Q_EMIT signalSelectedProviderIdChanged(); });
    connect(m_context,
            &LLMRuntimeContext::signalProviderListChanged,
            this,
            [this] { Q_EMIT signalProviderListChanged(); });
}

QStringList LLMRuntimeViewModel::getModelList() const
{
    return m_context ? m_context->getModelList() : QStringList{};
}

QStringList LLMRuntimeViewModel::getProviderList() const
{
    return m_context ? m_context->getProviderList() : QStringList{};
}

QString LLMRuntimeViewModel::getSelectedProviderId() const
{
    return m_context ? m_context->getSelectedProviderId() : QString{};
}

QString LLMRuntimeViewModel::getSelectedModel() const
{
    return m_context ? m_context->getSelectedModel() : QString{};
}

void LLMRuntimeViewModel::setSelectedModel(const QString& model)
{
    if (m_context)
    {
        m_context->setSelectedModel(model);
    }
}

void LLMRuntimeViewModel::setSelectedProviderId(const QString& providerId)
{
    if (m_context)
    {
        m_context->setSelectedProviderId(providerId);
    }
}

} // namespace QA::Service
