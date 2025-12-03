//
// Created by 31305 on 2025/11/25.
//
#pragma once

#include <QDir>
#include <QMutex>
namespace QA::Log
{
enum class LogLevel
{
    Debug,
    Info,
    Warn,
    Error
};

class LogManager final : public QObject
{
    Q_OBJECT
public:
    static LogManager& instance();

    void log(LogLevel level, const QString& msg);

Q_SIGNALS:
    void signalLogMessage(LogLevel level, const QString& msg);

private:
    LogManager() = default;
    Q_DISABLE_COPY(LogManager)
};

#define QA_LOG_INFO(msg) Log::LogManager::instance().log(Log::LogLevel::Info, msg)
#define QA_LOG_WARN(msg) Log::LogManager::instance().log(Log::LogLevel::Warn, msg)
#define QA_LOG_ERR(msg) Log::LogManager::instance().log(Log::LogLevel::Error, msg)

} // namespace QA::Log
