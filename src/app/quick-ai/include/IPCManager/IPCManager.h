#pragma once
#include <QObject>
#include <QLocalServer>

namespace QA::App {
class IPCManager final : public QObject
{
    Q_OBJECT
public:
    explicit IPCManager(const QString& serverName, QObject* parent = nullptr);
    ~IPCManager() override;

    bool startListening();

    static QString getDefaultServerName() {
        return "QuickAI_IPC_Server";
    } 

Q_SIGNALS:
    void signalToggle();

    void signalShow();

private Q_SLOTS:
    void handleNewConnection();

    void processParams();


private:
    QString m_serverName;
    QLocalServer m_server;
};

}