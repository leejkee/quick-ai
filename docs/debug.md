## 分析UI不刷新的原因
### 核心代码

```cpp
void SessionService::pushMessage(const Core::Message& msg)
{
    m_conversation->pushMessage(msg);
    Q_EMIT signalConversationChanged();
}

void SessionService::chatNoStreaming()
{
    if (const auto r = m_client->noStreamingChat(m_modelParams,
                                                 m_conversation->getContext());
        r.has_value())
    {
        QA_LOG_INFO << "No-streaming chat completed successfully. Role: "
                    << r->role << ", Content length: " << r->content.size();
        const Core::Message rMsg{r->role, r->content};
        pushMessage(rMsg);
    }
    else
    {
        QA_LOG_ERR
                << "Failed to get response from LLM client in no-streaming "
                   "mode. "
                << "Please check network connection or API key configuration.";
    }
}
SessionService::SessionService(SettingsRepository* configService,
                               QObject* parent)
    : QObject(parent), m_settingsRepo(configService)
{
    const auto settings = m_settingsRepo->getSettings();
    m_modelParams = settings.m_modelParams;
    m_selectedModel = settings.m_selectedModel;
    m_selectedProviderId = settings.m_selectedProviderId;
    m_conversation = new Core::LLMConversation(settings.m_systemPrompt, this);

    m_selectedProviderId = settings.m_selectedProviderId;
    m_selectedModel = settings.m_selectedModel;
    m_messageModel = new MessageModel(m_conversation, this);

    Core::PostBody body;
    body.model = m_selectedModel;
    if (auto r = getDataFromVector(settings.m_providers,
                                   [this](const Provider& p)
                                   { return p.id == m_selectedProviderId; });
        r.has_value())
    {
        body.apiKey = r.value().apiKey;
        body.url = r.value().getUrl(m_selectedModel);
    }
    m_client = Core::LLMClientFactory::createLLMClient(body, this);

    connect(this,
            &SessionService::signalConversationChanged,
            m_messageModel,
            &MessageModel::updateData);
}

```

```cpp
void MessageModel::updateData()
{
    const int newIndex = static_cast<int>(m_conversation->getMessageSize());
    beginInsertRows(QModelIndex(), newIndex, newIndex);
    endInsertRows();
}
```

```cpp

namespace QA::Service
{
MessageViewModel::MessageViewModel(SessionService* service, QObject* parent)
    : QObject(parent), m_service(service)
{
}

void MessageViewModel::handleUserRequest(const QString& prompt)
{
    if (prompt.isEmpty())
    {
        return;
    }
    Core::Message promptMsg;
    promptMsg.role = "user";
    promptMsg.content = prompt;
    // TODO
    // 多行代码作为prompt输入，对话框显示不完整

    m_service->pushMessage(promptMsg);
    m_service->chatNoStreaming();
}

void MessageViewModel::handleClearSession() { m_service->clearMessage(); }

void MessageViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message)
    {
        m_statusMessage = message;
        Q_EMIT signalStatusMessageChanged();
    }
}

QObject* MessageViewModel::getMessageListModel() const
{
    return m_service->getMessageModel();
}
```

```qmllang
// Main.qml
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    height: 480
    title: "QML Chat UI"
    visible: false
    width: 640

    onActiveFocusItemChanged: {
        if (!activeFocusItem) {
            visible = false
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            visible = false
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        InputView {
            id: inputView

            Layout.fillWidth: true
            onSendMessage: text => messageViewModel.handleUserRequest(text)
        }
        SessionView {
            id: sessionView

            Layout.fillHeight: true
            Layout.fillWidth: true
            chatModel: messageViewModel?.messageListModel
        }
        StatusBar {
            id: statusBar

            Layout.fillWidth: true
            leftText: messageViewModel?.statusMessage ?? ""
        }
    }
}
```

```qmllang
// SessionView.qml
import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    clip: true
    color: "white"
    radius: 6
    border.width: 2
    border.color: "#323232"
    property alias chatModel: messageListView.model

    Flickable {
        id: flickable

        anchors.fill: parent

        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 8
        anchors.rightMargin: 8

        clip: true
        contentHeight: messageListView.contentHeight
        ScrollBar.vertical: ScrollBar {}

        ListView {
            id: messageListView

            height: flickable.height
            width: flickable.width

            delegate: Rectangle {
                color: model.role === "user" ? "#E3F2FD" : "#FFFFFF"

                height: messageText.height +  20
                width: messageListView.width

                Text {
                    id: messageText
                    anchors.left: parent.left
                    // anchors.margins: 10
                    anchors.top: parent.top
                    text: model.role + ": " + model.content
                    textFormat: Text.MarkdownText

                    width: messageListView.width
                    wrapMode: Text.WordWrap
                }
            }

            onCountChanged: {
                positionViewAtEnd();
            }
        }
    }
}
```