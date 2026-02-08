//
// Created by 31305 on 2025/12/20.
//
#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <llm/LLMModels.h>
#include <tuple>

namespace QA::Service
{

template <class T, class UnaryPred>
[[nodiscard]] static std::optional<T> getDataFromVector(const QList<T>& vector,
                                                        const UnaryPred& p)
{
    for (auto& data : vector)
    {
        if (p(data))
        {
            return data;
        }
    }
    return std::nullopt;
}

struct ConfigModel
{
    QString name;
    QString endpoint;

    static ConfigModel fromJson(const QJsonObject& json)
    {
        return {json["name"].toString(), json["endpoint"].toString()};
    }

    [[nodiscard]] QJsonObject toJson() const
    {
        return {{"name", name}, {"endpoint", endpoint}};
    }

    bool operator==(const ConfigModel& rhs) const
    {
        return name == rhs.name && endpoint == rhs.endpoint;
    }

    bool operator!=(const ConfigModel& rhs) const { return !(*this == rhs); }
};

struct ConfigModelParams
{
    Core::ModelParams data;

    ConfigModelParams() = default;

    explicit ConfigModelParams(const Core::ModelParams& p) : data(p) {}

    static ConfigModelParams fromJson(const QJsonObject& json)
    {
        ConfigModelParams config;
        config.data.frequency_penalty = json["frequencyPenalty"].toInt();
        config.data.max_tokens = json["maxTokens"].toInt();
        config.data.presence_penalty = json["presencePenalty"].toInt();
        config.data.temperature = json["temperature"].toDouble();
        config.data.top_p = json["topP"].toDouble();
        config.data.stream = json["stream"].toBool();
        return config;
    }

    [[nodiscard]] QJsonObject toJson() const
    {
        return {{"frequencyPenalty", data.frequency_penalty},
                {"maxTokens", data.max_tokens},
                {"presencePenalty", data.presence_penalty},
                {"temperature", data.temperature},
                {"topP", data.top_p},
                {"stream", data.stream}};
    }

    bool operator==(const ConfigModelParams& rhs) const
    {
        return data == rhs.data;
    }

    bool operator!=(const ConfigModelParams& rhs) const
    {
        return !(*this == rhs);
    }
};

struct ConfigProvider
{
    QString id;
    QString baseUrl;
    QString apiKey;
    QList<ConfigModel> models;

    static ConfigProvider fromJson(const QJsonObject& json)
    {
        ConfigProvider p;
        p.id = json["id"].toString();
        p.baseUrl = json["baseUrl"].toString();
        p.apiKey = json["apiKey"].toString();

        QJsonArray modelArray = json["models"].toArray();
        for (const auto& m : modelArray)
        {
            p.models.append(ConfigModel::fromJson(m.toObject()));
        }
        return p;
    }

    [[nodiscard]] QJsonObject toJson() const
    {
        QJsonArray modelArray;
        for (const auto& m : models)
        {
            modelArray.append(m.toJson());
        }

        return {{"id", id},
                {"baseUrl", baseUrl},
                {"apiKey", apiKey},
                {"models", modelArray}};
    }

    bool operator==(const ConfigProvider& rhs) const
    {
        return std::tie(id, baseUrl, apiKey, models) ==
                std::tie(rhs.id, rhs.baseUrl, rhs.apiKey, rhs.models);
    }

    bool operator!=(const ConfigProvider& rhs) const { return !(*this == rhs); }

    [[nodiscard]] QString getUrl(const QString& modelName) const
    {
        if (const auto r =
                    getDataFromVector(models,
                                      [&modelName](const ConfigModel& model)
                                      { return modelName == model.name; });
            r.has_value())
        {
            return baseUrl + r.value().endpoint;
        }
        return {};
    }
};

// this struct will be used to init the default provider and model.
struct ConfigActiveModel
{
    QString providerId;
    QString model;
    static ConfigActiveModel fromJson(const QJsonObject& json)
    {
        return {json["providerId"].toString(), json["model"].toString()};
    }

    [[nodiscard]] QJsonObject toJson() const
    {
        return {{"providerId", providerId}, {"model", model}};
    }

    bool operator==(const ConfigActiveModel& rhs) const
    {
        return providerId == rhs.providerId && model == rhs.model;
    }

    bool operator!=(const ConfigActiveModel& rhs) const
    {
        return !(*this == rhs);
    }
};

struct AppSettings
{
    enum class Theme
    {
        Light = 0,
        Dark
    };

    Theme theme = Theme::Light;

    [[nodiscard]] QJsonObject toJson() const
    {
        QJsonObject json;
        json["theme"] = (theme == Theme::Dark ? "Dark" : "Light");
        return json;
    }

    static AppSettings fromJson(const QJsonObject& json)
    {
        AppSettings config;
        if (json.contains("theme") && json["theme"].isString())
        {
            if (const QString themeStr = json["theme"].toString();
                themeStr.compare("Dark", Qt::CaseInsensitive) == 0)
            {
                config.theme = Theme::Dark;
            }
            else
            {
                config.theme = Theme::Light;
            }
        }
        return config;
    }

    bool operator==(const AppSettings& rhs) const
    {
        return std::tie(theme) == std::tie(rhs.theme);
    }

    bool operator!=(const AppSettings& rhs) const { return !(*this == rhs); }
};

struct LLMSettings
{
    QList<ConfigProvider> m_providers;
    ConfigModelParams m_modelParams;
    ConfigActiveModel m_activeModel;
    QString m_systemPrompt;


    bool operator==(const LLMSettings& rhs) const
    {
        return std::tie(m_providers,
                        m_modelParams,
                        m_activeModel,
                        m_systemPrompt) ==
                std::tie(rhs.m_providers,
                         rhs.m_modelParams,
                         rhs.m_activeModel,
                         rhs.m_systemPrompt);
    }

    bool operator!=(const LLMSettings& rhs) const { return !(*this == rhs); }

    [[nodiscard]] std::optional<ConfigProvider>
    findProvider(const QString& id) const
    {
        if (const auto r = getDataFromVector(m_providers,
                                             [id](const ConfigProvider& p)
                                             { return p.id == id; });
            r.has_value())
        {
            return r.value();
        }
        return std::nullopt;
    }

    [[nodiscard]] QStringList getModels(const QString& providerId) const
    {
        if (const auto r = findProvider(providerId); r.has_value())
        {
            QStringList models;
            for (const auto& m : r.value().models)
            {
                models.append(m.name);
            }
            return models;
        }
        return {};
    }

    bool setActiveProvider(const QString& id)
    {
        if (findProvider(id).has_value())
        {
            m_activeModel.providerId = id;
            return true;
        }
        return false;
    }

    bool setActiveModel(const QString& model)
    {
        const auto p = findProvider(m_activeModel.providerId).value();
        if (const auto r = getDataFromVector(p.models,
                                             [model](const ConfigModel& m)
                                             { return m.name == model; });
            r.has_value())
        {
            m_activeModel.model = r.value().name;
            return true;
        }
        return false;
    }
};


// read/write config files;
struct UserSettings
{
    AppSettings m_appSettings;
    LLMSettings m_llmSettings;
    bool operator==(const UserSettings& rhs) const
    {
        return m_appSettings == rhs.m_appSettings &&
                m_llmSettings == rhs.m_llmSettings;
    }

    bool operator!=(const UserSettings& rhs) const { return !(*this == rhs); }
};

} // namespace QA::Service
