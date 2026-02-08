//
// Created by 31305 on 2025/12/24.
//

#pragma once
#include <QObject>
#include <QPointer>

namespace QA::Service
{
class ConfigViewModel;
}
namespace QA::Service
{
class ConfigService;
}
namespace QA::Service
{
class LLMConfigViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList providers READ getProviders)
    Q_PROPERTY(QString currentProvider READ getCurrentProvider WRITE
                       setCurrentProvider NOTIFY signalCurrentProviderChanged)
    Q_PROPERTY(QStringList models READ getModels NOTIFY signalModelsChanged)
    Q_PROPERTY(QString systemPrompt READ getSystemPrompt WRITE setSystemPrompt
                       NOTIFY signalSystemPromptChanged)
    Q_PROPERTY(QString activeModel READ getActiveMdodel WRITE setActiveModel
                       NOTIFY signalActiveModelChanged)
    Q_PROPERTY(QString activeProvider READ getActiveProvider WRITE
                       setActiveProvider NOTIFY signalActiveProviderChanged)
public:
    explicit LLMConfigViewModel(ConfigViewModel* parentVM,
                                QObject* parent = nullptr);

    // getter
    [[nodiscard]] QString getCurrentProvider() const;
    [[nodiscard]] QString getSystemPrompt() const;
    [[nodiscard]] QStringList getModels() const;
    [[nodiscard]] QStringList getProviders() const;
    [[nodiscard]] QString getActiveModel() const;
    [[nodiscard]] QString getActiveProvider() const;
    // setter
    void setCurrentProvider(const QString& providerId);
    void setSystemPrompt(const QString& systemPrompt);
    void setActiveModel(const QString& model);
    void setActiveProvider(const QString& providerId);

Q_SIGNALS:
    void signalCurrentProviderChanged(const QString& theme);
    void signalModelsChanged();
    void signalSystemPromptChanged();

private:
    QPointer<ConfigViewModel> m_parentVM;
    QStringList m_models;
    void refreshModels();
};
} // namespace QA::Service
