//
// Created by 31305 on 2025/11/9.
//
#include <MessageListModel/MessageListModel.h>
namespace QA::Service
{

MessageListModel::MessageListModel(QObject* parent) : QAbstractListModel(parent)
{
}

int MessageListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_messages.count());
}

QVariant MessageListModel::data(const QModelIndex& index, const int role) const
{
    if (!index.isValid() || index.row() >= m_messages.count())
    {
        return {};
    }
    const auto& [roleStr, content] = m_messages[index.row()];
    switch (role)
    {
    case RoleRole:
        return QString::fromStdString(roleStr);
    case TextRole:
        return QString::fromStdString(content);
    default:
        return {};
    }
}

QHash<int, QByteArray> MessageListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoleRole] = STR_ROLE.toUtf8();
    roles[TextRole] = STR_TEXT.toUtf8();
    return roles;
}

void MessageListModel::pushMessage(const Core::Message& message)
{
    const int newIndex = static_cast<int>(m_messages.count());
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_messages.append(message);
    endInsertRows();
}
}
