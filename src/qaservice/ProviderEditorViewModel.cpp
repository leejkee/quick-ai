//
// Created by 31305 on 2026/2/25.
//
#include <UserSettings/ProviderEditorViewModel.h>

namespace QA::Service
{

ProviderEditorViewModel::ProviderEditorViewModel(SettingsRepository* repo,
                                                 QObject* parent)
    : QObject(parent), m_repo(repo)
{
    discardChanges();
}

// ==================== Getters ====================

QStringList ProviderEditorViewModel::getProviderList() const
{
    QStringList list;
    for (const auto& p : m_draftProviders)
    {
        list.append(p.id);
    }
    return list;
}

QString ProviderEditorViewModel::getBaseURL() const
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
        return m_draftProviders[m_curProviderIndex].baseUrl;
    return {};
}

QString ProviderEditorViewModel::getApiKey() const
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
        return m_draftProviders[m_curProviderIndex].apiKey;
    return {};
}

QStringList ProviderEditorViewModel::getModelList() const
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
        return m_draftProviders[m_curProviderIndex].getModelList();
    return {};
}

QString ProviderEditorViewModel::getModelName() const
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
    {
        const auto& models = m_draftProviders[m_curProviderIndex].models;
        if (m_curModelIndex >= 0 && m_curModelIndex < models.size())
        {
            return models[m_curModelIndex].name;
        }
    }
    return {};
}

QString ProviderEditorViewModel::getEndpoint() const
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
    {
        const auto& models = m_draftProviders[m_curProviderIndex].models;
        if (m_curModelIndex >= 0 && m_curModelIndex < models.size())
        {
            return models[m_curModelIndex].endpoint;
        }
    }
    return {};
}

// ==================== Setters ====================

void ProviderEditorViewModel::setProviderIndex(qsizetype index)
{
    if (m_curProviderIndex != index)
    {
        m_curProviderIndex = index;
        Q_EMIT signalProviderIndexChanged(m_curProviderIndex);

        // 选中了一个新的 Provider，底下的 Model 索引必须重置
        const QStringList models = getModelList();
        setModelIndex(models.isEmpty() ? -1 : 0);

        refreshAllProviderSignals();
    }
}

void ProviderEditorViewModel::setModelIndex(qsizetype index)
{
    if (m_curModelIndex != index)
    {
        m_curModelIndex = index;
        Q_EMIT signalModelIndexChanged(m_curModelIndex);
        refreshAllModelSignals();
    }
}

void ProviderEditorViewModel::setBaseURL(const QString& url)
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
    {
        if (m_draftProviders[m_curProviderIndex].baseUrl != url)
        {
            m_draftProviders[m_curProviderIndex].baseUrl = url;
            Q_EMIT signalBaseURLChanged();
            markModified();
        }
    }
}

void ProviderEditorViewModel::setApiKey(const QString& key)
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
    {
        if (m_draftProviders[m_curProviderIndex].apiKey != key)
        {
            m_draftProviders[m_curProviderIndex].apiKey = key;
            Q_EMIT signalApiKeyChanged();
            markModified();
        }
    }
}

void ProviderEditorViewModel::setModelName(const QString& name)
{
    if (m_curProviderIndex >= 0 && m_curModelIndex >= 0)
    {
        auto& models = m_draftProviders[m_curProviderIndex].models;
        if (m_curModelIndex < models.size() &&
            models[m_curModelIndex].name != name)
        {
            models[m_curModelIndex].name = name;
            Q_EMIT signalModelNameChanged();
            Q_EMIT signalModelListChanged(); // 名字改了，ComboBox
                                             // 的列表也要刷新
            markModified();
        }
    }
}

void ProviderEditorViewModel::setEndpoint(const QString& endpoint)
{
    if (m_curProviderIndex >= 0 && m_curModelIndex >= 0)
    {
        auto& models = m_draftProviders[m_curProviderIndex].models;
        if (m_curModelIndex < models.size() &&
            models[m_curModelIndex].endpoint != endpoint)
        {
            models[m_curModelIndex].endpoint = endpoint;
            Q_EMIT signalEndpointChanged();
            markModified();
        }
    }
}

// ==================== 增删改查动作 ====================

void ProviderEditorViewModel::addProvider(const QString& id)
{
    Provider p;
    p.id = id;
    m_draftProviders.append(p);

    Q_EMIT signalProviderListChanged();
    setProviderIndex(m_draftProviders.size() - 1); // 自动选中新建的
    markModified();
}

void ProviderEditorViewModel::deleteCurrentProvider()
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
    {
        m_draftProviders.removeAt(m_curProviderIndex);
        Q_EMIT signalProviderListChanged();

        // 如果删光了，置为 -1；否则选中上一个/当前位置
        setProviderIndex(m_draftProviders.isEmpty()
                                 ? -1
                                 : qMax(static_cast<qsizetype>(0), m_curProviderIndex - 1));
        markModified();
    }
}

void ProviderEditorViewModel::addModel(const QString& name)
{
    if (m_curProviderIndex >= 0 && m_curProviderIndex < m_draftProviders.size())
    {
        Model m;
        m.name = name;
        m.endpoint = "/chat/completions"; // 给个默认值
        m_draftProviders[m_curProviderIndex].models.append(m);

        Q_EMIT signalModelListChanged();
        setModelIndex(m_draftProviders[m_curProviderIndex].models.size() - 1);
        markModified();
    }
}

void ProviderEditorViewModel::deleteCurrentModel()
{
    if (m_curProviderIndex >= 0 && m_curModelIndex >= 0)
    {
        auto& models = m_draftProviders[m_curProviderIndex].models;
        if (m_curModelIndex < models.size())
        {
            models.removeAt(m_curModelIndex);
            Q_EMIT signalModelListChanged();

            setModelIndex(models.isEmpty()
                                  ? -1
                                  : qMax(qsizetype(0), m_curModelIndex - 1));
            markModified();
        }
    }
}

// ==================== 核心：保存与恢复 ====================

void ProviderEditorViewModel::saveChanges()
{
    if (!m_repo || !m_isModified)
    {
        return;
    }

    // 假设你在 SettingsRepository 提供了一个 updateSettings 的闭包函数
    // 将内存草稿全量覆盖到底层 Repo 中，并触发真正的落盘
     m_repo->updateSettings([this](UserSettings& settings) {
        settings.m_providers = this->m_draftProviders;
    });

    m_isModified = false;
    Q_EMIT signalIsModifiedChanged();
}

void ProviderEditorViewModel::discardChanges()
{
    if (m_repo)
    {
        m_draftProviders = m_repo->getSettings().m_providers; // 深拷贝
        Q_EMIT signalProviderListChanged();
        setProviderIndex(m_draftProviders.isEmpty() ? -1 : 0);

        m_isModified = false;
        Q_EMIT signalIsModifiedChanged();
    }
}

// ==================== 私有辅助函数 ====================

void ProviderEditorViewModel::markModified()
{
    if (!m_isModified)
    {
        m_isModified = true;
        Q_EMIT signalIsModifiedChanged();
    }
}

void ProviderEditorViewModel::refreshAllProviderSignals()
{
    Q_EMIT signalBaseURLChanged();
    Q_EMIT signalApiKeyChanged();
    Q_EMIT signalModelListChanged();
}

void ProviderEditorViewModel::refreshAllModelSignals()
{
    Q_EMIT signalModelNameChanged();
    Q_EMIT signalEndpointChanged();
}

} // namespace QA::Service
