import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

// 这是一个纯粹的 UI 组件，只负责展示传入的 modelData
ColumnLayout {
    id: root
    spacing: 20

    // 【核心设计】接收来自后端的对象
    // 这里利用 var 接收 C++ 的 Model (Q_GADGET)
    property var modelData: null

    // 需要 providerId 来进行保存操作
    property string providerId: ""

    visible: modelData !== null

    // 1. 标题区
    Label {
        text: modelData ? modelData.name : ""
        font.bold: true
        font.pixelSize: 20
    }

    AppDivider {}

    // 2. Capabilities (Checkbox 组)
    GroupBox {
        title: "Capabilities"
        Layout.fillWidth: true

        RowLayout {
            spacing: 20

            CheckBox {
                id: checkText
                text: "Text Generation"
                // 【显示逻辑】后端传来 List，前端判断是否包含
                checked: modelData && modelData.capabilities.includes("text")
                onToggled: save()
            }

            CheckBox {
                id: checkVision
                text: "Vision / Image"
                checked: modelData && modelData.capabilities.includes("vision")
                onToggled: save()
            }
        }
    }

    // 3. Endpoint 编辑
    GroupBox {
        title: "Endpoint Configuration"
        Layout.fillWidth: true

        ColumnLayout {
            TextField {
                id: endpointField
                Layout.fillWidth: true
                // 【显示逻辑】后端对象直接驱动 UI
                text: modelData ? modelData.endPointer : ""
                placeholderText: "Use Default (Base Endpoint)"
                onEditingFinished: save()
            }

            Label {
                text: endpointField.text === ""
                    ? "Current status: Using Provider's Base Endpoint"
                    : "Current status: Using Custom Endpoint"
                color: "gray"
                font.pixelSize: 11
            }
        }
    }

    Item { Layout.fillHeight: true } // 底部弹簧

    // ----------------------------------------------------
    // 【交互逻辑】收集数据 -> 告诉后端修改 -> 后端刷新对象
    // ----------------------------------------------------
    function save() {
        if (!modelData) return

        // 1. 处理 Checkbox -> List (或你想要的二进制逻辑)
        let newCaps = []
        if (checkText.checked) newCaps.push("text")
        if (checkVision.checked) newCaps.push("vision")

        // 2. 处理 Endpoint
        let newEp = endpointField.text.trim()

        // 3. 调用后端接口更新
        // 注意：我们把新状态传回去，后端更新完后会 emit providersChanged
        // 从而导致 root.modelData 自动刷新，形成闭环。
        configManager.updateModel(
            root.providerId,
            modelData.name, // 旧名字作为 ID
            modelData.name, // 名字保持不变
            newEp,
            newCaps
        )
    }
}