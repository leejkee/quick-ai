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