import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    implicitWidth: 300
    implicitHeight: layout.implicitHeight + 20

    color: "#f8f9fa"
    border.color: "#e0e0e0"
    radius: 8

    property var paramsManager: null
    enabled: paramsManager !== null

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        Text {
            text: "Model Parameters"
            font.bold: true
            font.pixelSize: 14
            color: "#333"
        }

        // 分割线
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#ddd"
        }

        GridLayout {
            columns: 2
            columnSpacing: 10
            rowSpacing: 15
            Layout.fillWidth: true

            // --- Temperature (温度) --
            Label {
                text: "Temperature: " + tempSlider.value.toFixed(1)
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
            }
            Slider {
                id: tempSlider
                Layout.fillWidth: true
                from: 0.0
                to: 2.0
                stepSize: 0.1
                // 【核心】双向绑定后端属性
                value: paramsManager.temperature
                onMoved: paramsManager.temperature = value

                // 鼠标悬停提示
                ToolTip.visible: hovered
                ToolTip.text: "随机性控制 (0.0 精确 - 2.0 创意)"
            }

            // --- Top P ---
            Label {
                text: "Top P: " + topPSlider.value.toFixed(2)
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
            }
            Slider {
                id: topPSlider
                Layout.fillWidth: true
                from: 0.0
                to: 1.0
                stepSize: 0.05
                value: paramsManager.topP
                onMoved: paramsManager.topP = value

                ToolTip.visible: hovered
                ToolTip.text: "核采样概率"
            }

            // --- Max Tokens ---
            Label {
                text: "Max Tokens:"
                font.pixelSize: 12
                Layout.alignment: Qt.AlignVCenter
            }
            RowLayout {
                Layout.fillWidth: true

                // 使用 SpinBox 精确控制整数
                SpinBox {
                    id: tokenBox
                    Layout.fillWidth: true
                    from: 128
                    to: 8192 // 根据模型能力调整
                    stepSize: 128
                    editable: true // 允许直接输入数字

                    value: paramsManager.maxTokens
                    onValueModified: paramsManager.maxTokens = value
                }
            }
        }

        // 底部重置按钮（可选）
        Button {
            text: "Reset Defaults"
            Layout.alignment: Qt.AlignRight
            flat: true
            onClicked: {
                paramsManager.temperature = 0.0
                paramsManager.topP = 1.0
                paramsManager.maxTokens = 4096
            }
        }
    }
}
