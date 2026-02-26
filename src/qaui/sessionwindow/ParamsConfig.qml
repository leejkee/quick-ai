import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

Rectangle {
    id: root

    property var paramsManager: null
    enabled: paramsManager !== null

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color backgroundColor: palette.window
    readonly property color borderColor: palette.mid
    readonly property color titleColor: palette.text
    readonly property color dividerColor: palette.midlight

    // ============================================
    // Dimensions
    // ============================================
    readonly property int layoutMargin: 10
    readonly property int sectionSpacing: 15
    readonly property int gridSpacing: 10
    readonly property int titleFontSize: 14
    readonly property int labelFontSize: 12

    // ============================================
    // Fusion Style Metrics
    // ============================================
    QtObject {
        id: fusionMetrics
        readonly property int cornerRadius: 6
        readonly property int borderWidth: 1
    }

    color: backgroundColor
    border.color: borderColor
    radius: fusionMetrics.cornerRadius
    border.width: fusionMetrics.borderWidth

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.margins: root.layoutMargin
        spacing: root.sectionSpacing

        Text {
            text: "Model Parameters"
            font.bold: true
            font.pixelSize: root.titleFontSize
            color: root.titleColor
        }

        // Divider
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: root.dividerColor
        }

        GridLayout {
            columns: 2
            columnSpacing: root.gridSpacing
            rowSpacing: root.sectionSpacing
            Layout.fillWidth: true

            // --- Temperature ---
            Label {
                text: "Temperature: " + tempSlider.value.toFixed(1)
                font.pixelSize: root.labelFontSize
                Layout.alignment: Qt.AlignVCenter
            }
            Slider {
                id: tempSlider
                Layout.fillWidth: true
                from: 0.0
                to: 2.0
                stepSize: 0.1
                value: paramsManager.temperature
                onMoved: paramsManager.temperature = value

                ToolTip.visible: hovered
                ToolTip.text: "随机性控制 (0.0 精确 - 2.0 创意)"
            }

            // --- Top P ---
            Label {
                text: "Top P: " + topPSlider.value.toFixed(2)
                font.pixelSize: root.labelFontSize
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
                font.pixelSize: root.labelFontSize
                Layout.alignment: Qt.AlignVCenter
            }
            RowLayout {
                Layout.fillWidth: true

                SpinBox {
                    id: tokenBox
                    Layout.fillWidth: true
                    from: 128
                    to: 8192
                    stepSize: 128
                    editable: true

                    value: paramsManager.maxTokens
                    onValueModified: paramsManager.maxTokens = value
                }
            }
        }

        // Reset button
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
