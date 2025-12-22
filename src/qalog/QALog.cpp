//
// Created by 31305 on 2025/11/25.
//
#include <QALog.h>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <filesystem>
#include <iostream>

namespace QA::Log
{

LogManager& LogManager::instance()
{
    static LogManager instance;
    return instance;
}

void LogManager::initLogger(const LogMode mode, const QString& logBaseName,
                      const int bufferSize)
{
    QMutexLocker locker(&m_mutex);
    m_mode = mode;
    m_maxBufferSize = bufferSize;
    const QFileInfo fileInfo(logBaseName);
    m_baseDir = fileInfo.absolutePath();
    m_baseName = fileInfo.baseName();
    if (const QDir dir(m_baseDir); !dir.exists())
    {
        if (!dir.mkpath("."))
        {
            std::cerr << "LogManager: Failed to create log directory: "
                  << m_baseDir.toStdString() << std::endl;
        }
    }

    if (m_mode == LogMode::WriteAll)
    {
        const QString logFileName = generateLogFileName("");
        m_logFile.setFileName(logFileName);
        if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            std::cerr << "LogManager: Failed to create log file: "
                      << logFileName.toStdString() << std::endl;
        }
        else
        {
            m_outStream.setDevice(&m_logFile);
            m_outStream << "=== LOG STARTED AT "
                        << QDateTime::currentDateTime().toString(
                                   "yyyy-MM-dd HH:mm:ss")
                        << " ===\n";
        }
    }
    else if (m_mode == LogMode::WriteOnFail)
    {
        m_logBuffer.resize(m_maxBufferSize);
        m_bufferIndex = 0;
        m_bufferFull = false;
    }
    qInstallMessageHandler(LogManager::messageHandler);
}

LogManager::~LogManager()
{
    qInstallMessageHandler(nullptr);
    if (m_logFile.isOpen())
    {
        m_logFile.close();
    }
}

QString LogManager::generateLogFileName(const QString& tag) const
{
    QString timestamp =
            QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss");
    return QString("%1/%2%3_%4.log")
            .arg(m_baseDir, m_baseName, tag, timestamp);
}

void LogManager::messageHandler(const QtMsgType type,
                                const QMessageLogContext& context,
                                const QString& msg)
{
    instance().handleLogMessage(type, context, msg);
}

void LogManager::handleLogMessage(const QtMsgType type,
                                  const QMessageLogContext& context,
                                  const QString& msg)
{
    QMutexLocker locker(&m_mutex);

    QString levelStr;
    switch (type)
    {
    case QtDebugMsg:
        levelStr = "[DEBUG]";
        break;
    case QtInfoMsg:
        levelStr = "[INFO ]";
        break;
    case QtWarningMsg:
        levelStr = "[WARN ]";
        break;
    case QtCriticalMsg:
        levelStr = "[ERROR]";
        break;
    case QtFatalMsg:
        levelStr = "[FATAL]";
        break;
    }

    QString fileName = "unknown";
    if (context.file)
    {
        fileName = QString::fromStdString(
                std::filesystem::path(context.file).filename().string());
    }

    QString timeStr =
            QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    const QString formattedMessage = QString("%1 %2 [%3:%4] %5")
                                       .arg(timeStr, levelStr, fileName)
                                       .arg(context.line)
                                       .arg(msg);

    fprintf(type >= QtCriticalMsg ? stderr : stdout,
            "%s\n",
            qPrintable(formattedMessage));
    fflush(type >= QtCriticalMsg ? stderr : stdout);

    if (m_mode == LogMode::ConsoleOnly)
    {
        return;
    }

    if (m_mode == LogMode::WriteAll)
    {
        if (m_logFile.isOpen())
        {
            m_outStream << formattedMessage << "\n";
            m_outStream.flush();
        }
    }
    else if (m_mode == LogMode::WriteOnFail)
    {
        m_logBuffer[m_bufferIndex] = formattedMessage;
        m_bufferIndex++;
        if (m_bufferIndex >= m_maxBufferSize)
        {
            m_bufferIndex = 0;
            m_bufferFull = true;
        }

        if (type == QtCriticalMsg || type == QtFatalMsg)
        {
            dumpBufferToNewFile();
        }
    }
}

void LogManager::dumpBufferToNewFile()
{
    const QString crashFileName = generateLogFileName("_crash");

    QFile crashFile(crashFileName);
    if (!crashFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cerr << "LogManager: Failed to create crash dump file: "
                  << crashFileName.toStdString() << std::endl;
        return;
    }

    QTextStream stream(&crashFile);
    stream << "=== CRASH DUMP TRIGGERED ===\n";

    const int start = m_bufferFull ? m_bufferIndex : 0;
    const int count = m_bufferFull ? m_maxBufferSize : m_bufferIndex;

    for (int i = 0; i < count; ++i)
    {
        const int idx = (start + i) % m_maxBufferSize;
        stream << m_logBuffer[idx] << "\n";
    }

    stream << "=== END OF DUMP ===\n";
    crashFile.close();

    std::cerr << "Crash log saved to: " << crashFileName.toStdString()
              << std::endl;

    m_bufferIndex = 0;
    m_bufferFull = false;
}

} // namespace QA::Log
