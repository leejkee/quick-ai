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
        anchors.margins: Math.max(24, root.width * 0.05)
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
                model: root.viewModel ? root.viewModel.themeList : []
                currentIndex: root.viewModel ? root.viewModel.themeList.indexOf(root.viewModel.theme) : 0
                onActivated: if(root.viewModel) root.viewModel.theme = currentText
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 30
        }
    }
}
