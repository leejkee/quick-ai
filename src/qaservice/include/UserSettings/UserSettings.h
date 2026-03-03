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

struct Model
{
    QString name;
    QString endpoint;

    static Model fromJson(const QJsonObject& json) noexcept
    {
        Model model;
        model.name = json["name"].toString(model.name);
        model.endpoint = json["endpoint"].toString(model.endpoint);
        return model;
    }

    [[nodiscard]] QJsonObject toJson() const noexcept
    {
        return {{"name", name}, {"endpoint", endpoint}};
    }

    bool operator==(const Model& rhs) const noexcept
    {
        return name == rhs.name && endpoint == rhs.endpoint;
    }

    bool operator!=(const Model& rhs) const noexcept { return !(*this == rhs); }
};

struct Provider
{
    QString id;
    QString baseUrl;
    QString apiKey;
    QList<Model> models;

    static Provider fromJson(const QJsonObject& json)
    {
        Provider p;
        p.id = json["id"].toString(p.id);
        p.baseUrl = json["baseUrl"].toString(p.baseUrl);
        p.apiKey = json["apiKey"].toString(p.apiKey);

        if (json.contains("models"))
        {
            const QJsonArray modelArray = json["models"].toArray();
            p.models.reserve(modelArray.size());
            for (const auto& m : modelArray)
            {
                p.models.append(Model::fromJson(m.toObject()));
            }
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

    bool operator==(const Provider& rhs) const noexcept
    {
        return std::tie(id, baseUrl, apiKey, models) ==
                std::tie(rhs.id, rhs.baseUrl, rhs.apiKey, rhs.models);
    }

    bool operator!=(const Provider& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    [[nodiscard]] QString getUrl(const QString& modelName) const
    {
        if (const auto r =
                    getDataFromVector(models,
                                      [&modelName](const Model& model)
                                      { return modelName == model.name; });
            r.has_value())
        {
            return baseUrl + r.value().endpoint;
        }
        return {};
    }

    const Model* getModel(const QString& modelName) const
    {
        for (const auto& model : models)
        {
            if (model.name == modelName)
            {
                return &model;
            }
        }
        return nullptr;
    }

    QStringList getModelList() const
    {
        QStringList names;
        for (const auto& [name, endpoint] : models)
        {
            names.append(name);
        }
        return names;
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

    static const QList<QPair<Theme, QString>>& getThemeMap()
    {
        static const QList<QPair<Theme, QString>> map = {
                {Theme::Light, QStringLiteral("Light")},
                {Theme::Dark, QStringLiteral("Dark")}};
        return map;
    }

    static QStringList getAvailableThemeNames()
    {
        QStringList names;
        for (const auto& pair : getThemeMap())
        {
            names.append(pair.second);
        }
        return names;
    }

    static Theme stringToEnum(const QString& str)
    {
        for (const auto& pair : getThemeMap())
        {
            if (pair.second.compare(str, Qt::CaseInsensitive) == 0)
            {
                return pair.first;
            }
        }
        return Theme::Light;
    }

    static QString enumToString(Theme t)
    {
        for (const auto& pair : getThemeMap())
        {
            if (pair.first == t)
            {
                return pair.second;
            }
        }
        return QStringLiteral("Light");
    }

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

    bool operator==(const AppSettings& rhs) const noexcept
    {
        return std::tie(theme) == std::tie(rhs.theme);
    }

    bool operator!=(const AppSettings& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};


// read/write config files;
struct UserSettings
{
    AppSettings m_appSettings;
    QList<Provider> m_providers;
    Core::ModelParams m_modelParams;
    QString m_selectedProviderId;
    QString m_selectedModel;
    QString m_systemPrompt;

    const Provider* getProvider(const QString& providerId) const
    {
        for (const auto& provider : m_providers)
        {
            if (provider.id == providerId)
            {
                return &provider;
            }
        }
        return nullptr;
    }

    QStringList getProviderList() const
    {
        QStringList names;
        for (const auto& provider : m_providers)
        {
            names.append(provider.id);
        }
        return names;
    }

    void sanitize()
    {
        if (m_providers.isEmpty())
        {
            m_selectedProviderId.clear();
            m_selectedModel.clear();
            return;
        }

        const Provider* currentP = getProvider(m_selectedProviderId);
        if (!currentP)
        {
            currentP = &m_providers.first();
            m_selectedProviderId = currentP->id;
        }

        if (currentP->models.isEmpty())
        {
            m_selectedModel.clear();
        }
        else if (!currentP->getModel(m_selectedModel))
        {
            m_selectedModel = currentP->models.first().name;
        }
    }

    [[nodiscard]] QJsonObject toJson() const
    {
        QJsonObject root;

        root["appSettings"] = m_appSettings.toJson();
        root["modelParams"] = m_modelParams.toJson();

        QJsonArray providersArray;
        for (const auto& provider : m_providers)
        {
            providersArray.append(provider.toJson());
        }
        root["providers"] = providersArray;

        root["selectedProviderId"] = m_selectedProviderId;
        root["selectedModel"] = m_selectedModel;
        root["systemPrompt"] = m_systemPrompt;

        return root;
    }

    static UserSettings fromJson(const QJsonObject& json)
    {
        UserSettings settings;

        if (json.contains("appSettings"))
        {
            settings.m_appSettings =
                    AppSettings::fromJson(json["appSettings"].toObject());
        }

        if (json.contains("modelParams"))
        {
            settings.m_modelParams =
                    Core::ModelParams::fromJson(json["modelParams"].toObject());
        }

        if (json.contains("providers") && json["providers"].isArray())
        {
            const QJsonArray providersArray = json["providers"].toArray();
            settings.m_providers.reserve(providersArray.size());

            for (const auto& val : providersArray)
            {
                if (val.isObject())
                {
                    settings.m_providers.append(
                            Provider::fromJson(val.toObject()));
                }
            }
        }

        settings.m_selectedProviderId = json["selectedProviderId"].toString(
                settings.m_selectedProviderId);
        settings.m_selectedModel =
                json["selectedModel"].toString(settings.m_selectedModel);
        settings.m_systemPrompt =
                json["systemPrompt"].toString(settings.m_systemPrompt);

        settings.sanitize();
        return settings;
    }

    [[nodiscard]] static UserSettings createDefault()
    {
        UserSettings defaults;

        defaults.m_appSettings.theme = AppSettings::Theme::Light;

        defaults.m_modelParams.temperature = 0.7;
        defaults.m_modelParams.max_tokens = 2048;
        defaults.m_modelParams.top_p = 1.0;

        Provider defaultProvider;
        defaultProvider.id = "DeepSeek";
        defaultProvider.baseUrl = "https://api.deepseek.com";
        defaultProvider.apiKey = "";
        defaultProvider.models.reserve(2);

        Model deepseekChat;
        deepseekChat.name = "deepseek-chat";
        deepseekChat.endpoint = "/chat/completions";
        defaultProvider.models.append(deepseekChat);

        Model deepseekReasoner;
        deepseekReasoner.name = "deepseek-reasoner";
        deepseekReasoner.endpoint = "/chat/completions";
        defaultProvider.models.append(deepseekReasoner);

        defaults.m_providers.append(defaultProvider);

        defaults.m_selectedProviderId = "DeepSeek";
        defaults.m_selectedModel = "deepseek-chat";
        defaults.m_systemPrompt = "You are a helpful assistant.";

        return defaults;
    }

    bool operator==(const UserSettings& rhs) const noexcept
    {
        return m_appSettings == rhs.m_appSettings &&
                m_modelParams == rhs.m_modelParams &&
                m_providers == rhs.m_providers &&
                m_selectedProviderId == rhs.m_selectedProviderId &&
                m_selectedModel == rhs.m_selectedModel &&
                m_systemPrompt == rhs.m_systemPrompt;
    }

    bool operator!=(const UserSettings& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};

} // namespace QA::Service
