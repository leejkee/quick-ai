//
// Created by 31305 on 2026/2/13.
//
#pragma once
#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QQmlApplicationEngine>
#include <QSystemTrayIcon>
#include <QQuickWindow>

namespace QA::Service
{
class SessionService;
class SettingsRepository;
class MessageViewModel;
class ModelParamsViewModel;
class LLMRuntimeViewModel;
class AppConfigViewModel;
class LLMInitViewModel;
} // namespace QA::Service
namespace QA::App
{
class AppManager final : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit AppManager(QObject* parent = nullptr);
    ~AppManager() override;

    void initApp();

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

private Q_SLOTS:
    void toggleWindow();
    void hideWindow();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    static QString getDefaultConfigPath();
    
    void registerHotkey();
    void resetHotkey();
    void setupTrayAndWindow(QObject* rootObject);

    Service::SessionService* m_sessionService;
    Service::SettingsRepository* m_settingsRepo;
    Service::MessageViewModel* m_messageViewModel;
    Service::ModelParamsViewModel* m_modelParamsViewModel;
    Service::LLMRuntimeViewModel* m_llmRuntimeViewModel;
    Service::AppConfigViewModel* m_appConfigViewModel;
    Service::LLMInitViewModel* m_llmInitViewModel;
    QQmlApplicationEngine* m_qmlEngine;
    QSystemTrayIcon* m_trayIcon;
    QQuickWindow* m_window;
    int m_hotkeyId;
};

} // namespace QA::App
