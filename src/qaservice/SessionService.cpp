//
// Created by 31305 on 2026/2/10.
//
#include <SessionService/SessionService.h>
#include <llm/LLMClientFactory.h>
#include <UserSettings/SettingsRepository.h>
#include <SessionService/MessageModel.h>

namespace QA::Service
{

SessionService::SessionService(SettingsRepository* configService,
                               QObject* parent)
    : QObject(parent), m_settingsRepo(configService)
{
    const auto settings = m_settingsRepo->getSettings();
    m_modelParams = settings.m_modelParams;
    m_selectedModel = settings.m_selectedModel;
    m_selectedProviderId = settings.m_selectedProviderId;
    m_conversation =
            std::make_shared<Core::LLMConversation>(settings.m_systemPrompt);

    m_selectedProviderId = settings.m_selectedProviderId;
    m_selectedModel = settings.m_selectedModel;
    m_messageModel = new MessageModel(m_conversation, this);

    Core::PostBody body;
    body.model = m_selectedModel;
    if (auto r = getDataFromVector(settings.m_providers,
                                   [this](const Provider& p)
                                   { return p.id == m_selectedProviderId; });
        r.has_value())
    {
        body.apiKey = r.value().apiKey;
        body.url = r.value().getUrl(m_selectedModel);
    }
    m_client = Core::LLMClientFactory::createLLMClient(body);

    connect(this,
            &SessionService::signalConversationChanged,
            m_messageModel,
            &MessageModel::updateData);
}


void SessionService::pushMessage(const Core::Message& msg)
{
    m_conversation->pushMessage(msg);
    Q_EMIT signalConversationChanged();
}

Core::ChatResponseBody SessionService::chatNoStreaming()
{
    if (const auto r = m_client->noStreamingChat(m_modelParams,
                                                 m_conversation->getContext());
        r.has_value())
    {
        const auto response = r.value();
        const Core::Message rMsg{response.role, response.content};
        pushMessage(rMsg);
        return response;
    }
    return {};
}

void SessionService::clearMessage() { m_conversation->clearHistory(); }


} // namespace QA::Service
