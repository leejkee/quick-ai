import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

SplitView {
    id: root
    anchors.fill: parent
    orientation: Qt.Horizontal

    // ========================================================
    // 左侧：Provider 列表 (Master View)
    // ========================================================
    Rectangle {
        SplitView.preferredWidth: 220
        SplitView.minWidth: 150
        SplitView.maxWidth: 400
        color: "#f3f3f3" // 浅灰背景

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // 1. 列表头部
            Rectangle {
                Layout.fillWidth: true
                height: 40
                color: "transparent"
                Label {
                    text: "PROVIDERS"
                    font.bold: true
                    color: "#555"
                    anchors { left: parent.left; leftMargin: 15; verticalCenter: parent.verticalCenter }
                }
            }

            // 2. 列表主体
            ListView {
                id: providerListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                // 绑定 C++ 数据源
                model: configManager.providers

                delegate: ItemDelegate {
                    id: listDelegate
                    width: providerListView.width
                    height: 40
                    highlighted: ListView.isCurrentItem

                    contentItem: Text {
                        text: modelData.name
                        color: highlighted ? "white" : "black"
                        font.pixelSize: 14
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        leftPadding: 10
                    }

                    background: Rectangle {
                        color: parent.highlighted ? "#0078d4" : (parent.hovered ? "#e0e0e0" : "transparent")
                    }

                    onClicked: providerListView.currentIndex = index

                    // 右键菜单：删除功能
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onClicked: contextMenu.popup()
                    }

                    Menu {
                        id: contextMenu
                        MenuItem {
                            text: "Delete " + modelData.name
                            onTriggered: {
                                configManager.removeProvider(modelData.name) // 调用 C++ 删除
                            }
                        }
                    }
                }

                // 底部占位：创建逻辑
                footer: ColumnLayout {
                    width: providerListView.width

                    property bool isCreating: false

                    // 状态 A: 添加按钮
                    Button {
                        visible: !parent.isCreating
                        text: "+ New Provider"
                        flat: true
                        Layout.fillWidth: true
                        onClicked: {
                            parent.isCreating = true
                            nameInput.forceActiveFocus()
                        }
                    }

                    // 状态 B: 输入框
                    TextField {
                        id: nameInput
                        visible: parent.isCreating
                        Layout.fillWidth: true
                        placeholderText: "Name (Enter to confirm)"
                        leftPadding: 10
                        background: Rectangle { color: "white"; border.color: "#0078d4" }

                        Keys.onEscapePressed: cancel()
                        onEditingFinished: commit()

                        function commit() {
                            if (!visible) return
                            // 调用 C++ 智能创建接口 (带查重)
                            configManager.createProvider(text)

                            // 重置 UI 并选中新项
                            text = ""
                            parent.isCreating = false
                            providerListView.currentIndex = providerListView.count - 1
                        }

                        function cancel() {
                            text = ""
                            parent.isCreating = false
                            providerListView.forceActiveFocus()
                        }
                    }
                }
            }
        }
    }

    // ========================================================
    // 右侧：Provider 详情页 (Detail View)
    // ========================================================
    Rectangle {
        SplitView.fillWidth: true
        color: "white"

        // 只有选中了有效项才显示详情页
        Loader {
            anchors.fill: parent
            // 确保索引有效
            active: providerListView.currentIndex >= 0 && providerListView.currentIndex < configManager.providers.length

            sourceComponent: Component {
                ProviderPage {
                    // 【关键数据流】
                    // 从 ConfigManager 的列表中取出当前 Provider 的 Name (ID) 传给子页面
                    // 子页面会根据这个 ID 自己去 model 中定位数据
                    providerId: configManager.providers[providerListView.currentIndex].name
                }
            }
        }

        // 空状态提示
        Text {
            visible: providerListView.currentIndex === -1 || configManager.providers.length === 0
            text: "Select or create a provider to configure"
            anchors.centerIn: parent
            color: "#aaa"
            font.pixelSize: 16
        }
    }
}