//
// Created by 31305 on 2026/2/10.
//
#include <ChatService/SessionService.h>

namespace QA::Service
{

SessionService::SessionService(SettingsRepository* settingsRepo,
                               QObject* parent)
    : QObject(parent), m_settings(settingsRepo)
{
    const auto settings = m_settings->getSettings();
    m_selectedProviderId = settings.m_selectedProviderId;
    m_selectedModel = settings.m_selectedModel;
    m_messageModel = new MessageModel(this);
}

void SessionService::chatNoStreaming()
{
}
} // namespace QA::Service
