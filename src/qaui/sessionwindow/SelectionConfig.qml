import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Rectangle {
    id: root

    property var selectionManager: null
    enabled: selectionManager !== null
    readonly property bool isProviderSelected: providerComboBox.currentIndex !== -1

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color backgroundColor: palette.window
    readonly property color borderColor: palette.mid

    // ============================================
    // Dimensions
    // ============================================
    readonly property int layoutMargin: 10
    readonly property int rowSpacing: 15
    readonly property int columnSpacing: 10

    // ============================================
    // Fusion Style Metrics
    // ============================================
    QtObject {
        id: fusionMetrics
        readonly property int cornerRadius: 6
        readonly property int borderWidth: 1
    }

    radius: fusionMetrics.cornerRadius
    color: backgroundColor
    border.color: borderColor
    border.width: fusionMetrics.borderWidth

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: root.layoutMargin
        spacing: root.layoutMargin

        GridLayout {
            anchors.fill: undefined
            width: parent.width
            columns: 2
            rowSpacing: root.rowSpacing
            columnSpacing: root.columnSpacing

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
