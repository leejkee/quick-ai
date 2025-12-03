//
// Created by 31305 on 2025/11/25.
//
#include <QALog.h>

namespace QA::Log
{

LogManager& LogManager::instance()
{
    static LogManager instance;
    return instance;
}

void LogManager::log(const LogLevel level, const QString& msg)
{
    switch (level)
    {
    case LogLevel::Debug:
        {
            qDebug() << "[DEBUG]" << msg;
            return;
        }
    case LogLevel::Info:
        {
            qInfo() << "[INFO ]" << msg;
            break;
        }
    case LogLevel::Error:
        {
            qCritical() << "[ERROR]" << msg;
            break;
        }
    case LogLevel::Warn:
        {
            qWarning() << "[WARN ]" << msg;
        }
    }
    Q_EMIT signalLogMessage(level, msg);
}


} // namespace QA::Log
