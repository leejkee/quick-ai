import QtQuick
import QtQuick.Controls

Flickable {
    id: flickable

    property alias chatModel: messageListView.model

    contentHeight: messageListView.contentHeight

    clip: true

    ScrollBar.vertical: ScrollBar { }

    ListView {
        id: messageListView

        width: flickable.width
        height: flickable.height

        onCountChanged: {
            positionViewAtEnd()
        }

        delegate: Rectangle {
            color: model.role === "user" ? "#E3F2FD" : "#FFFFFF"
            width: parent.width
            height: messageText.height + 20

            Text {
                id: messageText
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 10

                text: model.role + ": " + model.text
                textFormat: Text.MarkdownText
                width: parent.width - 20
                wrapMode: Text.WordWrap
            }
        }
    }
}