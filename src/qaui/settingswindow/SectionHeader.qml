import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Label {
    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    color: palette.highlight

    // ============================================
    // Dimensions
    // ============================================
    readonly property int headerFontSize: 13
    readonly property real letterSpacing: 1.2
    readonly property int topMargin: 24
    readonly property int bottomMargin: 8

    font.pixelSize: headerFontSize
    font.bold: true
    font.capitalization: Font.AllUppercase
    font.letterSpacing: letterSpacing

    Layout.topMargin: topMargin
    Layout.bottomMargin: bottomMargin
    Layout.fillWidth: true
}
