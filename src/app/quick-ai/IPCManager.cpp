#include <IPCManager/IPCManager.h>
#include <QAbstractSocket>
#include <QLocalSocket>
#include <qalog/Log.h>
#include <qlocalsocket.h>

namespace QA::App {

IPCManager::IPCManager(const QString& serverName, QObject* parent) :
    QObject(parent), m_serverName(serverName) {
    connect(&m_server, &QLocalServer::newConnection, this,
            &IPCManager::handleNewConnection);
}

IPCManager::~IPCManager() {
    m_server.close();
    QLocalServer::removeServer(m_serverName);
}

bool IPCManager::startListening() {
    if (m_server.listen(m_serverName)) {
        return true;
    }
    if (m_server.serverError() == QAbstractSocket::AddressInUseError) {
        QLocalSocket socket;
        socket.connectToServer(m_serverName);
        if (socket.waitForConnected(200)) {
            return false;
        }
        QLocalServer::removeServer(m_serverName);
        return m_server.listen(m_serverName);
    }    
    return false;
}

void IPCManager::handleNewConnection() {
    QLocalSocket* socket = m_server.nextPendingConnection();
    if (!socket) {
        return;
    }
    connect(socket, &QLocalSocket::readyRead, this, &IPCManager::processParams);

    connect(socket, &QLocalSocket::disconnected, socket,
            &QLocalSocket::deleteLater);
}

void IPCManager::processParams() {
    auto* socket = qobject_cast<QLocalSocket*>(sender());
    if (!socket) {
        return;
    }

    const QString cmd = QString::fromUtf8(socket->readAll()).trimmed();
    QA_LOG_INFO << "IPC Received command:" << cmd;

    if (cmd == "--toggle") {
        Q_EMIT signalToggle();
    } else if (cmd == "default") {
        Q_EMIT signalShow();
    } else {
        QA_LOG_WARN << "Unknown IPC command:" << cmd;
    }

    socket->disconnectFromServer();
}

} // namespace QA::App