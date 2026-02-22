//
// Created by 31305 on 2026/1/25.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <llm/LLMModels.h>

namespace QA::Service
{
class SettingsRepository;

class ModelParamsViewModel : public QObject
{
    Q_OBJECT
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
    explicit ModelParamsViewModel(SettingsRepository* settingsRepo,
                                  QObject* parent = nullptr);

    [[nodiscard]] Core::ModelParams getParams() const;

    [[nodiscard]] double getFrequencyPenalty() const;
    [[nodiscard]] int getMaxTokens() const;
    [[nodiscard]] double getPresencePenalty() const;
    [[nodiscard]] double getTemperature() const;
    [[nodiscard]] double getTopP() const;
    [[nodiscard]] bool getStream() const;

public Q_SLOTS:
    void setFrequencyPenalty(double value);
    void setMaxTokens(int value);
    void setPresencePenalty(double value);
    void setTemperature(double value);
    void setTopP(double value);
    void setStream(bool value);

    void resetToDefaults();

Q_SIGNALS:
    void signalFrequencyPenaltyChanged(double value);
    void signalMaxTokensChanged(int value);
    void signalPresencePenaltyChanged(double value);
    void signalTemperatureChanged(double value);
    void signalTopPChanged(double value);
    void signalStreamChanged(bool value);

private:
    Core::ModelParams m_params;
    QPointer<SettingsRepository> m_settingsRepo;
};

} // namespace QA::Service
