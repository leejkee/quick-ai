import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion
import QtQuick.Layouts

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
    readonly property color aiMessageColor: palette.window
    readonly property color userMessageTextColor: palette.highlightedText
    readonly property color aiMessageTextColor: palette.text
    readonly property color systemTextColor: palette.placeholderText

    // ============================================
    // Dimensions - 统一管理区域
    // ============================================
    readonly property int messageMargin: 8          // 列表与窗口边界的间距
    readonly property int messagePadding: 10        // 气泡内部文字的内边距
    readonly property int messageSpacing: 12        // 消息条目之间的垂直间距
    readonly property int labelSpacing: 4           // 角色名与气泡之间的垂直间距
    readonly property real bubbleMaxWidthRatio: 0.8 // 气泡最大占屏幕宽度的比例
    readonly property int bubbleAbsoluteMaxWidth: 800 // 气泡在宽屏下的最大绝对宽度
    readonly property int systemFontSize: 11
    readonly property int messageFontSize: 13

    QtObject {
        id: fusionMetrics
        readonly property int cornerRadius: 6
        readonly property int borderWidth: 1
    }

    ListView {
        id: messageListView
        anchors.fill: parent
        anchors.margins: root.messageMargin
        spacing: root.messageSpacing
        clip: true
        ScrollBar.vertical: ScrollBar {}

        delegate: Item {
            id: delegateRoot
            width: messageListView.width
            height: contentColumn.height

            Column {
                id: contentColumn
                // 使用统一管理的宽度逻辑
                width: model.role === "system" ? parent.width : Math.min(parent.width * root.bubbleMaxWidthRatio, root.bubbleAbsoluteMaxWidth)
                
                // 水平定位逻辑
                anchors.right: model.role === "user" ? parent.right : undefined
                anchors.left: model.role === "assistant" ? parent.left : undefined
                anchors.horizontalCenter: model.role === "system" ? parent.horizontalCenter : undefined
                
                spacing: root.labelSpacing

                // 1. 角色标签
                Text {
                    id: roleText
                    text: model.role === "user" ? qsTr("You") : qsTr("AI")
                    font.pixelSize: 10
                    font.bold: true
                    color: palette.placeholderText
                    visible: model.role !== "system"
                    
                    // 确保标签也跟随气泡对齐
                    anchors.right: model.role === "user" ? parent.right : undefined
                    anchors.rightMargin: 4
                }

                // 2. 消息气泡
                Rectangle {
                    id: bubble
                    width: model.role === "system" ? parent.width : Math.min(messageText.implicitWidth + (root.messagePadding * 2), parent.width)
                    height: messageText.implicitHeight + (root.messagePadding * 2)
                    
                    // 当 user message 长度较短时，仍然保持bubble靠右
                    anchors.right: model.role === "user" ? parent.right : undefined
                    
                    radius: fusionMetrics.cornerRadius
                    border.width: model.role === "assistant" ? fusionMetrics.borderWidth : 0
                    border.color: palette.mid
                    
                    color: {
                        if (model.role === "user") return root.userMessageColor;
                        if (model.role === "assistant") return root.aiMessageColor;
                        return "transparent"; 
                    }

                    Text {
                        id: messageText
                        anchors.fill: parent
                        anchors.margins: root.messagePadding
                        text: model.content
                        
                        color: {
                            if (model.role === "user") return root.userMessageTextColor;
                            if (model.role === "system") return root.systemTextColor;
                            return root.aiMessageTextColor;
                        }

                        font.pixelSize: model.role === "system" ? root.systemFontSize : root.messageFontSize
                        font.italic: model.role === "system"
                        horizontalAlignment: model.role === "system" ? Text.AlignHCenter : Text.AlignLeft
                        
                        textFormat: Text.MarkdownText
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }

        onCountChanged: {
            Qt.callLater(positionViewAtEnd)
        }
    }
}