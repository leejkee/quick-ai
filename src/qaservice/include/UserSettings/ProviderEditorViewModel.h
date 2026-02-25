//
// Created by 31305 on 2026/2/25.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <QStringList>
#include <UserSettings/SettingsRepository.h>
#include <UserSettings/UserSettings.h>

namespace QA::Service
{
class SettingsRepository;

class ProviderEditorViewModel final : public QObject
{
    Q_OBJECT

    // 1. Provider 层级 (第一个 ComboBox 及其内容)
    Q_PROPERTY(QStringList providerList READ getProviderList NOTIFY
                       signalProviderListChanged)
    Q_PROPERTY(qsizetype providerIndex READ getProviderIndex WRITE
                       setProviderIndex NOTIFY signalProviderIndexChanged)
    Q_PROPERTY(QString baseURL READ getBaseURL WRITE setBaseURL NOTIFY
                       signalBaseURLChanged)
    Q_PROPERTY(QString apiKey READ getApiKey WRITE setApiKey NOTIFY
                       signalApiKeyChanged)

    // 2. Model 层级 (第二个 ComboBox 及其内容)
    Q_PROPERTY(QStringList modelList READ getModelList NOTIFY
                       signalModelListChanged)
    Q_PROPERTY(qsizetype modelIndex READ getModelIndex WRITE setModelIndex
                       NOTIFY signalModelIndexChanged)
    Q_PROPERTY(QString modelName READ getModelName WRITE setModelName NOTIFY
                       signalModelNameChanged)
    Q_PROPERTY(QString endpoint READ getEndpoint WRITE setEndpoint NOTIFY
                       signalEndpointChanged)

    // 3. 状态控制 (用于控制 UI 上 "Save" 按钮的启用/禁用状态)
    Q_PROPERTY(
            bool isModified READ getIsModified NOTIFY signalIsModifiedChanged)

public:
    explicit ProviderEditorViewModel(SettingsRepository* repo,
                                     QObject* parent = nullptr);

    // ---- Getters ----
    [[nodiscard]] QStringList getProviderList() const;
    [[nodiscard]] qsizetype getProviderIndex() const
    {
        return m_curProviderIndex;
    }
    [[nodiscard]] QString getBaseURL() const;
    [[nodiscard]] QString getApiKey() const;

    [[nodiscard]] QStringList getModelList() const;
    [[nodiscard]] qsizetype getModelIndex() const { return m_curModelIndex; }
    [[nodiscard]] QString getModelName() const;
    [[nodiscard]] QString getEndpoint() const;

    [[nodiscard]] bool getIsModified() const { return m_isModified; }

    // ---- Setters ----
    void setProviderIndex(qsizetype index);
    void setBaseURL(const QString& url);
    void setApiKey(const QString& key);

    void setModelIndex(qsizetype index);
    void setModelName(const QString& name);
    void setEndpoint(const QString& endpoint);

    // ---- QML 调用的动作接口 ----
    Q_INVOKABLE void addProvider(const QString& id);
    Q_INVOKABLE void deleteCurrentProvider();
    Q_INVOKABLE void addModel(const QString& name);
    Q_INVOKABLE void deleteCurrentModel();

    Q_INVOKABLE void saveChanges();
    Q_INVOKABLE void discardChanges(); // 放弃修改，重新从 Repo 加载

Q_SIGNALS:
    void signalProviderListChanged();
    void signalProviderIndexChanged(qsizetype index);
    void signalBaseURLChanged();
    void signalApiKeyChanged();

    void signalModelListChanged();
    void signalModelIndexChanged(qsizetype index);
    void signalModelNameChanged();
    void signalEndpointChanged();

    void signalIsModifiedChanged();

private:
    void markModified();
    void refreshAllProviderSignals();
    void refreshAllModelSignals();

    QPointer<SettingsRepository> m_repo;

    QList<Provider> m_draftProviders;

    qsizetype m_curProviderIndex = -1;
    qsizetype m_curModelIndex = -1;
    bool m_isModified = false;
};
} // namespace QA::Service
