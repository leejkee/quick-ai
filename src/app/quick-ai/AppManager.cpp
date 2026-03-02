//
// Created by 31305 on 2026/2/19.
//
#include "AppManager.h"
#include <IPCManager/IPCManager.h>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <SessionService/LLMRuntimeContext.h>
#include <SessionService/LLMRuntimeViewModel.h>
#include <SessionService/MessageModel.h>
#include <SessionService/MessageViewModel.h>
#include <SessionService/ModelParamsViewModel.h>
#include <SessionService/SessionService.h>
#include <SystemTray/SystemTray.h>
#include <UserSettings/AppConfigViewModel.h>
#include <UserSettings/LLMInitViewModel.h>
#include <UserSettings/ProviderEditorViewModel.h>
#include <UserSettings/SettingsRepository.h>
#include <WindowManager/WindowManager.h>
#include <qalog/Log.h>

#ifdef Q_OS_WIN
#include "WinGlobalShortcut.h"
#endif

namespace QA::App {
using namespace Qt::StringLiterals;

AppManager::AppManager(QObject* parent) : QObject(parent) {
    Log::LogManager::instance().initLogger(Log::LogMode::ConsoleOnly);
    try {
        m_settingsRepo =
            new Service::SettingsRepository(getDefaultConfigPath(), this);
    } catch (const std::exception& e) {
        QA_LOG_ERR << "Fatal error during App initialization:" << e.what();
        std::exit(EXIT_FAILURE);
    }

    m_llmRuntimeContext = new Service::LLMRuntimeContext(m_settingsRepo, this);
    m_messageModel =
        new Service::MessageModel(m_llmRuntimeContext->getSystemPrompt(), this);
    m_sessionService = new Service::SessionService(
        m_settingsRepo, m_llmRuntimeContext, m_messageModel, this);
    m_messageViewModel = new Service::MessageViewModel(m_messageModel, this);
    m_modelParamsViewModel =
        new Service::ModelParamsViewModel(m_llmRuntimeContext, this);
    m_llmRuntimeViewModel =
        new Service::LLMRuntimeViewModel(m_llmRuntimeContext, this);
    m_appConfigViewModel =
        new Service::AppConfigViewModel(m_settingsRepo, this);
    m_llmInitViewModel = new Service::LLMInitViewModel(m_settingsRepo, this);
    m_providerEditorViewModel =
        new Service::ProviderEditorViewModel(m_settingsRepo, this);

    m_qmlEngine = new QQmlApplicationEngine(this);
    auto* rootContext = m_qmlEngine->rootContext();
    rootContext->setContextProperty(u"messageViewModel"_s, m_messageViewModel);
    rootContext->setContextProperty(u"modelParamsViewModel"_s,
                                    m_modelParamsViewModel);
    rootContext->setContextProperty(u"llmRuntimeViewModel"_s,
                                    m_llmRuntimeViewModel);
    rootContext->setContextProperty(u"appConfigViewModel"_s,
                                    m_appConfigViewModel);
    rootContext->setContextProperty(u"llmInitViewModel"_s, m_llmInitViewModel);
    rootContext->setContextProperty(u"providerEditorViewModel"_s,
                                    m_providerEditorViewModel);

    try {
        m_windowManager = new WindowManager(m_qmlEngine, this);
    } catch (const std::exception& e) {
        QA_LOG_ERR << "Fatal error during WindowManager initialization:"
                   << e.what();
        std::exit(EXIT_FAILURE);
    }
    m_systemTray = new SystemTray(m_windowManager, this);
    m_ipcManager = new IPCManager(IPCManager::getDefaultServerName(), this);
    if (!m_ipcManager->startListening()) {
        QA_LOG_WARN
            << "Failed to start IPC Manager, some features may not work.";
    } else {
        QA_LOG_INFO << "IPC Manager started successfully.";
        connect(m_ipcManager, &IPCManager::signalToggle, m_windowManager,
                &WindowManager::toggleWindow);
        connect(m_ipcManager, &IPCManager::signalShow, m_windowManager,
                &WindowManager::showWindow);
    }

#ifdef Q_OS_WIN
    resetHotkey();
    registerHotkey();
#endif

    connect(m_messageViewModel, &Service::MessageViewModel::signalMessageAdded,
            m_sessionService, &Service::SessionService::handleUserChat);

    connect(m_systemTray, &SystemTray::signalExitApp, qApp,
            &QCoreApplication::quit);
}

AppManager::~AppManager() { resetHotkey(); }

void AppManager::registerHotkey() {

#ifdef Q_OS_WIN
    m_hotkeyId = WinGlobalShortcut::instance().registerShortcut(
        Qt::ControlModifier | Qt::ShiftModifier, Qt::Key_Return,
        [this]() { m_windowManager->toggleWindow(); });
#endif
}

void AppManager::resetHotkey() {
#ifdef Q_OS_WIN
    if (m_hotkeyId != 0) {
        WinGlobalShortcut::instance().unregisterShortcut(m_hotkeyId);
        m_hotkeyId = 0;
    }
#endif
}

QString AppManager::getDefaultConfigPath() {
    const QString exePath = QCoreApplication::applicationFilePath();
    const QFileInfo exeInfo(exePath);
    const QString dirPath = exeInfo.absoluteDir().absolutePath();
    QString configPath = QDir(dirPath).filePath("config.json");
    QA_LOG_INFO << "Default config path: " << configPath;
    return configPath;
}

} // namespace QA::App
