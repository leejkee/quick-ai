//
// Created by 31305 on 2026/2/13.
//
#pragma once
#include <QObject>
#include <QSystemTrayIcon>
#include <memory>
#include <qqmlapplicationengine.h>


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
class WindowManager;
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
    void setupTray();

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
    QSystemTrayIcon* m_trayIcon = nullptr;
    std::unique_ptr<QMenu> m_trayMenu;
    int m_hotkeyId = 0;
};

} // namespace QA::App
