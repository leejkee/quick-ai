//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <QByteArray>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QNetworkAccessManager>
#include <QObject>
#include <optional>
#include "LLMModels.h"

namespace QA::Core
{

class LLMClientBase : public QObject
{
    Q_OBJECT
public:
    explicit LLMClientBase(QObject* parent = nullptr);
    ~LLMClientBase() override;

    virtual std::optional<ChatResponseBody>
    noStreamingChat(const ModelParams& modelParams,
                    const QList<Message>& userPrompt);

protected:
    [[nodiscard]] virtual QJsonObject
    getRequestBody(const ModelParams& params,
                   const QList<Message>& userPrompt) const = 0;

    [[nodiscard]] virtual QMap<QByteArray, QByteArray>
    getRequestHeader() const = 0;

    [[nodiscard]] virtual std::optional<ChatResponseBody>
    parseResponse(const QByteArray& responseData) const = 0;

    [[nodiscard]] virtual QString getModelUrl() const = 0;

private:
    QNetworkAccessManager* m_networkManager;
};

} // namespace QA::Core
