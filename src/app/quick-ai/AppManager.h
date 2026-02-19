//
// Created by 31305 on 2026/2/13.
//
#pragma once
#include <QObject>
#include <QQmlApplicationEngine>

namespace QA::Service
{
class SessionService;
class SettingsRepository;
class MessageViewModel;
class ModelParamsViewModel;
class LLMRuntimeViewModel;
class AppConfigViewModel;
class LLMInitViewModel;
} // namespace QA::Service
namespace QA::App
{
class AppManager final : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject* parent = nullptr);

    void initApp();

private:
    Service::SessionService* m_sessionService;
    Service::SettingsRepository* m_settingsRepo;
    Service::MessageViewModel* m_messageViewModel;
    Service::ModelParamsViewModel* m_modelParamsViewModel;
    Service::LLMRuntimeViewModel* m_llmRuntimeViewModel;
    Service::AppConfigViewModel* m_appConfigViewModel;
    Service::LLMInitViewModel* m_llmInitViewModel;
    QQmlApplicationEngine* m_qmlEngine;
};

} // namespace QA::App
