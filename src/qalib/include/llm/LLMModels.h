//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <QString>
#include <QtGlobal>
#include <QJsonObject>
namespace QA::Core
{
struct Message
{
    QString role;
    QString content;
    [[nodiscard]] QJsonObject toJson() const
    {
        return {{"role", role}, {"content", content}};
    }

    static Message fromJson(const QJsonObject& json)
    {
        Message msg;
        msg.role = json["role"].toString(msg.role);
        msg.content = json["content"].toString(msg.content);
        return msg;
    }
};

struct ModelParams
{
    int frequency_penalty = 0;
    int max_tokens = 4096;
    int presence_penalty = 0;
    double temperature = 0.0;
    double top_p = 1.0;
    bool stream = false;

    static ModelParams fromJson(const QJsonObject& json)
    {
        ModelParams p;
        p.temperature = json["temperature"].toDouble(p.temperature);
        p.top_p = json["topP"].toDouble(p.top_p);
        p.max_tokens = json["maxTokens"].toInt(p.max_tokens);
        p.frequency_penalty =
                json["frequencyPenalty"].toInt(p.frequency_penalty);
        p.presence_penalty = json["presencePenalty"].toInt(p.presence_penalty);
        p.stream = json["stream"].toBool(p.stream);
        return p;
    }

    [[nodiscard]] QJsonObject toJson() const
    {
        return {{"frequencyPenalty", frequency_penalty},
                {"maxTokens", max_tokens},
                {"presencePenalty", presence_penalty},
                {"temperature", temperature},
                {"topP", top_p},
                {"stream", stream}};
    }

    bool operator==(const ModelParams& rhs) const
    {
        return frequency_penalty == rhs.frequency_penalty &&
                max_tokens == rhs.max_tokens &&
                presence_penalty == rhs.presence_penalty &&
                temperature == rhs.temperature && top_p == rhs.top_p &&
                stream == rhs.stream;
    }

    bool operator!=(const ModelParams& rhs) const { return !(*this == rhs); }
};

struct ChatResponseBody
{
    QString role;
    QString content;
    int totalTokens = 0;
};

struct PostBody
{
    QString model;
    QString apiKey;
    QString url;

    bool operator==(const PostBody& rhs) const
    {
        return model == rhs.model && apiKey == rhs.apiKey && url == rhs.url;
    }

    bool operator!=(const PostBody& rhs) const { return !(*this == rhs); }
};

} // namespace QA::Core
