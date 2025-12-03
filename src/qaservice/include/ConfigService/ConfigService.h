//
// Created by 31305 on 2025/11/23.
//
// ConfigService.h

#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <llm/llm_models.h>
#include <tuple>

namespace QA::Service
{

struct ConfigModel
{
    QString name;
    QString endpoint;

    static ConfigModel fromJson(const QJsonObject& json)
    {
        return {json["name"].toString(), json["endpoint"].toString()};
    }

    QJsonObject toJson() const
    {
        return {{"name", name}, {"endpoint", endpoint}};
    }
};

struct ConfigModelParams
{
    int frequencyPenalty = 0;
    int maxTokens = 4096;
    int presencePenalty = 0;
    double temperature = 0;
    double topP = 1;
    bool stream = false;

    static ConfigModelParams fromJson(const QJsonObject& json)
    {
        return {json["frequencyPenalty"].toInt(),
                json["maxTokens"].toInt(),
                json["presencePenalty"].toInt(),
                json["temperature"].toDouble(),
                json["topP"].toDouble(),
                json["stream"].toBool()};
    }

    QJsonObject toJson() const
    {
        return {{"frequencyPenalty", frequencyPenalty},
                {"maxTokens", maxTokens},
                {"presencePenalty", presencePenalty},
                {"temperature", temperature},
                {"topP", topP},
                {"stream", stream}};
    }

    bool operator==(const ConfigModelParams& rhs) const
    {
        return std::tie(frequencyPenalty,
                        maxTokens,
                        presencePenalty,
                        temperature,
                        topP,
                        stream) ==
                std::tie(rhs.frequencyPenalty,
                         rhs.maxTokens,
                         rhs.presencePenalty,
                         rhs.temperature,
                         rhs.topP,
                         rhs.stream);
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
    QString standard;
    QList<ConfigModel> models;

    static ConfigProvider fromJson(const QJsonObject& json)
    {
        ConfigProvider p;
        p.id = json["id"].toString();
        p.baseUrl = json["baseUrl"].toString();
        p.apiKey = json["apiKey"].toString();
        p.standard = json["standard"].toString();

        QJsonArray modelArray = json["models"].toArray();
        for (const auto& m : modelArray)
        {
            p.models.append(ConfigModel::fromJson(m.toObject()));
        }
        return p;
    }

    QJsonObject toJson() const
    {
        QJsonArray modelArray;
        for (const auto& m : models)
            modelArray.append(m.toJson());

        return {{"id", id},
                {"baseUrl", baseUrl},
                {"apiKey", apiKey},
                {"standard", standard},
                {"models", modelArray}};
    }
};


class ConfigService final : public QObject
{
    Q_OBJECT
public:
    explicit ConfigService(const QString& configFile,
                           QObject* parent = nullptr);

    [[nodiscard]] Core::ModelMeta getModelMeta() const;

    [[nodiscard]] Core::ModelParams getModelParams() const;

    template <class T, class UnaryPred>
    [[nodiscard]] static std::optional<T>
    getDataFromVector(const QList<T>& vector, UnaryPred p)
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

public Q_SLOTS:
    void setActiveModelParams(const ConfigModelParams& params);

    void setActiveModel(const QString& model);

    void setActiveProvider(const QString& provider);

Q_SIGNALS:
    void signalConfigChanged();

private:
    void readConfig();
    void saveConfig();
    void applyConfig();

    ConfigModelParams m_modelParams;
    QList<ConfigProvider> m_providers;
    QString m_activeProviderId;
    QString m_activeModelId;
    QString m_configFilePath;
};
} // namespace QA::Service
