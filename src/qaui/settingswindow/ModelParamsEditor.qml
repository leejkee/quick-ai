import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion

GridLayout {
    id: root
    columns: 2

    property var viewModel: null

    // ============================================
    // Dimensions
    // ============================================
    readonly property int rowSpacingValue: 10
    readonly property int columnSpacingValue: 10
    readonly property int labelMinWidth: 30

    rowSpacing: rowSpacingValue
    columnSpacing: columnSpacingValue

    // --- Frequency Penalty ---
    Label {
        text: "Frequency Penalty"
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    }
    RowLayout {
        Layout.fillWidth: true
        Slider {
            id: freqSlider
            from: -2.0
            to: 2.0
            stepSize: 0.1
            value: root.viewModel ? root.viewModel.frequencyPenalty : 0.0
            onMoved: if(root.viewModel) root.viewModel.frequencyPenalty = value
            Layout.fillWidth: true
        }
        Label {
            // 实时显示数值，保留1位小数
            text: freqSlider.value.toFixed(1)
            Layout.minimumWidth: root.labelMinWidth
            horizontalAlignment: Text.AlignRight
        }
    }

    // --- Max Tokens ---
    Label {
        text: "Max Tokens"
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    }
    RowLayout {
        Layout.fillWidth: true
        Slider {
            id: tokensSlider
            from: 1
            to: 8192
            stepSize: 1
            value: root.viewModel ? root.viewModel.maxTokens : 2048
            onMoved: if(root.viewModel) root.viewModel.maxTokens = value
            Layout.fillWidth: true
        }
        Label {
            text: Math.round(tokensSlider.value).toString()
            Layout.minimumWidth: root.labelMinWidth
            horizontalAlignment: Text.AlignRight
        }
    }

    // --- Presence Penalty ---
    Label {
        text: "Presence Penalty"
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    }
    RowLayout {
        Layout.fillWidth: true
        Slider {
            id: presSlider
            from: -2.0
            to: 2.0
            stepSize: 0.1
            value: root.viewModel ? root.viewModel.presencePenalty : 0.0
            onMoved: if(root.viewModel) root.viewModel.presencePenalty = value
            Layout.fillWidth: true
        }
        Label {
            text: presSlider.value.toFixed(1)
            Layout.minimumWidth: root.labelMinWidth
            horizontalAlignment: Text.AlignRight
        }
    }

    // --- Temperature ---
    Label {
        text: "Temperature"
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    }
    RowLayout {
        Layout.fillWidth: true
        Slider {
            id: tempSlider
            from: 0.0
            to: 2.0
            stepSize: 0.1
            value: root.viewModel ? root.viewModel.temperature : 0.7
            onMoved: if(root.viewModel) root.viewModel.temperature = value
            Layout.fillWidth: true
        }
        Label {
            text: tempSlider.value.toFixed(1)
            Layout.minimumWidth: root.labelMinWidth
            horizontalAlignment: Text.AlignRight
        }
    }

    // --- Top P ---
    Label {
        text: "Top P"
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    }
    RowLayout {
        Layout.fillWidth: true
        Slider {
            id: topPSlider
            from: 0.0
            to: 1.0
            stepSize: 0.05
            value: root.viewModel ? root.viewModel.topP : 1.0
            onMoved: if(root.viewModel) root.viewModel.topP = value
            Layout.fillWidth: true
        }
        Label {
            text: topPSlider.value.toFixed(2) // 两位小数
            Layout.minimumWidth: root.labelMinWidth
            horizontalAlignment: Text.AlignRight
        }
    }

    // --- Stream ---
    Label {
        text: "Stream Output"
        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
    }
    Switch {
        checked: root.viewModel ? root.viewModel.stream : false
        onToggled: if(root.viewModel) root.viewModel.stream = checked
        Layout.alignment: Qt.AlignLeft
    }
}