import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Label {
    // 独立的组件应尽量自包含，直接使用 Material 的颜色系统
    font.pixelSize: 13
    font.bold: true
    font.capitalization: Font.AllUppercase
    font.letterSpacing: 1.2

    color: Material.accent

    Layout.topMargin: 24
    Layout.bottomMargin: 8
    Layout.fillWidth: true
}
