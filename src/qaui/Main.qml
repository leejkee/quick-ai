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