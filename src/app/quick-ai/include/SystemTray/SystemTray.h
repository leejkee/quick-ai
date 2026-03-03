#pragma once
#include <QObject>

namespace QA::App {
class SystemTray : public QObject {
    Q_OBJECT
public:
    explicit SystemTray(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void showChat() { emit signalRequestShowChat(); }
    Q_INVOKABLE void showSettings() { emit signalRequestShowSettings(); }
    Q_INVOKABLE void quitApp() { emit signalExitApp(); }

    Q_INVOKABLE void handleActivated(int reason) {
        emit signalTrayActivated(reason);
    }

signals:
    void signalRequestShowChat();
    void signalRequestShowSettings();
    void signalTrayActivated(int reason);
    void signalExitApp();
};
} // namespace QA::App