//
// Created by 31305 on 2026/1/25.
//
#pragma once
#include <QObject>
#include <QPointer>
#include <llm/LLMModels.h>

namespace QA::Service
{
class LLMRuntimeContext;

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
    explicit ModelParamsViewModel(LLMRuntimeContext* context,
                                  QObject* parent = nullptr);

    [[nodiscard]] double getFrequencyPenalty() const noexcept;
    [[nodiscard]] int getMaxTokens() const noexcept;
    [[nodiscard]] double getPresencePenalty() const noexcept;
    [[nodiscard]] double getTemperature() const noexcept;
    [[nodiscard]] double getTopP() const noexcept;
    [[nodiscard]] bool getStream() const noexcept;

public Q_SLOTS:
    void setFrequencyPenalty(double value);
    void setMaxTokens(int value);
    void setPresencePenalty(double value);
    void setTemperature(double value);
    void setTopP(double value);
    void setStream(bool value);

    void resetToDefaults();

Q_SIGNALS:
    void signalFrequencyPenaltyChanged();
    void signalMaxTokensChanged();
    void signalPresencePenaltyChanged();
    void signalTemperatureChanged();
    void signalTopPChanged();
    void signalStreamChanged();

private:
    QPointer<LLMRuntimeContext> m_context;
};

} // namespace QA::Service
