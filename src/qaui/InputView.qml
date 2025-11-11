import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    height: 80
    color: "white"

    signal sendMessage(string text)

    RowLayout {
        anchors.fill: parent
        anchors.margins: 5

        Label {
            text: "Prompt:"
            font.pixelSize: 20
        }

        TextField {
            id: textInput
            Layout.fillWidth: true
            placeholderText: "Type a message..."

            onAccepted: {
                root.sendMessage(textInput.text)
                textInput.text = ""
            }
        }

        Button {
            id: sendButton
            text: "Enter"
            onClicked: {
                root.sendMessage(textInput.text)
                textInput.text = ""
            }
        }
    }
}