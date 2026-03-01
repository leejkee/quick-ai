#include <WindowManager/WindowManager.h>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWindow>
#include <qalog/Log.h>
#include <stdexcept>

namespace QA::App{
using namespace Qt::StringLiterals;

WindowManager::WindowManager(QQmlApplicationEngine* qmlEngine, QObject* parent)
    : QObject(parent), m_qmlEngine(qmlEngine)
{
    if (!m_qmlEngine)
    {
        QA_LOG_ERR << "QQmlApplicationEngine pointer is null";
        throw std::invalid_argument("QQmlApplicationEngine cannot be null.");
    }
    m_settingsComponent = new QQmlComponent(
            m_qmlEngine,
            QUrl(u"qrc:/qt/qml/qaui/settingswindow/MainView.qml"_s),
            this);
    initWindow();
}

void WindowManager::initWindow()
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
            &WindowManager::hideWindow);
}

void WindowManager::hideWindow()
{
    if (!m_window)
    {
        return;
    }

    if (!m_window->activeFocusItem())
    {
        m_window->hide();
    }
}

void WindowManager::toggleWindow()
{
    if (!m_window)
    {
        return;
    }

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

void WindowManager::handleTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        toggleWindow();
    }
}

void WindowManager::showSettingsWindow(){
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

void WindowManager::closeSettingsWindow()
{
    if (m_settingsWindow)
    {
        m_settingsWindow->deleteLater();
        m_settingsWindow = nullptr;
    }
}

}