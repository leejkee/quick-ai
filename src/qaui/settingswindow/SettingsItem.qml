import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

GridLayout {
    id: root

    // ============================================
    // Public Properties
    // ============================================
    property alias text: label.text
    property alias description: subLabel.text

    // Whether to enable vertical layout (default false for horizontal)
    property bool vertical: false

    // Points to internal container for child components
    default property alias content: container.data

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color labelColor: palette.text
    readonly property color hintColor: palette.mid

    // ============================================
    // Dimensions
    // ============================================
    readonly property int labelFontSize: 15
    readonly property int descriptionFontSize: 12
    readonly property int columnSpacingValue: 16
    readonly property int rowSpacingValue: 8
    readonly property int containerSpacing: 10
    readonly property real lineHeightValue: 1.3

    // --- Layout Config ---
    columns: vertical ? 1 : 2
    rowSpacing: vertical ? rowSpacingValue : 0
    columnSpacing: columnSpacingValue
    Layout.fillWidth: true

    // 1. Text area on left (or top)
    ColumnLayout {
        spacing: 4

        // Layout logic:
        // Vertical mode: fill width
        // Horizontal mode: take remaining space
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop

        Label {
            id: label
            font.pixelSize: root.labelFontSize
            font.weight: Font.Medium
            color: root.labelColor
            Layout.fillWidth: true
        }

        Label {
            id: subLabel
            visible: text !== ""
            font.pixelSize: root.descriptionFontSize
            color: root.hintColor
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            lineHeight: root.lineHeightValue
        }
    }

    // 2. Component container on right (or bottom)
    RowLayout {
        id: container

        // Layout logic:
        // Vertical mode: fill width
        // Horizontal mode: right aligned, width by content
        Layout.fillWidth: root.vertical
        Layout.alignment: root.vertical ? Qt.AlignLeft : (Qt.AlignRight | Qt.AlignVCenter)

        spacing: root.containerSpacing
    }
}
