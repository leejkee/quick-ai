//
// Created by 31305 on 2026/2/19.
//
#include "AppManager.h"
#include <QCoreApplication>
#include <QQmlContext>
#include <SessionService/LLMRuntimeViewModel.h>
#include <SessionService/MessageViewModel.h>
#include <SessionService/ModelParamsViewModel.h>
#include <SessionService/SessionService.h>
#include <UserSettings/AppConfigViewModel.h>
#include <UserSettings/LLMInitViewModel.h>
#include <UserSettings/SettingsRepository.h>
#include <qalog/Log.h>

namespace QA::App
{
AppManager::AppManager(QObject* parent) : QObject(parent)
{
    m_settingsRepo = new Service::SettingsRepository({}, this);
    m_sessionService = new Service::SessionService(m_settingsRepo, this);
    m_messageViewModel = new Service::MessageViewModel(m_sessionService, this);
    m_modelParamsViewModel =
            new Service::ModelParamsViewModel(m_settingsRepo, this);
    m_llmRuntimeViewModel =
            new Service::LLMRuntimeViewModel(m_settingsRepo, this);
    m_appConfigViewModel =
            new Service::AppConfigViewModel(m_settingsRepo, this);
    m_llmInitViewModel = new Service::LLMInitViewModel(m_settingsRepo, this);

    m_qmlEngine = new QQmlApplicationEngine(this);
}

void AppManager::initApp()
{
    connect(
            m_qmlEngine,
            &QQmlApplicationEngine::objectCreated,
            this,
            [](const QObject* obj, const QUrl&)
            {
                if (!obj)
                {
                    QA_LOG_WARN << "Failed to load QML from module. Exiting.";
                    QCoreApplication::exit(-1);
                }
            },
            Qt::QueuedConnection);
    m_qmlEngine->rootContext()->setContextProperty("messageViewModel",
                                                   m_messageViewModel);
    m_qmlEngine->rootContext()->setContextProperty("modelParamsViewModel",
                                                   m_modelParamsViewModel);
    m_qmlEngine->rootContext()->setContextProperty("llmRuntimeViewModel",
                                                   m_llmRuntimeViewModel);
    m_qmlEngine->rootContext()->setContextProperty("appConfigViewModel",
                                                   m_appConfigViewModel);
    m_qmlEngine->rootContext()->setContextProperty("llmRuntimeViewModel",
                                                   m_llmInitViewModel);
    m_qmlEngine->loadFromModule("qaui.sessionwindow", "Main");
}


} // namespace QA::App
