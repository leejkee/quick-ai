//
// Created by 31305 on 2026/1/29.
//
#pragma once
#include <QObject>
#include <QPointer>
namespace QA::Service
{
class LLMRuntimeContext;
class LLMRuntimeViewModel : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QStringList providerList READ getProviderList NOTIFY
                       signalProviderListChanged)
    Q_PROPERTY(QStringList modelList READ getModelList NOTIFY
                       signalSelectedProviderIdChanged)
    Q_PROPERTY(QString selectedProviderId READ getSelectedProviderId WRITE
                       setSelectedProviderId NOTIFY
                               signalSelectedProviderIdChanged)
    Q_PROPERTY(QString selectedModel READ getSelectedModel WRITE
                       setSelectedModel NOTIFY signalSelectedModelChanged)
public:
    explicit LLMRuntimeViewModel(LLMRuntimeContext* context,
                                 QObject* parent = nullptr);
    [[nodiscard]] Q_INVOKABLE QStringList getProviderList() const;
    [[nodiscard]] Q_INVOKABLE QStringList getModelList() const;
    [[nodiscard]] QString getSelectedProviderId() const;
    void setSelectedProviderId(const QString& providerId);
    [[nodiscard]] QString getSelectedModel() const;
    void setSelectedModel(const QString& model);

Q_SIGNALS:
    void signalSelectedProviderIdChanged();
    void signalSelectedModelChanged();
    void signalProviderListChanged();

private:
    QPointer<LLMRuntimeContext> m_context;
};


} // namespace QA::Service
