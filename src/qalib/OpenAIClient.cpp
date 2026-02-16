//
// Created by 31305 on 2025/11/21.
//
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <llm/OpenAIClient.h>
#include <qalog/Log.h>

namespace QA::Core
{

OpenAIClient::OpenAIClient(const PostBody& modelMeta, QObject* parent)
    : LLMClientBase(parent), m_model(modelMeta.model),
      m_apiKey(modelMeta.apiKey), m_modelUrl(modelMeta.url)
{
}

QJsonObject OpenAIClient::getRequestBody(const ModelParams& params,
                                         const QList<Message>& userPrompt) const
{
    QJsonObject root = params.toJson();
    root.insert(QStringLiteral("model"), m_model);

    QJsonArray messagesArray;
    for (const auto& msg : userPrompt)
    {
        messagesArray.append(msg.toJson());
    }
    root.insert(QStringLiteral("messages"), messagesArray);

    return root;
}

QMap<QByteArray, QByteArray> OpenAIClient::getRequestHeader() const
{
    QMap<QByteArray, QByteArray> headers;
    headers.insert("Content-Type", "application/json");
    headers.insert("Authorization", "Bearer " + m_apiKey.toUtf8());
    return headers;
}

std::optional<ChatResponseBody>
OpenAIClient::parseResponse(const QByteArray& responseData) const
{
    ChatResponseBody response;
    response.role = QStringLiteral("assistant");
    QJsonParseError parseError;
    const QJsonDocument doc =
            QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        QA_LOG_WARN << "JSON Parse Exception:" << parseError.errorString();
        return std::nullopt;
    }

    if (!doc.isObject())
    {
        QA_LOG_WARN << "Response is not a JSON object";
        return std::nullopt;
    }

    const QJsonObject root = doc.object();

    if (root.contains(QStringLiteral("error")))
    {
        const QJsonObject errorObj =
                root.value(QStringLiteral("error")).toObject();
        QA_LOG_WARN << "API Error:" <<
                    errorObj.value(QStringLiteral("message")).toString();
        return std::nullopt;
    }

    if (!root.contains(QStringLiteral("choices")) ||
        root.value(QStringLiteral("choices")).toArray().isEmpty())
    {
        QA_LOG_WARN << "Parse Error: 'choices' is missing or empty.";
        return std::nullopt;
    }

    const QJsonArray choices = root.value(QStringLiteral("choices")).toArray();
    const QJsonObject firstChoice = choices.first().toObject();
    const QJsonObject messageObj =
            firstChoice.value(QStringLiteral("message")).toObject();
    response.content =
            messageObj.value(QStringLiteral("content")).toString();

    if (root.contains(QStringLiteral("usage")))
    {
        if (const QJsonObject usage =
                    root.value(QStringLiteral("usage")).toObject();
            usage.contains(QStringLiteral("total_tokens")))
        {
            response.totalTokens = usage.value(QStringLiteral("total_tokens")).toInt();
        }
    }

    return response;
}

QString OpenAIClient::getModelUrl() const { return m_modelUrl; }

} // namespace QA::Core
