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
    const auto& [roleStr, content, tokens] = m_messages[index.row()];
    switch (role)
    {
    case RoleRole:
        return roleStr;
    case ContentRole:
        return content;
    case TokensRole:
        return tokens.has_value() ? tokens.value() : 0;
    default:
        return {};
    }
}

QHash<int, QByteArray> MessageListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoleRole] = STR_ROLE.toUtf8();
    roles[ContentRole] = STR_Content.toUtf8();
    roles[TokensRole] = STR_TOKENS.toUtf8();
    return roles;
}

void MessageListModel::pushMessage(const MessageBody& message)
{
    const int newIndex = static_cast<int>(m_messages.count());
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_messages.append(message);
    endInsertRows();
}
} // namespace QA::Service
