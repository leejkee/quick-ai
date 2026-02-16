//
// Created by 31305 on 2025/11/21.
//
#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <llm/LLMClientBase.h>
#include <qalog/Log.h>

namespace QA::Core
{

LLMClientBase::LLMClientBase(QObject* parent)
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this))
{
}

LLMClientBase::~LLMClientBase() = default;

std::optional<ChatResponseBody>
LLMClientBase::noStreamingChat(const ModelParams& modelParams,
                               const QList<Message>& userPrompt)
{
    if (userPrompt.isEmpty())
    {
        return std::nullopt;
    }

    const QUrl url(getModelUrl());
    QNetworkRequest request(url);

    const QMap<QByteArray, QByteArray> headers = getRequestHeader();
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it)
    {
        request.setRawHeader(it.key(), it.value());
    }

    const QJsonObject jsonBody = getRequestBody(modelParams, userPrompt);
    const QJsonDocument doc(jsonBody);
    const QByteArray postData = doc.toJson(QJsonDocument::Compact);

    QNetworkReply* reply = m_networkManager->post(request, postData);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    std::optional<ChatResponseBody> result = std::nullopt;

    if (reply->error() != QNetworkReply::NoError)
    {
        QA_LOG_WARN << "Request failed. Error:" << reply->error();
        QA_LOG_WARN << "Error message:" << reply->errorString();
        QA_LOG_WARN << "Server response:" << reply->readAll();
    }
    else
    {
        const QByteArray responseData = reply->readAll();
        result = parseResponse(responseData);
    }

    reply->deleteLater();

    return result;
}

} // namespace QA::Core
