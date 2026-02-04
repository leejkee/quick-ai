//
// Created by 31305 on 2026/1/25.
//
#pragma once
#include <QObject>
#include <UserSettings/UserSettings.h>
namespace QA::Service
{
class ParamsConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double temperature READ getTemp WRITE setTemp NOTIFY
                       signalTempChanged)
    Q_PROPERTY(double topP READ getTopP WRITE setTopP NOTIFY signalTopPChanged)
    Q_PROPERTY(int maxTokens READ getMaxTokens WRITE setMaxTokens NOTIFY
                       signalMaxTokensChanged)
public:
    explicit ParamsConfig(const ConfigModelParams& params,
                          QObject* parent = nullptr);

    [[nodiscard]] double getTemp() const { return m_params.data.temperature; }
    void setTemp(double t);

    [[nodiscard]] double getTopP() const { return m_params.data.top_p; }
    void setTopP(double tp);

    [[nodiscard]] int getMaxTokens() const { return m_params.data.max_tokens; }
    void setMaxTokens(int tokens);

    [[nodiscard]] Core::ModelParams getParams() const { return m_params.data; }

Q_SIGNALS:
    void signalTempChanged(double t);
    void signalTopPChanged(double tp);
    void signalMaxTokensChanged(int tokens);

private:
    ConfigModelParams m_params;
};
} // namespace QA::Service
