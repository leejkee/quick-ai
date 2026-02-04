import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    implicitWidth: 400
    implicitHeight: mainLayout.implicitHeight + 20

    radius: 8
    color: palette.window
    border.color: palette.mid
    border.width: 1

    property var selectionManager: null
    enabled: selectionManager !== null
    readonly property bool isProviderSelected: providerComboBox.currentIndex !== -1

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        GridLayout {
            anchors.fill: undefined
            width: parent.width
            columns: 2
            rowSpacing: 15
            columnSpacing: 10

            Label {
                text: "Provider: "
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
            ComboBox {
                id: providerComboBox
                editable: false
                Layout.fillWidth: true
                model: root.selectionManager ? root.selectionManager.providerList : []
                currentIndex: root.selectionManager ? root.selectionManager.providerList.indexOf(root.selectionManager.selectedProviderId) : -1

                onActivated: (index) => {
                    if (root.selectionManager) {
                        root.selectionManager.selectedProviderId = currentText
                    }
                }
            }

            Label {
                text: "Base URL: "
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                enabled: root.isProviderSelected
            }
            TextField {
                id: urlField
                Layout.fillWidth: true
                placeholderText: "https://api.provider.com"
                selectByMouse: true
                enabled: root.isProviderSelected
                text: root.selectionManager ? root.selectionManager.baseURL : ""
            }

            Label {
                text: "API Key: "
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                enabled: root.isProviderSelected
            }
            TextField {
                id: keyField
                Layout.fillWidth: true
                placeholderText: "xxx-xxx"
                echoMode: TextInput.Password
                selectByMouse: true
                enabled: root.isProviderSelected
                text: root.selectionManager ? root.selectionManager.APIKey : ""
            }

            Label {
                text: "Models: "
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                enabled: root.isProviderSelected
            }
            ComboBox {
                id: modelComboBox
                editable: false
                Layout.fillWidth: true
                model: root.selectionManager ? root.selectionManager.modelList : []
                currentIndex: root.selectionManager ? root.selectionManager.modelList.indexOf(root.selectionManager.selectedModel) : -1
                enabled: root.isProviderSelected
                onActivated: (index) => {
                    if (root.selectionManager) {
                        root.selectionManager.selectedModel = currentText
                    }
                }
            }
        }
    }
}
