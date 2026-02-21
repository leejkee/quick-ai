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

    property var runtimeViewModel
    property var paramsViewModel

    readonly property color textColor: "#1e1e1e"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(24, window.width * 0.05)
        spacing: 0

        // 一级标题
        Label {
            text: "LLM Configuration"
            font.pixelSize: 24
            font.bold: true
            color: root.textColor
            Layout.bottomMargin: 20
            Layout.topMargin: 10
        }

        SectionHeader {
            text: "Provider Selection"
        }

        SettingsItem {
            text: "Provider:"

            ComboBox {
                Layout.preferredWidth: 160
                model: runtimeViewModel ? runtimeViewModel.providerList : []
                currentIndex: runtimeViewModel ? runtimeViewModel.providerList.indexOf(runtimeViewModel.selectedProviderId) : 0
                onActivated: if(runtimeViewModel) runtimeViewModel.selectedProviderId = currentText
            }
        }

        Divider {}

        SectionHeader {
            text: "Model Settings"
        }

        SettingsItem {
            text: "Target Model:"

            ComboBox {
                id: modelCombo
                Layout.preferredWidth: 160
                model: runtimeViewModel ? runtimeViewModel.modelList : []
                currentIndex: runtimeViewModel ? runtimeViewModel.modelList.indexOf(runtimeViewModel.selectedModel) : 0
                onActivated: if(runtimeViewModel) runtimeViewModel.selectedModel = currentText
            }
        }

        Divider {}

        SectionHeader {
            text: "Inference Parameters"
        }

        SettingsItem {
            text: "Temperature:"
            description: "Higher values (e.g., 1.5) make output more random, while lower values (e.g., 0.2) make it more focused."
            vertical: true
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: tempSlider
                    from: 0.0
                    to: 2.0
                    stepSize: 0.1
                    value: paramsViewModel ? paramsViewModel.temperature : 0.7
                    onMoved: if(paramsViewModel) paramsViewModel.temperature = value

                    Layout.fillWidth: true
                }
                Label {
                    text: tempSlider.value.toFixed(1)
                    color: root.textColor
                    font.bold: true
                    Layout.preferredWidth: 40
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        SettingsItem {
            text: "Max Tokens:"
            vertical: true
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Slider {
                    id: tokenSlider
                    from: 512
                    to: 8192
                    stepSize: 128
                    value: paramsViewModel ? paramsViewModel.maxTokens : 2048
                    onMoved: if(paramsViewModel) paramsViewModel.maxTokens = value

                    Layout.fillWidth: true
                }

                SpinBox {
                    from: 512
                    to: 8192
                    stepSize: 128
                    value: tokenSlider.value
                    editable: true
                    onValueModified: if(paramsViewModel) paramsViewModel.maxTokens = value
                    Layout.preferredWidth: 160
                }
            }
        }

        Divider {}

        SectionHeader {
            text: "Additional Parameters"
        }

        SettingsItem {
            text: "Top P:"
            description: "Nucleus sampling probability (0.0 to 1.0)"
            vertical: true
            RowLayout {
                Layout.fillWidth: true
                Slider {
                    id: topPSlider
                    from: 0.0
                    to: 1.0
                    stepSize: 0.05
                    value: paramsViewModel ? paramsViewModel.topP : 1.0
                    onMoved: if(paramsViewModel) paramsViewModel.topP = value
                    Layout.fillWidth: true
                }
                Label {
                    text: topPSlider.value.toFixed(2)
                    color: root.textColor
                    font.bold: true
                    Layout.preferredWidth: 40
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        // SettingsItem {
        //     text: "Interface Font Size"
        //     description: "Adjust the base text size."
        //
        //     SpinBox {
        //         Layout.preferredWidth: 160
        //         from: 10; to: 32
        //         value: viewModel ? viewModel.fontSize : 14
        //         onValueModified: if(viewModel) viewModel.fontSize = value
        //         editable: true
        //     }
        // }
        //
        // Divider {}

        // SectionHeader {
        //     text: "AI Behavior"
        // }
        // 
        // SettingsItem {
        //     text: "System Prompt"
        //     description: "Define the AI's persona and constraints."
        //     vertical: true
        //     Frame {
        //         Layout.fillWidth: true
        //         Layout.preferredHeight: 150
        //         padding: 0
        //         Layout.topMargin: 10
        //         ScrollView {
        //             anchors.fill: parent
        //             TextArea {
        //                 text: viewModel ? viewModel.systemPrompt : ""
        //                 placeholderText: "e.g. You are a helpful assistant..."
        //                 color: root.textColor
        //                 selectByMouse: true
        //                 wrapMode: Text.Wrap
        // 
        //                 leftPadding: 12
        //                 rightPadding: 12
        //                 topPadding: 12
        //                 bottomPadding: 12
        // 
        //                 background: null
        //                 onTextChanged: if(viewModel) viewModel.systemPrompt = text
        //             }
        //         }
        //     }
        // }
        // 
        // SettingsItem {
        //     text: "Launch"
        //     description: "启动加载的默认参数"
        //     vertical: true
        // }
        Item {
            Layout.fillHeight: true
        }
    }
}
