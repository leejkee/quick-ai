//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <QAbstractListModel>
#include <QVector>
#include <optional>

namespace QA::Service
{

struct MessageBody
{
    QString role;
    QString content;
    std::optional<int> tokens;
};

class MessageListModel final : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MessageListModel(QObject* parent = nullptr);

    enum MessageRoles
    {
        RoleRole = Qt::UserRole + 1,
        ContentRole,
        TokensRole
    };

    inline static const auto STR_ROLE = QStringLiteral("role");

    inline static const auto STR_Content = QStringLiteral("content");

    inline static const auto STR_TOKENS = QStringLiteral("tokens");

    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_SLOT void pushMessage(const MessageBody& message);

private:
    QVector<MessageBody> m_messages;
};
} // namespace QA::Service
