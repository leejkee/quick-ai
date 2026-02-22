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
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickWindow>
#include <QStyle>
#include <SessionService/LLMRuntimeViewModel.h>
#include <SessionService/MessageViewModel.h>
#include <SessionService/ModelParamsViewModel.h>
#include <SessionService/SessionService.h>
#include <UserSettings/AppConfigViewModel.h>
#include <UserSettings/LLMInitViewModel.h>
#include <UserSettings/SettingsRepository.h>
#include <qalog/Log.h>

#ifdef Q_OS_WIN
#include "WinGlobalShortcut.h"
#endif

namespace QA::App
{
AppManager::AppManager(QObject* parent) : QObject(parent)
{
    try
    {
        m_settingsRepo = new QA::Service::SettingsRepository(
                getDefaultConfigPath(), this);
    } catch (const std::exception& e)
    {
        QA_LOG_ERR << "Fatal error during App initialization:" << e.what();
        std::exit(EXIT_FAILURE);
    }
    m_settingsRepo =
            new Service::SettingsRepository(getDefaultConfigPath(), this);
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
    m_trayIcon = nullptr;
    m_window = nullptr;
    m_hotkeyId = 0;
    m_settingsWindow = nullptr;
    m_settingsComponent = nullptr;
}

AppManager::~AppManager()
{
    resetHotkey();
    qApp->removeNativeEventFilter(this);
    if (m_trayIcon)
    {
        m_trayIcon->hide();
        delete m_trayIcon;
    }
    if (m_settingsWindow)
    {
        m_settingsWindow->deleteLater();
    }
    if (m_settingsComponent)
    {
        delete m_settingsComponent;
    }
}

void AppManager::initApp()
{
    connect(
            m_qmlEngine,
            &QQmlApplicationEngine::objectCreated,
            this,
            [this](QObject* obj, const QUrl&)
            {
                if (!obj)
                {
                    QA_LOG_WARN << "Failed to load QML from module. Exiting.";
                    QCoreApplication::exit(-1);
                    return;
                }
                setupTrayAndWindow(obj);
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
    m_qmlEngine->rootContext()->setContextProperty("llmInitViewModel",
                                                   m_llmInitViewModel);

    // Preload settings window component
    m_settingsComponent = new QQmlComponent(
            m_qmlEngine,
            QUrl("qrc:/qt/qml/qaui/settingswindow/MainView.qml"),
            this);

    m_qmlEngine->loadFromModule("qaui.sessionwindow", "Main");
}


void AppManager::setupTrayAndWindow(QObject* rootObject)
{
    m_window = qobject_cast<QQuickWindow*>(rootObject);
    if (!m_window)
    {
        QA_LOG_WARN << "Root object is not a QQuickWindow";
        return;
    }

    m_window->setVisible(false);
    m_window->setFlags(m_window->flags() | Qt::WindowStaysOnTopHint);

    QObject::connect(m_window,
                     &QQuickWindow::activeFocusItemChanged,
                     this,
                     &AppManager::hideWindow);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(
            QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));

    QMenu* trayMenu = new QMenu();
    QAction* showAction = trayMenu->addAction("显示窗口");
    QAction* settingsAction = trayMenu->addAction("Quick AI Settings");
    QAction* quitAction = trayMenu->addAction("退出");

    QObject::connect(
            showAction, &QAction::triggered, this, &AppManager::toggleWindow);
    QObject::connect(settingsAction,
                     &QAction::triggered,
                     this,
                     &AppManager::showSettingsWindow);
    QObject::connect(
            quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    QObject::connect(m_trayIcon,
                     &QSystemTrayIcon::activated,
                     this,
                     &AppManager::onTrayIconActivated);

    m_trayIcon->setContextMenu(trayMenu);
    m_trayIcon->show();

    registerHotkey();
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

void AppManager::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
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

bool AppManager::nativeEventFilter(const QByteArray& eventType,
                                   void* message,
                                   qintptr* result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    Q_UNUSED(result);
    return false;
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

    // Connect close signal to cleanup
    QObject::connect(m_settingsWindow,
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
