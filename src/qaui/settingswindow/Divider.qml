import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

// This item is used to divide two sections
Rectangle {
    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    color: palette.mid

    // ============================================
    // Dimensions
    // ============================================
    readonly property int dividerHeight: 1
    readonly property int verticalMargin: 16

    Layout.fillWidth: true
    height: dividerHeight
    Layout.topMargin: verticalMargin
    Layout.bottomMargin: verticalMargin
}
