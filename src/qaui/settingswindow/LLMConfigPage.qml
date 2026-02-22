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
        anchors.margins: Math.max(24, root.width * 0.05)
        spacing: 0

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
                Layout.preferredWidth: 160
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
