//
// Created by 31305 on 2026/1/8.
//
#pragma once
#include <QList>
#include <QMetaObject>
#include <QString>

namespace QA::Service
{
Q_NAMESPACE
enum class AppTheme
{
    Light,
    Dark,
    System
};
Q_ENUM_NS(AppTheme)

struct Model
{
    Q_GADGET
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QStringList capabilities READ getCapabilities CONSTANT)
public:
    QString name;
    QStringList capabilities;
};

struct RuntimeParams
{
    Q_GADGET
    Q_PROPERTY(int maxTokens MEMBER maxTokens)
    Q_PROPERTY(double temperature MEMBER temperature)
    Q_PROPERTY(double topP MEMBER topP)
public:
    int maxTokens = 4096;
    double temperature = 0.0;
    double topP = 0.0;
};

struct Provider
{
    Q_GADGET
    Q_PROPERTY(QString id READ getId CONSTANT)
    Q_PROPERTY(QString protocol MEMBER protocol)
    Q_PROPERTY(QString baseUrl MEMBER baseUrl)
    Q_PROPERTY(QString apiKey MEMBER apiKey)
    Q_PROPERTY(QString baseEndPointer MEMBER baseEndPointer)
    Q_PROPERTY(QList<Model> models MEMBER models)
public:
    QString protocol;
    QString baseUrl;
    QString apiKey;
    QString baseEndPointer;
    QList<Model> models;

    Provider() = default;
    explicit Provider(const QString& providerId) : m_id(providerId) {}
    [[nodiscard]] QString getId() const { return m_id; }

private:
    QString m_id;
};

struct LaunchSelection
{
    Q_GADGET
    Q_PROPERTY(QString providerId MEMBER providerId)
    Q_PROPERTY(QString modelName MEMBER modelName)
public:
    QString providerId;
    QString modelName;
};
} // namespace QA::Service
