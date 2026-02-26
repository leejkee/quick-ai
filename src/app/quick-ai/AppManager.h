//
// Created by 31305 on 2026/2/13.
//
#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include <memory>


class QQmlApplicationEngine;
class QQmlComponent;
class QQuickWindow;
class QMenu;

namespace QA::Service
{
class SessionService;
class SettingsRepository;
class MessageViewModel;
class ModelParamsViewModel;
class LLMRuntimeViewModel;
class AppConfigViewModel;
class LLMInitViewModel;
class LLMRuntimeContext;
class MessageModel;
class ProviderEditorViewModel;
} // namespace QA::Service

namespace QA::App
{

class AppManager final : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject* parent = nullptr);
    ~AppManager() override;

    void initApp();

private Q_SLOTS:
    void toggleWindow();
    void hideWindow();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showSettingsWindow();
    void closeSettingsWindow();

private:
    static QString getDefaultConfigPath();
    void registerHotkey();
    void resetHotkey();
    void setupTray();
    void initMainWindow();

    Service::SessionService* m_sessionService = nullptr;
    Service::SettingsRepository* m_settingsRepo = nullptr;
    Service::LLMRuntimeContext* m_llmRuntimeContext = nullptr;
    Service::MessageModel* m_messageModel = nullptr;
    Service::MessageViewModel* m_messageViewModel = nullptr;
    Service::ModelParamsViewModel* m_modelParamsViewModel = nullptr;
    Service::LLMRuntimeViewModel* m_llmRuntimeViewModel = nullptr;
    Service::AppConfigViewModel* m_appConfigViewModel = nullptr;
    Service::LLMInitViewModel* m_llmInitViewModel = nullptr;
    Service::ProviderEditorViewModel* m_providerEditorViewModel = nullptr;

    QQmlApplicationEngine* m_qmlEngine = nullptr;
    QSystemTrayIcon* m_trayIcon = nullptr;
    std::unique_ptr<QMenu> m_trayMenu;
    QQuickWindow* m_window = nullptr;
    int m_hotkeyId = 0;

    QQuickWindow* m_settingsWindow = nullptr;
    QQmlComponent* m_settingsComponent = nullptr;
    QQmlComponent* m_chatComponent = nullptr;
};

} // namespace QA::App
