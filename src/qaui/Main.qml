import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    height: 480
    title: "QML Chat UI"
    visible: true
    width: 640

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        InputView {
            id: inputView

            Layout.fillWidth: true
            Layout.preferredHeight: 80

            onSendMessage: text => chatViewModel.handleUserRequest(text)
        }
        SessionView {
            id: sessionView

            Layout.fillHeight: true
            Layout.fillWidth: true
            chatModel: chatViewModel.messageListModel
        }
    }
}