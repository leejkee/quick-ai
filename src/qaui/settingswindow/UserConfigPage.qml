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

    property var initViewModel: null
    property var providerViewModel: null

    readonly property color textColor: "#1e1e1e"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(24, root.width * 0.05)
        spacing: 0

        Label {
            text: "User Configuration"
            font.pixelSize: 24
            font.bold: true
            color: root.textColor
            Layout.bottomMargin: 20
            Layout.topMargin: 10
        }

        SettingsItem {
            text: "Launch Selection"
            description: "启动时默认选择的模型"
            vertical: true
            GridLayout {
                columns: 2
                rowSpacing: 10
                columnSpacing: 10
                Label {
                    text: "Selected Provider:"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
                TextField {
                    Layout.fillWidth: true
                    selectByMouse: true
                    placeholderText: "Enter default provider..."

                    text: root.initViewModel ? root.initViewModel.selectedProvider : ""

                    onTextEdited: {
                        if (root.initViewModel) {
                            root.initViewModel.selectedProvider = text
                        }
                    }
                    onEditingFinished: if (root.initViewModel) root.initViewModel.selectedProvider = text
                }

                Label {
                    text: "Selected Model:"
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                }
                TextField {
                    Layout.fillWidth: true
                    selectByMouse: true
                    placeholderText: "Enter default model..."

                    text: root.initViewModel ? root.initViewModel.selectedModel : ""

                    onTextEdited: {
                        if (root.initViewModel) {
                            root.initViewModel.selectedModel = text
                        }
                    }
                }
            }
        }

        Divider {
        }

        SettingsItem {
            text: "Model Params:"
            description: "启动时程序会加载这里的参数作为默认参数"
            vertical: true
            ModelParamsEditor {
                Layout.fillWidth: true
                viewModel: root.initViewModel
            }
        }

        Divider {
        }

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
                        text: root.initViewModel ? root.initViewModel.systemPrompt : ""
                        placeholderText: "e.g. You are a helpful assistant..."
                        color: root.textColor
                        selectByMouse: true
                        wrapMode: Text.Wrap

                        leftPadding: 12
                        rightPadding: 12
                        topPadding: 12
                        bottomPadding: 12

                        background: null
                        onTextChanged: if (root.initViewModel) root.initViewModel.systemPrompt = text
                    }
                }
            }
        }

        Divider {
        }

        SettingsItem{
            text: "Provider Config"
            description: "Edit/Add/Remove providers"
            vertical: true
            ProviderEditor{
                viewModel: root.providerViewModel ? root.providerViewModel : null
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 60
        }
    }
}