//
// Created by 31305 on 2026/2/13.
//
#pragma once
#include <QObject>
#include <SystemTray/SystemTray.h>
class QQmlApplicationEngine;

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
class WindowManager;
class IPCManager;
class AppManager final : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject* parent = nullptr);
    ~AppManager() override;

private:
    static QString getDefaultConfigPath();
    void registerHotkey();
    void resetHotkey();
    void initSystemTray();

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

    WindowManager* m_windowManager = nullptr;
    QQmlApplicationEngine* m_qmlEngine = nullptr;
    SystemTray* m_systemTray = nullptr;
    int m_hotkeyId = 0;
    IPCManager* m_ipcManager = nullptr;
};

} // namespace QA::App
