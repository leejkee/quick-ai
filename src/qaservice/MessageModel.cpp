//
// Created by 31305 on 2025/11/9.
//
#include <llm/LLMConversation.h>
#include <SessionService/MessageModel.h>
namespace QA::Service
{

MessageModel::MessageModel(const QString& systemPrompt, QObject* parent)
    : QAbstractListModel(parent), m_conversation(new Core::LLMConversation(systemPrompt, this))
{
}

int MessageModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return static_cast<int>(m_conversation->getMessageSize());
}

QVariant MessageModel::data(const QModelIndex& index, const int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_conversation->getMessageSize()))
    {
        return {};
    }
    const auto& [roleStr, content] = m_conversation->at(index.row());
    switch (role)
    {
    case RoleRole:
        return roleStr;
    case ContentRole:
        return content;
    default:
        return {};
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoleRole] = STR_ROLE.toUtf8();
    roles[ContentRole] = STR_Content.toUtf8();
    return roles;
}

void MessageModel::appendMessage(const Core::Message& message)
{
    const int newIndex = static_cast<int>(m_conversation->getMessageSize());
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    m_conversation->pushMessage(message);
    endInsertRows();
}

void MessageModel::clearMessages()
{
    beginResetModel();
    m_conversation->clearHistory();
    endResetModel();
}

QList<Core::Message> MessageModel::getContext() const
{
    return m_conversation->getContext();
}

} // namespace QA::Service
