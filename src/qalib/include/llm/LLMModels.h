//
// Created by 31305 on 2025/11/20.
//
#pragma once
#include <QString>
#include <QtGlobal>
namespace QA::Core
{
struct Message
{
    QString role;
    QString content;
};

struct ModelParams
{
    int frequency_penalty = 0;
    int max_tokens = 4096;
    int presence_penalty = 0;
    double temperature = 0.0;
    double top_p = 1.0;
    bool stream = false;

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
    Message message;
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
