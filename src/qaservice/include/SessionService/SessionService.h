//
// Created by 31305 on 2026/2/10.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <QThreadPool>
#include <llm/LLMConversation.h>
#include <UserSettings/UserSettings.h>

namespace QA::Service
{
class SettingsRepository;
class MessageModel;

class SessionService final : public QObject
{
    Q_OBJECT
public:
    explicit SessionService(SettingsRepository* settingsRepo,
                            QObject* parent = nullptr);

    void chatNoStreaming();

    [[nodiscard]] MessageModel* getMessageModel() const
    {
        return m_messageModel;
    };

    void pushMessage(const Core::Message& msg);

    void clearMessage();
    void setSelectedModel(const QString& model);
    void setSelectedProviderId(const QString& providerId);
    QString getSelectedProviderId() const {return m_selectedProviderId;};
    QString getSelectedModel() const {return m_selectedModel;}
    QStringList getProviderList() const;
    QStringList getModelList() const;
    QString getUrl() const;
    QString getAPIKey() const;

    Core::ModelParams getModelParams() const;

    template <typename Func>
    void setModelParams(Func&& f)
    {
        Core::ModelParams copy = m_modelParams;
        f(copy);
        if (copy != m_modelParams)
        {
            m_modelParams = copy;
            Q_EMIT signalModelParamsChanged();
        }
    }

public Q_SLOTS:
    void handleSettingsChanged();

    void handleUserChat(const QString& prompt);


Q_SIGNALS:
    void signalLLMResponse(const QString& response);

    void signalSelectedModelChanged();
    void signalSelectedProviderIdChanged();
    void signalProviderListChanged();
    void signalModelParamsChanged();

private:
    QThreadPool m_threadPool;
    Core::LLMConversation* m_conversation;

    QPointer<SettingsRepository> m_settingsRepo;
    MessageModel* m_messageModel;

    QList<Provider> m_providers;
    QString m_selectedProviderId;
    QString m_selectedModel;
    Core::ModelParams m_modelParams;
};

} // namespace QA::Service
