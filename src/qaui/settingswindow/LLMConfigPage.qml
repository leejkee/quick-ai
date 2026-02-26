import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    id: root
    contentWidth: availableWidth
    clip: true

    property var runtimeViewModel
    property var paramsViewModel

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color textColor: palette.text
    readonly property color accentColor: palette.highlight

    // ============================================
    // Dimensions
    // ============================================
    readonly property int titleFontSize: 24
    readonly property int comboBoxWidth: 160
    readonly property int sectionSpacing: 20
    readonly property int defaultMargin: 24

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(root.defaultMargin, root.width * 0.05)
        spacing: 0

        Label {
            text: "LLM Configuration"
            font.pixelSize: root.titleFontSize
            font.bold: true
            color: root.textColor
            Layout.bottomMargin: root.sectionSpacing
            Layout.topMargin: 10
        }

        SectionHeader {
            text: "Provider Selection"
        }

        SettingsItem {
            text: "Provider:"

            ComboBox {
                Layout.preferredWidth: root.comboBoxWidth
                model: root.runtimeViewModel ? root.runtimeViewModel.providerList : []
                currentIndex: root.runtimeViewModel ? root.runtimeViewModel.providerList.indexOf(runtimeViewModel.selectedProviderId) : 0
                onActivated: if(root.runtimeViewModel) root.runtimeViewModel.selectedProviderId = currentText
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
                Layout.preferredWidth: root.comboBoxWidth
                model: root.runtimeViewModel ? root.runtimeViewModel.modelList : []
                currentIndex: root.runtimeViewModel ? root.runtimeViewModel.modelList.indexOf(runtimeViewModel.selectedModel) : 0
                onActivated: if(root.runtimeViewModel) root.runtimeViewModel.selectedModel = currentText
            }
        }

        Divider {}

        SectionHeader {
            text: "Inference Parameters"
        }

        SettingsItem {
            text: "Model Params:"
            description: "程序会使用这里的参数向api发送post请求"
            vertical: true
            ModelParamsEditor{
                Layout.fillWidth: true
                viewModel: root.paramsViewModel
            }
        }


        Item {
            Layout.fillHeight: true
        }
    }
}
