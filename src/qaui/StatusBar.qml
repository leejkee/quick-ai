import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    height: 40

    property alias leftText: messageLabel.text
    property alias rightText: rightInfoLabel.text

    color: "#fafafa"
    Rectangle {
        height: 1
        width: parent.width
        color: "#dcdcdc"
        anchors.top: parent.top
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Label {
            id: messageLabel
            text: "info"
            color: "#333333"

            elide: Text.ElideRight

            Layout.maximumWidth: root.width * 0.7
        }

        Item {
            Layout.fillWidth: true
        }

        Label {
            id: rightInfoLabel
            text: ""
            color: "#777777"
            font.italic: true
        }
    }
}