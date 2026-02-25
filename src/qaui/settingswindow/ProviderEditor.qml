import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

ScrollView {
    id: root
    clip: true // 防止内部控件滚动时溢出边界

    property var viewModel: null

    // 用于输入新 Provider ID 的轻量弹窗
    Dialog {
        id: newProviderDialog
        title: "New Provider"
        standardButtons: Dialog.Ok | Dialog.Cancel
        anchors.centerIn: parent
        modal: true

        RowLayout {
            TextField {
                id: newProviderIdInput
                placeholderText: "Enter ID (e.g., openai)"
                // 弹窗打开时自动获取焦点
                Component.onCompleted: forceActiveFocus()
            }
        }

        onAccepted: {
            if (root.viewModel && newProviderIdInput.text.trim() !== "") {
                root.viewModel.addProvider(newProviderIdInput.text.trim())
                newProviderIdInput.clear()
            }
        }
        onRejected: newProviderIdInput.clear()
    }

    // 主内容区
    ColumnLayout {
        // 核心安全机制：如果 viewModel 没注入，整个界面不可交互
        enabled: root.viewModel !== null
        spacing: 20

        // ================= 1. Provider 列 =================
        RowLayout {
            Layout.alignment: Qt.AlignTop // 强制顶部对齐

            Label { text: "Provider" }
            ComboBox {
                Layout.fillWidth: true
                // 安全绑定：查空
                model: root.viewModel ? root.viewModel.providerList : []
                currentIndex: root.viewModel ? root.viewModel.providerIndex : -1
                onActivated: {
                    if (root.viewModel) {
                        root.viewModel.providerIndex = currentIndex
                    }
                }
            }

            RowLayout {
                Button {
                    text: "New"
                    onClicked: newProviderDialog.open()
                }

                Button {
                    text: "Del"
                    // 仅当选中了有效的 Provider 时，删除按钮才可用
                    enabled: root.viewModel ? root.viewModel.providerIndex >= 0 : false
                    onClicked: {
                        if (root.viewModel) {
                            root.viewModel.deleteCurrentProvider()
                        }
                    }
                }
            }
        }

        // ================= 2. BaseURL 列 =================
        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "BaseURL" }
            TextField {
                Layout.fillWidth: true
                Layout.minimumWidth: 200 // 给输入框一个合理的最小宽度
                text: root.viewModel ? root.viewModel.baseURL : ""
                onTextEdited: {
                    if (root.viewModel) {
                        root.viewModel.baseURL = text
                    }
                }
            }
        }

        // ================= 3. APIKey 列 =================
        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "APIKey" }
            TextField {
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                echoMode: TextInput.Password // 核心：隐藏秘钥
                text: root.viewModel ? root.viewModel.APIKey : ""
                onTextEdited: {
                    if (root.viewModel) {
                        root.viewModel.APIKey = text
                    }
                }
            }
        }

        // ================= 4. Model 列 =================
        RowLayout {
            Layout.alignment: Qt.AlignTop

            Label { text: "Model" }
            ComboBox {
                Layout.fillWidth: true
                model: root.viewModel ? root.viewModel.modelList : []
                currentIndex: root.viewModel ? root.viewModel.modelIndex : -1
                onActivated: {
                    if (root.viewModel) {
                        root.viewModel.modelIndex = currentIndex
                    }
                }
            }

            // 稍微增加一点顶部边距，让上下两组控件看起来更透气
            Label {
                text: "Endpoint"
                Layout.topMargin: 10
            }
            TextField {
                Layout.fillWidth: true
                text: root.viewModel ? root.viewModel.endpoint : ""
                onTextEdited: {
                    if (root.viewModel) {
                        root.viewModel.endpoint = text
                    }
                }
            }
        }

        // ================= 5. 动作 列 =================
        RowLayout {
            Layout.alignment: Qt.AlignTop

            Button {
                text: "Save"
                // 进阶体验：如果你在 C++ 实现了 isModified 属性，可以把下一行取消注释
                // enabled: root.viewModel ? root.viewModel.isModified : false
                onClicked: {
                    if (root.viewModel) {
                        root.viewModel.saveChanges()
                    }
                }
            }
            Button {
                text: "Reset"
                onClicked: {
                    if (root.viewModel) {
                        root.viewModel.discardChanges()
                    }
                }
            }
        }
    }
}