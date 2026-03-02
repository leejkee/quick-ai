#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>
#include <SystemTray/SystemTray.h>
#include <WindowManager/WindowManager.h>
#include <qalog/Log.h>

namespace QA::App {
using namespace Qt::StringLiterals;

SystemTray::SystemTray(WindowManager* windowManager, QObject* parent) :
    QObject(parent), m_windowManager(windowManager) {
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayMenu = std::make_unique<QMenu>();
    m_trayIcon->setIcon(QIcon(":/icons/tray.svg"));
    QAction* showAction = m_trayMenu->addAction(u"Show Chat Window"_s);
    QAction* settingsAction = m_trayMenu->addAction(u"Quick AI Settings"_s);
    m_trayMenu->addSeparator();
    QAction* quitAction = m_trayMenu->addAction(u"Exit"_s);

    connect(quitAction, &QAction::triggered, this, &SystemTray::signalExitApp);

    if (m_windowManager) {
        connect(showAction, &QAction::triggered, m_windowManager,
                &WindowManager::toggleWindow);

        connect(settingsAction, &QAction::triggered, m_windowManager,
                &WindowManager::showSettingsWindow);

        connect(m_trayIcon, &QSystemTrayIcon::activated, m_windowManager,
                &WindowManager::handleTrayIconActivated);
    } else {
        QA_LOG_ERR << "WindowManager is null. Tray icon actions will not work.";
    }

    m_trayIcon->setContextMenu(m_trayMenu.get());
    m_trayIcon->show();
}

SystemTray::~SystemTray() {
    if (m_trayIcon) {
        m_trayIcon->hide();
    }
}

} // namespace QA::App