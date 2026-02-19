import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    height: 480
    title: "QML Chat UI"
    visible: true
    width: 640

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
            chatModel: messageViewModel.messageListModel
        }
        StatusBar {
            id: statusBar

            Layout.fillWidth: true
            leftText: messageViewModel.statusMessage
        }
        ParamsConfig {
            id: paramsConfig

            paramsManager: paramsConfigManager
            Layout.fillWidth: true
        }

        SelectionConfig {
            id: selectionConfig
            selectionManager: selectionConfigManager
            Layout.fillWidth: true
        }
    }
}
