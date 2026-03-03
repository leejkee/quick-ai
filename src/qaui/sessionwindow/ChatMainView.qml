import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Fusion

ApplicationWindow {
    id: root
    title: "QML Chat UI"
    flags: Qt.FramelessWindowHint
    visible: false
    minimumHeight: 300
    minimumWidth: 400

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color primaryColor: palette.text
    readonly property color borderColor: palette.mid

    // ============================================
    // Dimensions
    // ============================================
    readonly property int defaultMargin: 10

    onActiveFocusItemChanged: {
        if (!activeFocusItem) {
            visible = false
        }
    }

    onVisibleChanged: {
        if (visible) {
            root.x = Screen.virtualX + (Screen.width - root.width) / 2
            root.y = Screen.virtualY + (Screen.height - root.height) * 0.33 // 位于 1/3 处
            inputView.forceActiveFocus()
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            root.visible = false
        }
    }

    MouseArea {
        anchors.fill: parent
        z: -1

        onPressed: {
            root.startSystemMove()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: root.defaultMargin
        spacing: root.defaultMargin

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
