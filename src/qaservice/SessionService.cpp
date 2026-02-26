//
// Created by 31305 on 2026/2/10.
//
#include <SessionService/LLMRuntimeContext.h>
#include <SessionService/MessageModel.h>
#include <SessionService/SessionService.h>
#include <UserSettings/SettingsRepository.h>
#include <llm/LLMClientFactory.h>
#include <qalog/Log.h>

namespace QA::Service
{

SessionService::SessionService(SettingsRepository* settingsRepo,
                               LLMRuntimeContext* runtimeContext,
                               MessageModel* messageModel,
                               QObject* parent)
    : QObject(parent)
    , m_settingsRepo(settingsRepo)
    , m_runtimeContext(runtimeContext)
    , m_messageModel(messageModel)
{
    m_threadPool.setMaxThreadCount(2);
    m_threadPool.setExpiryTimeout(-1);

    connect(m_settingsRepo,
            &SettingsRepository::signalSettingsChanged,
            m_runtimeContext,
            &LLMRuntimeContext::handleSettingsUpdate);
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
    if (!m_runtimeContext || !m_messageModel)
    {
        QA_LOG_ERR << "Runtime context or message model is null";
        return;
    }

    Core::ModelParams params = m_runtimeContext->getModelParams();
    auto context = m_messageModel->getContext();
    QString model = m_runtimeContext->getSelectedModel();
    QString apiKey = m_runtimeContext->getAPIKey();
    QString url = m_runtimeContext->getUrl();

    m_threadPool.start(
            [this, params, context, model, apiKey, url]()
            {
                Core::ModelInitData initData;
                initData.model = model;
                initData.apiKey = apiKey;
                initData.url = url;
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

void SessionService::clearMessage()
{
    if (m_messageModel)
    {
        m_messageModel->clearMessages();
    }
}

void SessionService::handleUserChat(const QString& prompt)
{
    const Core::Message uMsg{"user", prompt};
    pushMessage(uMsg);
    chatNoStreaming();
}

} // namespace QA::Service
