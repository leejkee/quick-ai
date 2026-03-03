//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <QJsonObject>
#include <QString>
#include <QtGlobal>
namespace QA::Core
{
struct Message
{
    QString role;
    QString content;
    [[nodiscard]] QJsonObject toJson() const noexcept
    {
        return {{"role", role}, {"content", content}};
    }

    static Message fromJson(const QJsonObject& json) noexcept
    {
        Message msg;
        msg.role = json["role"].toString(msg.role);
        msg.content = json["content"].toString(msg.content);
        return msg;
    }
};

struct ModelParams
{
    double frequency_penalty = 0.0;
    int max_tokens = 4096;
    double presence_penalty = 0.0;
    double temperature = 0.0;
    double top_p = 1.0;
    bool stream = false;

    static ModelParams fromJson(const QJsonObject& json) noexcept
    {
        ModelParams p;
        p.temperature = json["temperature"].toDouble(p.temperature);
        p.top_p = json["topP"].toDouble(p.top_p);
        p.max_tokens = json["maxTokens"].toInt(p.max_tokens);
        p.frequency_penalty =
                json["frequencyPenalty"].toDouble(p.frequency_penalty);
        p.presence_penalty =
                json["presencePenalty"].toDouble(p.presence_penalty);
        p.stream = json["stream"].toBool(p.stream);
        return p;
    }

    [[nodiscard]] QJsonObject toJson() const noexcept
    {
        return {{"frequencyPenalty", frequency_penalty},
                {"maxTokens", max_tokens},
                {"presencePenalty", presence_penalty},
                {"temperature", temperature},
                {"topP", top_p},
                {"stream", stream}};
    }

    bool operator==(const ModelParams& rhs) const noexcept
    {
        return frequency_penalty == rhs.frequency_penalty &&
                max_tokens == rhs.max_tokens &&
                presence_penalty == rhs.presence_penalty &&
                temperature == rhs.temperature && top_p == rhs.top_p &&
                stream == rhs.stream;
    }

    bool operator!=(const ModelParams& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};

struct ChatResponseBody
{
    QString role;
    QString content;
    int totalTokens = 0;
};

struct ModelInitData
{
    QString model;
    QString apiKey;
    QString url;

    bool operator==(const ModelInitData& rhs) const noexcept
    {
        return model == rhs.model && apiKey == rhs.apiKey && url == rhs.url;
    }

    bool operator!=(const ModelInitData& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};

} // namespace QA::Core
