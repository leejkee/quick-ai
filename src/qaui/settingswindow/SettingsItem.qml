import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

GridLayout {
    id: root

    // --- 公开属性 ---
    property alias text: label.text
    property alias description: subLabel.text

    // 新增属性：是否启用垂直模式（默认 false 为左右布局）
    property bool vertical: false

    // 指向内部容器，用于存放子组件
    default property alias content: container.data

    // --- 布局配置 ---
    columns: vertical ? 1 : 2
    rowSpacing: vertical ? 8 : 0
    columnSpacing: 16
    Layout.fillWidth: true

    // 1. 左侧（或上方）的文字区域
    ColumnLayout {
        spacing: 4

        // 关键布局逻辑：
        // 垂直模式下：文字区域填满宽度
        // 水平模式下：文字区域占据剩余空间
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop

        Label {
            id: label
            font.pixelSize: 15
            font.weight: Font.Medium
            color: Material.foreground
            Layout.fillWidth: true
        }

        Label {
            id: subLabel
            visible: text !== ""
            font.pixelSize: 12
            color: Material.hintTextColor
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            lineHeight: 1.3
        }
    }

    // 2. 右侧（或下方）的组件容器
    RowLayout {
        id: container

        // 关键布局逻辑：
        // 垂直模式下：填满宽度 (Layout.fillWidth: true)
        // 水平模式下：靠右对齐，宽度由内容决定
        Layout.fillWidth: root.vertical
        Layout.alignment: root.vertical ? Qt.AlignLeft : (Qt.AlignRight | Qt.AlignVCenter)

        spacing: 10
    }
}
