//
// Created by 31305 on 2026/1/29.
//
#pragma once
#include <QObject>
#include <UserSettings/UserSettings.h>
namespace QA::Service
{
class SelectionConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList providerList READ getProviderList CONSTANT)
    Q_PROPERTY(QStringList modelList READ getModelList NOTIFY signalSelectedProviderIdChanged)
    Q_PROPERTY(QString APIKey READ getAPIKey NOTIFY signalSelectedProviderIdChanged)
    Q_PROPERTY(QString baseURL READ getBaseURL NOTIFY signalSelectedProviderIdChanged)

    Q_PROPERTY(QString selectedProviderId READ getSelectedProviderId WRITE
                       setSelectedProviderId NOTIFY
                               signalSelectedProviderIdChanged)
    Q_PROPERTY(QString selectedModel READ getSelectedModel WRITE
                       setSelectedModel NOTIFY signalSelectedModelChanged)
public:
    explicit SelectionConfig(QObject* parent = nullptr);
    [[nodiscard]] Q_INVOKABLE QStringList getProviderList() const;
    [[nodiscard]] Q_INVOKABLE QStringList getModelList() const;
    [[nodiscard]] QString getAPIKey() const;
    [[nodiscard]] QString getBaseURL() const;
    [[nodiscard]] QString getSelectedProviderId() const { return m_selectedProviderId; }
    void setSelectedProviderId(const QString& providerId);
    [[nodiscard]] QString getSelectedModel() const { return m_selectedModel; }
    void setSelectedModel(const QString& model);
    [[nodiscard]] QString getUrl() const;

Q_SIGNALS:
    void signalSelectedProviderIdChanged(const QString& providerId);
    void signalSelectedModelChanged(const QString& model);

private:
    QList<ConfigProvider> m_providers;
    QString m_selectedProviderId;
    QString m_selectedModel;

    [[nodiscard]] ConfigProvider getSelectedProvider() const;
};


} // namespace QA::Service
