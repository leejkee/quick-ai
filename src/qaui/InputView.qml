import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root

    signal sendMessage(string text)

    implicitHeight: mainColumnLayout.implicitHeight
    implicitWidth: mainColumnLayout.implicitWidth

    ColumnLayout {
        id: mainColumnLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        // anchors.margins: 10
        spacing: 5

        RowLayout {
            id: topBar
            Layout.fillWidth: true

            Label {
                font.pixelSize: 20
                text: "Prompt:"
                Layout.alignment: Qt.AlignVCenter
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: sendButton
                text: "Enter"
                onClicked: {
                    if (textInput.text.trim().length > 0) {
                        root.sendMessage(textInput.text);
                        textInput.text = "";
                    } else {
                        textInput.text = "";
                    }
                }
            }
        }

        Rectangle {
            id: borderRectangle
            property int paddingConst: 5

            Layout.fillWidth: true
            Layout.preferredHeight: Math.min(textInput.implicitHeight + paddingConst * 2, 100)

            radius: 6
            border.width: 2
            border.color: "#323232"
            clip: true

            Flickable {
                id: inputArea
                anchors.fill: parent
                contentHeight: textInput.height
                clip: true
                ScrollBar.vertical: ScrollBar {
                }

                TextArea {
                    id: textInput
                    width: inputArea.width - borderRectangle.paddingConst * 2
                    height: textInput.implicitHeight

                    leftPadding: borderRectangle.paddingConst
                    rightPadding: borderRectangle.paddingConst
                    topPadding: borderRectangle.paddingConst
                    bottomPadding: borderRectangle.paddingConst

                    anchors.left: parent.left
                    anchors.top: parent.top

                    placeholderText: "Type a message... (Shift + Enter for a new line)"
                    wrapMode: Text.WordWrap
                    background: null
                    onCursorRectangleChanged: {
                        // 1. 获取光标矩形在 Flickable 内容中的 Y 坐标
                        const cursorBottomY = textInput.cursorRectangle.y + textInput.cursorRectangle.height
                        // 2. 获取 Flickable 当前可见区域的底部
                        const visibleBottomY = inputArea.contentY + inputArea.height
                        // 3. 如果光标在可见区域 *下方*
                        if (cursorBottomY > visibleBottomY) {
                            // 滚动 Flickable，使光标底部与 Flickable 底部对齐
                            inputArea.contentY = cursorBottomY - inputArea.height
                        }
                        // 4. 如果光标在可见区域 *上方* (例如，用方向键向上移动)
                        if (textInput.cursorRectangle.y < inputArea.contentY) {
                            // 滚动 Flickable，使光标顶部与 Flickable 顶部对齐
                            inputArea.contentY = textInput.cursorRectangle.y
                        }
                    }

                    Keys.onPressed: event => {
                        if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter) && (event.modifiers & Qt.ShiftModifier)) {
                            textInput.insert(textInput.cursorPosition, "\n");
                            event.accepted = true;
                        } else if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {

                            if (textInput.text.trim().length === 0) {
                                textInput.text = "";
                                event.accepted = true;
                                return;
                            }

                            root.sendMessage(textInput.text);
                            textInput.text = "";
                            event.accepted = true;
                        }
                    }
                } //  TextArea
            } //  Flickable
        } //  borderRectangle
    } //  mainColumnLayout
}
