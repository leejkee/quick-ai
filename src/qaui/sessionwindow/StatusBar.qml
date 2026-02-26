import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Fusion

Rectangle {
    id: root

    property alias leftText: messageLabel.text
    property alias rightText: rightInfoLabel.text

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color backgroundColor: palette.window
    readonly property color dividerColor: palette.mid
    readonly property color primaryTextColor: palette.text
    readonly property color secondaryTextColor: palette.mid

    // ============================================
    // Dimensions
    // ============================================
    readonly property int barHeight: 40
    readonly property int horizontalMargin: 10
    readonly property int dividerHeight: 1
    readonly property real maxTextWidthRatio: 0.7

    height: barHeight
    color: backgroundColor

    Rectangle {
        height: dividerHeight
        width: parent.width
        color: dividerColor
        anchors.top: parent.top
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: root.horizontalMargin
        anchors.rightMargin: root.horizontalMargin

        Label {
            id: messageLabel
            text: "info"
            color: root.primaryTextColor

            elide: Text.ElideRight

            Layout.maximumWidth: root.width * root.maxTextWidthRatio
        }

        Item {
            Layout.fillWidth: true
        }

        Label {
            id: rightInfoLabel
            text: ""
            color: root.secondaryTextColor
            font.italic: true
        }
    }
}