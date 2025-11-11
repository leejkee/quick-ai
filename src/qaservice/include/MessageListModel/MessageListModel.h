//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <QAbstractListModel>
#include <QVector>
#include <llm/models.h>
namespace QA::Service
{

class MessageListModel final : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MessageListModel(QObject* parent = nullptr);

    enum MessageRoles
    {
        RoleRole = Qt::UserRole + 1,
        TextRole
    };

    inline static const auto STR_ROLE = QStringLiteral("role");

    inline static const auto STR_TEXT = QStringLiteral("text");

    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_SLOT void pushMessage(const Core::Message& message);

private:
    QVector<Core::Message> m_messages;
};
} // namespace QA::Service
