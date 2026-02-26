import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Rectangle {
    id: root
    clip: true
    color: palette.base
    radius: fusionMetrics.cornerRadius
    border.width: fusionMetrics.borderWidth
    border.color: palette.mid
    property alias chatModel: messageListView.model

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color userMessageColor: palette.highlight
    readonly property color aiMessageColor: palette.base
    readonly property color userMessageTextColor: palette.highlightedText
    readonly property color aiMessageTextColor: palette.text

    // ============================================
    // Dimensions
    // ============================================
    readonly property int messageMargin: 8
    readonly property int messagePadding: 10
    readonly property int messageSpacing: 20

    // ============================================
    // Fusion Style Metrics
    // ============================================
    QtObject {
        id: fusionMetrics
        readonly property int cornerRadius: 6
        readonly property int borderWidth: 1
    }

    ListView {
        id: messageListView
        anchors.fill: parent
        anchors.margins: root.messageMargin
        clip: true
        ScrollBar.vertical: ScrollBar {}
        delegate: Rectangle {
            color: model.role === "user" ? root.userMessageColor : root.aiMessageColor

            width: ListView.view.width

            height: messageText.implicitHeight + (root.messagePadding * 2)

            Text {
                id: messageText
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: root.messagePadding

                text: model.role === "user" ? "User: " + model.content : "AI: " + model.content
                color: model.role === "user" ? root.userMessageTextColor : root.aiMessageTextColor

                textFormat: Text.MarkdownText
                wrapMode: Text.WordWrap
            }
        }

        onCountChanged: {
            positionViewAtEnd();
        }
    }
}