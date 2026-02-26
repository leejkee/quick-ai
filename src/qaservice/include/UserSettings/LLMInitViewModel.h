//
// Created by 31305 on 2025/11/28.
//
#pragma once
#include <QPointer>
#include <UserSettings/UserSettings.h>

namespace QA::Service
{
class SettingsRepository;

class LLMInitViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QA::Service::Provider> providerList READ getProviderList
                       CONSTANT)

    Q_PROPERTY(QString systemPrompt READ getSystemPrompt WRITE setSystemPrompt
                       NOTIFY signalSystemPromptChanged)
    Q_PROPERTY(QString selectedModel READ getSelectedModel WRITE
                       setSelectedModel NOTIFY signalSelectedModelChanged)
    Q_PROPERTY(QString selectedProvider READ getSelectedProvider WRITE
                       setSelectedProvider NOTIFY signalSelectedProviderChanged)

    Q_PROPERTY(double frequencyPenalty READ getFrequencyPenalty WRITE
                       setFrequencyPenalty NOTIFY signalFrequencyPenaltyChanged)
    Q_PROPERTY(int maxTokens READ getMaxTokens WRITE setMaxTokens NOTIFY
                       signalMaxTokensChanged)
    Q_PROPERTY(double presencePenalty READ getPresencePenalty WRITE
                       setPresencePenalty NOTIFY signalPresencePenaltyChanged)
    Q_PROPERTY(double temperature READ getTemperature WRITE setTemperature
                       NOTIFY signalTemperatureChanged)
    Q_PROPERTY(double topP READ getTopP WRITE setTopP NOTIFY signalTopPChanged)
    Q_PROPERTY(bool stream READ getStream WRITE setStream NOTIFY
                       signalStreamChanged)

public:
    explicit LLMInitViewModel(SettingsRepository* settingsRepo,
                              QObject* parent = nullptr);

    [[nodiscard]] QList<Provider> getProviderList() const;
    [[nodiscard]] QString getSystemPrompt() const;
    [[nodiscard]] QString getSelectedModel() const;
    [[nodiscard]] QString getSelectedProvider() const;
    [[nodiscard]] double getFrequencyPenalty() const;
    [[nodiscard]] int getMaxTokens() const;
    [[nodiscard]] double getPresencePenalty() const;
    [[nodiscard]] double getTemperature() const;
    [[nodiscard]] double getTopP() const;
    [[nodiscard]] bool getStream() const;

    void setSystemPrompt(const QString& value);
    void setSelectedModel(const QString& value);
    void setSelectedProvider(const QString& value);
    void setFrequencyPenalty(double value);
    void setMaxTokens(int value);
    void setPresencePenalty(double value);
    void setTemperature(double value);
    void setTopP(double value);
    void setStream(bool value);

Q_SIGNALS:
    void signalFrequencyPenaltyChanged();
    void signalMaxTokensChanged();
    void signalPresencePenaltyChanged();
    void signalTemperatureChanged();
    void signalTopPChanged();
    void signalStreamChanged();
    void signalSystemPromptChanged();
    void signalSelectedModelChanged();
    void signalSelectedProviderChanged();

private:
    QPointer<SettingsRepository> m_settingsRepo;
};

} // namespace QA::Service
