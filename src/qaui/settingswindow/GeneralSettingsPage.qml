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
                model: viewModel ? viewModel.themeList : []
                currentIndex: viewModel ? viewModel.themeList.indexOf(viewModel.theme) : 0
                onActivated: if(viewModel) viewModel.theme = currentText
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.minimumHeight: 30
        }
    }
}
