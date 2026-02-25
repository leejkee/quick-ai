//
// Created by 31305 on 2026/1/29.
//
#include <SessionService/LLMRuntimeViewModel.h>
#include <SessionService/SessionService.h>
namespace QA::Service
{
LLMRuntimeViewModel::LLMRuntimeViewModel(SessionService* service,
                                         QObject* parent)
    : QObject(parent), m_service(service)
{
    connect(m_service,
            &SessionService::signalSelectedModelChanged,
            this,
            [this] { Q_EMIT signalSelectedModelChanged(); });
    connect(m_service,
            &SessionService::signalSelectedProviderIdChanged,
            this,
            [this] { Q_EMIT signalSelectedProviderIdChanged(); });
    connect(m_service,
            &SessionService::signalProviderListChanged,
            this,
            [this] { Q_EMIT signalProviderListChanged(); });
}

QStringList LLMRuntimeViewModel::getModelList() const
{
    return m_service->getModelList();
}

QStringList LLMRuntimeViewModel::getProviderList() const
{
    return m_service->getProviderList();
}

QString LLMRuntimeViewModel::getSelectedProviderId() const
{
    return m_service->getSelectedProviderId();
}

QString LLMRuntimeViewModel::getSelectedModel() const
{
    return m_service->getSelectedModel();
}

void LLMRuntimeViewModel::setSelectedModel(const QString& model)
{
    m_service->setSelectedModel(model);
}

void LLMRuntimeViewModel::setSelectedProviderId(const QString& providerId)
{
    m_service->setSelectedProviderId(providerId);
}

} // namespace QA::Service
