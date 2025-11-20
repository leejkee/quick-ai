//
// Created by 31305 on 2025/11/14.
//
#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace QA::Core
{

enum class HttpsMethod
{
    UNKNOWN,
    GET,
    POST
};

NLOHMANN_JSON_SERIALIZE_ENUM(HttpsMethod,
                             {{HttpsMethod::UNKNOWN, nullptr},
                              {HttpsMethod::GET, "GET"},
                              {HttpsMethod::POST, "POST"}})

struct ModelConfig
{
    std::string model_name;
    std::string endpoint;
    HttpsMethod method = HttpsMethod::POST;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ModelConfig, model_name, endpoint, method)
};

struct ServiceConfig
{
    std::string name;
    std::string endpoint;
    HttpsMethod method = HttpsMethod::GET;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServiceConfig, name, endpoint, method)
};

struct ProviderConfig
{
    std::string id;
    std::string base_url;
    std::string api_key;
    std::string protocol;
    std::vector<ModelConfig> models;
    std::vector<ServiceConfig> services;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            ProviderConfig, id, base_url, api_key, protocol, models, services)
};

struct AppConfig
{
    std::vector<ProviderConfig> providers;
    std::string active_provider;
    std::string active_model;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AppConfig,
                                   providers,
                                   active_provider,
                                   active_model)
};

} // namespace QA::Core
