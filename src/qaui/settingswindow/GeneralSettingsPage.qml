import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ScrollView {
    id: root

    contentWidth: availableWidth
    clip: true

    property var viewModel

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
    readonly property int defaultMargin: 24
    readonly property int sectionSpacing: 20

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(root.defaultMargin, root.width * 0.05)
        spacing: 0

        Label {
            text: "General Settings"
            font.pixelSize: root.titleFontSize
            font.bold: true
            color: root.textColor
            Layout.bottomMargin: root.sectionSpacing
            Layout.topMargin: 10
        }

        SectionHeader {
            text: "Appearance"
        }

        SettingsItem {
            text: "Theme"
            description: "Switch between Light and Dark themes"

            ComboBox {
                Layout.preferredWidth: root.comboBoxWidth
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
