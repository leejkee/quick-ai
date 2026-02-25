//
// Created by 31305 on 2026/2/10.
//
#include <SessionService/MessageModel.h>
#include <SessionService/SessionService.h>
#include <UserSettings/SettingsRepository.h>
#include <llm/LLMClientFactory.h>
#include <qalog/Log.h>

namespace QA::Service
{

SessionService::SessionService(SettingsRepository* settingsRepo,
                               QObject* parent)
    : QObject(parent), m_settingsRepo(settingsRepo)
{
    const auto settings = m_settingsRepo->getSettings();
    m_modelParams = settings.m_modelParams;
    m_selectedProviderId = settings.m_selectedProviderId;
    m_selectedModel = settings.m_selectedModel;
    m_providers = settings.m_providers;
    m_threadPool.setMaxThreadCount(2);
    m_threadPool.setExpiryTimeout(-1);
    m_conversation = new Core::LLMConversation(settings.m_systemPrompt, this);

    m_messageModel = new MessageModel(m_conversation, this);

    connect(m_settingsRepo,
            &SettingsRepository::signalSettingsChanged,
            this,
            &SessionService::handleSettingsChanged);
}

void SessionService::pushMessage(const Core::Message& msg)
{
    if (m_messageModel)
    {
        m_messageModel->appendMessage(msg);
    }
}

void SessionService::chatNoStreaming()
{
    Core::ModelParams params = m_modelParams;
    auto context = m_conversation->getContext();
    m_threadPool.start(
            [this, params, context]()
            {
                Core::ModelInitData initData;
                initData.model = this->m_selectedModel;
                initData.apiKey = this->getAPIKey();
                initData.url = this->getUrl();
                const auto client =
                        Core::LLMClientFactory::createLLMClient(initData);
                auto r = client->noStreamingChat(params, context);
                QMetaObject::invokeMethod(
                        this,
                        [this, r]
                        {
                            if (r.has_value())
                            {
                                QA_LOG_INFO << "No-streaming chat completed "
                                               "successfully. Role: "
                                            << r->role << ", Content length: "
                                            << r->content.size();

                                const Core::Message rMsg{r->role, r->content};
                                this->pushMessage(rMsg);
                            }
                            else
                            {
                                QA_LOG_ERR << "Failed to get response from "
                                              "LLM "
                                              "client in no-streaming mode. "
                                           << "Please check network "
                                              "connection "
                                              "or API key configuration.";
                            }
                        });
            });
}

void SessionService::clearMessage() { m_conversation->clearHistory(); }

Core::ModelParams SessionService::getModelParams() const
{
    return m_modelParams;
}

void SessionService::handleSettingsChanged()
{
    const auto& settings = m_settingsRepo->getSettings();
    setSelectedModel(settings.m_selectedModel);
    setSelectedProviderId(settings.m_selectedProviderId);
    m_providers = settings.m_providers;
    Q_EMIT signalProviderListChanged();
}

void SessionService::handleUserChat(const QString& prompt)
{
    const Core::Message uMsg{"user", prompt};
    pushMessage(uMsg);
    chatNoStreaming();
}

void SessionService::setSelectedModel(const QString& model)
{
    if (m_selectedModel != model)
    {
        m_selectedModel = model;
    }
    Q_EMIT signalSelectedModelChanged();
}

void SessionService::setSelectedProviderId(const QString& providerId)
{
    if (m_selectedProviderId != providerId)
    {
        m_selectedProviderId = providerId;
    }
    Q_EMIT signalSelectedProviderIdChanged();
}

QStringList SessionService::getModelList() const
{
    const auto provider = getDataFromVector(
            m_providers,
            [this](const Provider& p) { return p.id == m_selectedProviderId; });
    if (!provider.has_value())
    {
        return {};
    }
    const auto& models = provider->models;
    QStringList modelNameList;
    for (const auto& [name, endpoint] : models)
    {
        modelNameList.append(name);
    }
    return modelNameList;
}

QStringList SessionService::getProviderList() const
{
    QStringList providerIdList;
    for (const auto& provider : m_providers)
    {
        providerIdList.append(provider.id);
    }
    return providerIdList;
}

QString SessionService::getUrl() const
{
    const auto provider = getDataFromVector(
            m_providers,
            [this](const Provider& p) { return p.id == m_selectedProviderId; });
    if (!provider.has_value())
    {
        return {};
    }

    return provider->getUrl(m_selectedModel);
}

QString SessionService::getAPIKey() const
{
    const auto provider = getDataFromVector(
            m_providers,
            [this](const Provider& p) { return p.id == m_selectedProviderId; });
    if (!provider.has_value())
    {
        return {};
    }
    return provider->apiKey;
}
} // namespace QA::Service
