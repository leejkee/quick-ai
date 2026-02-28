//
// Created by 31305 on 2026/2/19.
//
#include "AppManager.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QMenu>
#include <QQmlContext>
#include <QStyle>
#include <SessionService/LLMRuntimeContext.h>
#include <SessionService/LLMRuntimeViewModel.h>
#include <SessionService/MessageModel.h>
#include <SessionService/MessageViewModel.h>
#include <SessionService/ModelParamsViewModel.h>
#include <SessionService/SessionService.h>
#include <UserSettings/AppConfigViewModel.h>
#include <UserSettings/LLMInitViewModel.h>
#include <UserSettings/ProviderEditorViewModel.h>
#include <UserSettings/SettingsRepository.h>
#include <WindowManager/WindowManager.h>
#include <qalog/Log.h>


#ifdef Q_OS_WIN
#include "WinGlobalShortcut.h"
#endif

namespace QA::App
{
using namespace Qt::StringLiterals;

AppManager::AppManager(QObject* parent) : QObject(parent)
{
    try
    {
        m_settingsRepo =
                new Service::SettingsRepository(getDefaultConfigPath(), this);
    } catch (const std::exception& e)
    {
        QA_LOG_ERR << "Fatal error during App initialization:" << e.what();
        std::exit(EXIT_FAILURE);
    }

    m_llmRuntimeContext = new Service::LLMRuntimeContext(m_settingsRepo, this);
    m_messageModel = new Service::MessageModel(
            m_llmRuntimeContext->getSystemPrompt(), this);
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
    m_windowManager = new WindowManager(m_qmlEngine, this);
    setupTray();
#ifdef Q_OS_WIN
    resetHotkey();
    registerHotkey();
#endif
    connect(m_messageViewModel,
            &Service::MessageViewModel::signalMessageAdded,
            m_sessionService,
            &Service::SessionService::handleUserChat);
}

AppManager::~AppManager()
{
    resetHotkey();
    if (m_trayIcon)
    {
        m_trayIcon->hide();
    }
}

void AppManager::setupTray()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(
            QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    m_trayMenu = std::make_unique<QMenu>();

    QAction* showAction = m_trayMenu->addAction(u"Show Chat Window"_s);
    QAction* settingsAction = m_trayMenu->addAction(u"Quick AI Settings"_s);
    m_trayMenu->addSeparator();
    QAction* quitAction = m_trayMenu->addAction(u"Exit"_s);

    connect(showAction,
            &QAction::triggered,
            m_windowManager,
            &WindowManager::toggleWindow);

    connect(settingsAction,
            &QAction::triggered,
            m_windowManager,
            &WindowManager::showSettingsWindow);

    connect(quitAction,
            &QAction::triggered,
            this,
            [this]()
            {
                if (m_trayIcon)
                {
                    m_trayIcon->hide();
                }
                qApp->quit();
            });

    connect(m_trayIcon,
            &QSystemTrayIcon::activated,
            m_windowManager,
            &WindowManager::handleTrayIconActivated);

    m_trayIcon->setContextMenu(m_trayMenu.get());
    m_trayIcon->show();
}


void AppManager::registerHotkey()
{

#ifdef Q_OS_WIN
    m_hotkeyId = WinGlobalShortcut::instance().registerShortcut(
            Qt::ControlModifier | Qt::ShiftModifier,
            Qt::Key_Return,
            [this]() { m_windowManager->toggleWindow(); });
#endif
}

void AppManager::resetHotkey()
{
#ifdef Q_OS_WIN
    if (m_hotkeyId != 0)
    {
        WinGlobalShortcut::instance().unregisterShortcut(m_hotkeyId);
        m_hotkeyId = 0;
    }
#endif
}


QString AppManager::getDefaultConfigPath()
{
    const QString exePath = QCoreApplication::applicationFilePath();
    const QFileInfo exeInfo(exePath);
    const QString dirPath = exeInfo.absoluteDir().absolutePath();
    QString configPath = QDir(dirPath).filePath("config.json");

    QA_LOG_INFO << "Default config path: " << configPath;
    return configPath;
}

} // namespace QA::App
