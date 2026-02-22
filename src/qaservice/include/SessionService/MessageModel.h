//
// Created by 31305 on 2025/11/9.
//
#pragma once
#include <QAbstractListModel>
#include <QPointer>

namespace QA::Core
{
class LLMConversation;
}
namespace QA::Service
{
class MessageModel final : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MessageModel(Core::LLMConversation* conversation,
                          QObject* parent = nullptr);

    enum MessageRoles
    {
        RoleRole = Qt::UserRole + 1,
        ContentRole,
    };

    inline static const auto STR_ROLE = QStringLiteral("role");

    inline static const auto STR_Content = QStringLiteral("content");

    // inline static const auto STR_TOKENS = QStringLiteral("tokens");

    [[nodiscard]] int rowCount(const QModelIndex& parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

public Q_SLOTS:
    void updateData();

private:
    QPointer<Core::LLMConversation> m_conversation;
};
} // namespace QA::Service
