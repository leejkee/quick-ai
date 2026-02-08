//
// Created by 31305 on 2025/11/28.
//
#pragma once
#include <ConfigService/ConfigService.h>
#include "AppConfigViewModel.h"
#include "LLMConfigViewModel.h"

namespace QA::Service
{
class ConfigViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QA::Service::AppConfigViewModel* appConfVM READ appVM CONSTANT)
    Q_PROPERTY(QA::Service::LLMConfigViewModel* llmConfVM READ llmVM CONSTANT)

public:
    explicit ConfigViewModel(ConfigService* service, QObject* parent = nullptr);

    [[nodiscard]] AppConfigViewModel* appVM() const
    {
        return m_appConfVM;
    }

    [[nodiscard]] LLMConfigViewModel* llmVM() const
    {
        return m_llmConfVM;
    }

    [[nodiscard]] UserSettings draft() const{ return m_draftConfig;}

private:
    AppConfigViewModel* m_appConfVM;
    LLMConfigViewModel* m_llmConfVM;
    UserSettings m_draftConfig;
};


} // namespace QA::Service
