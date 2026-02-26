import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    id: root
    clip: true

    property var viewModel: null

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color textFieldBackground: palette.base

    // ============================================
    // Dimensions
    // ============================================
    readonly property int minTextFieldWidth: 200
    readonly property int sectionSpacing: 20
    readonly property int labelSpacing: 10

    Dialog {
        id: newProviderDialog
        title: "New Provider"
        standardButtons: Dialog.Ok | Dialog.Cancel
        anchors.centerIn: parent
        modal: true

        RowLayout {
            TextField {
                id: newProviderIdInput
                placeholderText: "Enter ID (e.g., openai)"
                Component.onCompleted: forceActiveFocus()
            }
        }

        onAccepted: {
            if (root.viewModel && newProviderIdInput.text.trim() !== "") {
                root.viewModel.addProvider(newProviderIdInput.text.trim())
                newProviderIdInput.clear()
            }
        }
        onRejected: newProviderIdInput.clear()
    }

    ColumnLayout {
        enabled: root.viewModel !== null
        spacing: 20
        width: root.availableWidth
        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "Provider" }
            ComboBox {
                Layout.fillWidth: true
                model: root.viewModel ? root.viewModel.providerList : []
                currentIndex: root.viewModel ? root.viewModel.providerIndex : -1
                onActivated: {
                    if (root.viewModel) {
                        root.viewModel.providerIndex = currentIndex
                    }
                }
            }

            RowLayout {
                Button {
                    text: "New"
                    onClicked: newProviderDialog.open()
                }

                Button {
                    text: "Del"
                    enabled: root.viewModel ? root.viewModel.providerIndex >= 0 : false
                    onClicked: {
                        if (root.viewModel) {
                            root.viewModel.deleteCurrentProvider()
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "BaseURL" }
            TextField {
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                text: root.viewModel ? root.viewModel.baseURL : ""
                onTextEdited: {
                    if (root.viewModel) {
                        root.viewModel.baseURL = text
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "APIKey" }
            TextField {
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                echoMode: TextInput.Password
                text: root.viewModel ? root.viewModel.APIKey : ""
                onTextEdited: {
                    if (root.viewModel) {
                        root.viewModel.APIKey = text
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "Model" }
            ComboBox {
                Layout.fillWidth: true
                model: root.viewModel ? root.viewModel.modelList : []
                currentIndex: root.viewModel ? root.viewModel.modelIndex : -1
                onActivated: {
                    if (root.viewModel) {
                        root.viewModel.modelIndex = currentIndex
                    }
                }
            }

            Label {
                text: "Endpoint"
                Layout.topMargin: 10
            }
            TextField {
                Layout.fillWidth: true
                text: root.viewModel ? root.viewModel.endpoint : ""
                onTextEdited: {
                    if (root.viewModel) {
                        root.viewModel.endpoint = text
                    }
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop

            Button {
                text: "Save"
                enabled: root.viewModel ? root.viewModel.isModified : false
                onClicked: {
                    if (root.viewModel) {
                        root.viewModel.saveChanges()
                    }
                }
            }
            Button {
                text: "Reset"
                onClicked: {
                    if (root.viewModel) {
                        root.viewModel.discardChanges()
                    }
                }
            }
        }
    }
}