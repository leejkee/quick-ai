//
// Created by 31305 on 2025/11/25.
//
#pragma once
#include <QDebug>
#include <QFile>
#include <QMessageLogContext>
#include <QMutex>
#include <QString>
#include <QTextStream>
#include <QVector>

namespace QA::Log
{

enum class LogMode
{
    ConsoleOnly, // print log to console only
    WriteAll, // write log to logs/app_xxxxxxxx-120000.log
    WriteOnFail // write log generated when the program crashes to
                // logs/app_crash_xxxxxx-120000.log
};

class LogManager
{
public:
    static LogManager& instance();

    /**
     * @brief init log manager
     * @param mode log mode
     * @param logBaseName the path of log file (e.g. "log/app")
     * @param bufferSize Number of recent log entries to retain in memory for
     *                   WriteOnFail mode. Upon crash, this buffer is flushed
     *                   to disk to aid debugging.
     */
    void initLogger(LogMode mode = LogMode::ConsoleOnly,
                    const QString& logBaseName = "log/app",
                    int bufferSize = 10);

private:
    LogManager() = default;

    ~LogManager();

    Q_DISABLE_COPY(LogManager)

    static void messageHandler(QtMsgType type,
                               const QMessageLogContext& context,
                               const QString& msg);

    void handleLogMessage(QtMsgType type,
                          const QMessageLogContext& context,
                          const QString& msg);

    [[nodiscard]] QString generateLogFileName(const QString& tag) const;

    void dumpBufferToNewFile();

    LogMode m_mode = LogMode::ConsoleOnly;
    int m_maxBufferSize = 10;

    QString m_baseDir; // e.g., "logs/"
    QString m_baseName; // e.g., "app"

    QFile m_logFile;
    QTextStream m_outStream;
    QMutex m_mutex;

    QVector<QString> m_logBuffer;
    int m_bufferIndex = 0;
    bool m_bufferFull = false;
};

#define QA_LOG_INFO QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).info()
#define QA_LOG_WARN QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).warning()
#define QA_LOG_ERR QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).critical()
#define QA_LOG_DEBUG QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).debug()

} // namespace QA::Log
