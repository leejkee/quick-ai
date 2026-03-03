import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    id: root
    contentWidth: availableWidth
    clip: true

    property var initViewModel: null
    property var providerViewModel: null

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color textColor: palette.text
    readonly property color accentColor: palette.highlight
    readonly property color frameBackground: palette.base

    // ============================================
    // Dimensions
    // ============================================
    readonly property int titleFontSize: 24
    readonly property int gridSpacing: 10
    readonly property int systemPromptHeight: 150
    readonly property int textPadding: 12
    readonly property int defaultMargin: 24
    readonly property int sectionSpacing: 20

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(root.defaultMargin, root.width * 0.05)
        spacing: 0

        Label {
            text: "User Configuration"
            font.pixelSize: root.titleFontSize
            font.bold: true
            color: root.textColor
            Layout.bottomMargin: root.sectionSpacing
            Layout.topMargin: 10
        }

        SettingsItem {
            text: "Launch Selection"
            description: "Choose your default provider and model for quick launching."
            vertical: true
            GridLayout {
                columns: 2
                rowSpacing: root.gridSpacing
                columnSpacing: root.gridSpacing
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
            description: "Launch parameters for the default model"
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
                Layout.preferredHeight: root.systemPromptHeight
                padding: 0
                Layout.topMargin: root.gridSpacing
                ScrollView {
                    anchors.fill: parent
                    TextArea {
                        text: root.initViewModel ? root.initViewModel.systemPrompt : ""
                        placeholderText: "e.g. You are a helpful assistant..."
                        color: root.textColor
                        selectByMouse: true
                        wrapMode: Text.Wrap

                        leftPadding: root.textPadding
                        rightPadding: root.textPadding
                        topPadding: root.textPadding
                        bottomPadding: root.textPadding

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
                Layout.fillWidth: true
                viewModel: root.providerViewModel ? root.providerViewModel : null
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 60
        }
    }
}