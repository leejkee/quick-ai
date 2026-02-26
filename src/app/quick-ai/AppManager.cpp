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
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWindow>
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

    // Initialize runtime context and message model first
    m_llmRuntimeContext = new Service::LLMRuntimeContext(m_settingsRepo, this);
    m_messageModel = new Service::MessageModel(
            m_llmRuntimeContext->getSystemPrompt(), this);

    // Inject dependencies into SessionService
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
    if (m_settingsWindow)
    {
        m_settingsWindow->deleteLater();
    }
    if (m_window)
    {
        m_window->deleteLater();
    }
}

void AppManager::initApp()
{
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

    setupTray();
    registerHotkey();

    m_settingsComponent = new QQmlComponent(
            m_qmlEngine,
            QUrl(u"qrc:/qt/qml/qaui/settingswindow/MainView.qml"_s),
            this);

    initMainWindow();
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

    connect(showAction, &QAction::triggered, this, &AppManager::toggleWindow);
    connect(settingsAction,
            &QAction::triggered,
            this,
            &AppManager::showSettingsWindow);

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
            this,
            &AppManager::onTrayIconActivated);

    m_trayIcon->setContextMenu(m_trayMenu.get());
    m_trayIcon->show();
}

void AppManager::initMainWindow()
{
    m_chatComponent = new QQmlComponent(m_qmlEngine, this);
    m_chatComponent->loadFromModule(u"qaui.sessionwindow"_s, u"Main"_s);

    if (m_chatComponent->status() == QQmlComponent::Error)
    {
        QA_LOG_WARN << "Failed to load Main QML:";
        for (const auto& err : m_chatComponent->errors())
        {
            QA_LOG_WARN << err.toString();
        }
        return;
    }


    QObject* rootObject = m_chatComponent->create();
    m_window = qobject_cast<QQuickWindow*>(rootObject);

    if (!m_window)
    {
        QA_LOG_WARN << "Root object is not a QQuickWindow";
        if (rootObject)
            rootObject->deleteLater();
        return;
    }

    m_window->setFlags(m_window->flags() | Qt::WindowStaysOnTopHint);

    connect(m_window,
            &QQuickWindow::activeFocusItemChanged,
            this,
            &AppManager::hideWindow);
}


void AppManager::toggleWindow()
{
    if (!m_window)
        return;

    if (m_window->isVisible())
    {
        m_window->hide();
    }
    else
    {
        m_window->show();
        m_window->raise();
        m_window->requestActivate();
    }
}

void AppManager::hideWindow()
{
    if (!m_window)
        return;

    if (!m_window->activeFocusItem())
    {
        m_window->hide();
    }
}

void AppManager::onTrayIconActivated(
        const QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        toggleWindow();
    }
}

void AppManager::registerHotkey()
{
#ifdef Q_OS_WIN
    m_hotkeyId = WinGlobalShortcut::instance().registerShortcut(
            Qt::ControlModifier | Qt::ShiftModifier,
            Qt::Key_Return,
            [this]() { toggleWindow(); });
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


void AppManager::showSettingsWindow()
{
    if (!m_settingsComponent)
    {
        QA_LOG_WARN << "Settings component not initialized";
        return;
    }
    if (m_settingsComponent->status() == QQmlComponent::Error)
    {
        QA_LOG_WARN << "QML Component load errors:";
        for (const QQmlError& error : m_settingsComponent->errors())
        {
            QA_LOG_WARN << error.toString();
        }
        return;
    }
    if (m_settingsWindow)
    {
        // Window already exists, bring to front
        m_settingsWindow->show();
        m_settingsWindow->raise();
        m_settingsWindow->requestActivate();
        return;
    }

    // Create window instance
    QObject* object = m_settingsComponent->create();
    m_settingsWindow = qobject_cast<QQuickWindow*>(object);

    if (!m_settingsWindow)
    {
        QA_LOG_WARN << "Failed to create settings window";
        delete object;
        return;
    }

    connect(m_settingsWindow,
            &QQuickWindow::visibleChanged,
            this,
            [this](bool visible)
            {
                if (!visible)
                {
                    closeSettingsWindow();
                }
            });

    m_settingsWindow->setTitle("Quick AI Settings");
    m_settingsWindow->show();
    m_settingsWindow->raise();
    m_settingsWindow->requestActivate();
}

void AppManager::closeSettingsWindow()
{
    if (m_settingsWindow)
    {
        m_settingsWindow->deleteLater();
        m_settingsWindow = nullptr;
    }
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
