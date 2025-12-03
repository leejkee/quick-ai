//
// Created by 31305 on 2025/11/28.
//
#pragma once
#include <ConfigViewModel/ConfigViewModel.h>
#include <ConfigService/ConfigService.h>
#include <QObject>
#include <QPointer>

namespace QA::Service
{
class ConfigViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ConfigViewModel(ConfigService* service, QObject *parent = nullptr);

private:
    QPointer<ConfigService> m_service;
};



}