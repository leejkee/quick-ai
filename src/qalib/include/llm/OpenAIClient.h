//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include "LLMClientBase.h"

namespace QA::Core
{

class OpenAIClient final : public LLMClientBase
{
public:
    explicit OpenAIClient(const PostBody& modelMeta, QObject* parent = nullptr);

protected:
    [[nodiscard]] QJsonObject
    getRequestBody(const ModelParams& params,
                   const QList<Message>& userPrompt) const override;

    [[nodiscard]] QMap<QByteArray, QByteArray>
    getRequestHeader() const override;

    [[nodiscard]] std::optional<ChatResponseBody>
    parseResponse(const QByteArray& responseData) const override;

    [[nodiscard]] QString getModelUrl() const override;

private:
    QString m_model;
    QString m_apiKey;
    QString m_modelUrl;
};

} // namespace QA::Core
