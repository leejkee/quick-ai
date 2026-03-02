#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include <QPointer>

class QQmlApplicationEngine;
class QQmlComponent;
class QQuickWindow;

namespace QA::App{
class WindowManager final: public QObject
{
public:
    explicit WindowManager(QQmlApplicationEngine* qmlEngine, QObject* parent = nullptr);

    void initWindow();
    
    void hideWindow();

    void showWindow();

    void toggleWindow();

    void handleTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void showSettingsWindow();

    void closeSettingsWindow();

private:
    QPointer<QQmlApplicationEngine> m_qmlEngine;
    QQuickWindow* m_window = nullptr;
    QQuickWindow* m_settingsWindow = nullptr;
    QQmlComponent* m_settingsComponent = nullptr;
    QQmlComponent* m_chatComponent = nullptr;
};
}