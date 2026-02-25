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

    ListView {
        id: messageListView
        anchors.fill: parent
        anchors.margins: 8
        clip: true
        ScrollBar.vertical: ScrollBar {}
        delegate: Rectangle {
            color: model.role === "user" ? "#E3F2FD" : "#FFFFFF"

            height: messageText.implicitHeight + 20
            width: ListView.view.width

            Text {
                id: messageText
                anchors.left: parent.left
                anchors.margins: 10
                text: model.role + ": " + model.content
                textFormat: Text.MarkdownText
                wrapMode: Text.WordWrap
            }
        }

        onCountChanged: {
            positionViewAtEnd();
        }
    }
}