#pragma once
#include <QObject>
#include <QPointer>
#include <WindowManager/WindowManager.h>
#include <memory>

class QSystemTrayIcon;
class QMenu;
namespace QA::App {
class WindowManager;
class SystemTray final : public QObject {
    Q_OBJECT
public:
    explicit SystemTray(WindowManager* windowManager,
                        QObject* parent = nullptr);
    ~SystemTray();

Q_SIGNALS:
    void signalToggleWindow();

    void signalShowSettingsWindow();

    void signalExitApp();

private:
    QPointer<WindowManager> m_windowManager;
    QSystemTrayIcon* m_trayIcon = nullptr;
    std::unique_ptr<QMenu> m_trayMenu;
};
} // namespace QA::App