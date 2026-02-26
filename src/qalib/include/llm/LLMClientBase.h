//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <QByteArray>
#include <QMap>
#include <QObject>
#include <optional>
#include "LLMModels.h"

namespace QA::Core
{

class LLMClientBase : public QObject
{
public:
    explicit LLMClientBase(QObject* parent = nullptr);

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
};

} // namespace QA::Core
