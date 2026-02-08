import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

ScrollView {
    id: root

    width: 800
    height: 600
    contentWidth: availableWidth
    clip: true

    property var viewModel
    readonly property color textColor: "#1e1e1e"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(24, window.width * 0.05)
        spacing: 0

        Label {
            text: "General Settings"
            font.pixelSize: 24
            font.bold: true
            color: root.textColor
            Layout.bottomMargin: 20
            Layout.topMargin: 10
        }

        SectionHeader {
            text: "Appearance"
        }

        SettingsItem {
            text: "Theme"
            description: "Switch between Light and Dark themes"

            ComboBox {
                Layout.preferredWidth: 160
                model: ["System", "Light", "Dark" ]
                currentIndex: viewModel ? viewModel.themeIndex : 0
                onActivated: if(viewModel) viewModel.themeIndex = currentIndex
            }
        }

        AppDivider {}

        SettingsItem {
            text: "Interface Font Size"
            description: "Adjust the base text size."

            SpinBox {
                Layout.preferredWidth: 160
                from: 10; to: 32
                value: viewModel ? viewModel.fontSize : 14
                onValueModified: if(viewModel) viewModel.fontSize = value
                editable: true
            }
        }

        AppDivider {}

        SectionHeader { text: "AI Behavior" }

        SettingsItem {
            text: "System Prompt"
            description: "Define the AI's persona and constraints."
            vertical: true
            Frame {
                Layout.fillWidth: true
                Layout.preferredHeight: 150
                padding: 0
                Layout.topMargin: 10
                ScrollView {
                    anchors.fill: parent
                    TextArea {
                        text: viewModel ? viewModel.systemPrompt : ""
                        placeholderText: "e.g. You are a helpful assistant..."
                        color: root.textColor
                        selectByMouse: true
                        wrapMode: Text.Wrap

                        leftPadding: 12
                        rightPadding: 12
                        topPadding: 12
                        bottomPadding: 12

                        background: null
                        onTextChanged: if(viewModel) viewModel.systemPrompt = text
                    }
                }
            }
        }


        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 30
        }
    }
}
