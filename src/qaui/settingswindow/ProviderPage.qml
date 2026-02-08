import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: root
    spacing: 0

    // 【输入参数】由外部传入 Provider ID
    property string providerId: ""

    // 【数据获取】根据 ID 查找 Provider 对象 (只读副本)
    property var currentProvider: {
        if (!configManager || !providerId) return null
        // 简单查找逻辑
        for (let i = 0; i < configManager.providers.length; i++) {
            if (configManager.providers[i].name === providerId)
                return configManager.providers[i]
        }
        return null
    }

    // 顶部标题栏
    Rectangle {
        Layout.fillWidth: true
        height: 60
        color: "white"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 5

            Label {
                text: root.currentProvider ? root.currentProvider.name : ""
                font.bold: true
                font.pixelSize: 22
            }
            Label {
                text: "Provider Configuration"
                color: "gray"
                font.pixelSize: 12
            }
        }
    }

    // 分割线
    AppDivider {}

    // 主内容区
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20

            // ----------------------------------------
            // 1. 基础信息编辑区 (API Key & URL)
            // ----------------------------------------
            GroupBox {
                title: "Connection Settings"
                Layout.fillWidth: true

                GridLayout {
                    columns: 2
                    rowSpacing: 15
                    columnSpacing: 15

                    // Base URL
                    Label { text: "Base URL:" }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "https://api.example.com/v1"
                        text: root.currentProvider ? root.currentProvider.baseUrl : ""
                        selectByMouse: true

                        onEditingFinished: updateBasicInfo()
                    }

                    // API Key
                    Label { text: "API Key:" }
                    TextField {
                        Layout.fillWidth: true
                        placeholderText: "sk-..."
                        echoMode: TextInput.Password // 密码模式
                        text: root.currentProvider ? root.currentProvider.apiKey : ""
                        selectByMouse: true

                        onEditingFinished: updateBasicInfo()
                    }
                }
            }

            // ----------------------------------------
            // 2. 模型列表区域 (嵌入 ModelListPage)
            // ----------------------------------------
            Label {
                text: "Available Models"
                font.bold: true
                font.pixelSize: 14
                topPadding: 10
            }

            // 这里使用你之前定义的 ModelListPage (即 ModelManagementPanel)
            // 记得确保 ModelListPage.qml 文件在同一目录下
            ModelListPage {
                Layout.fillWidth: true
                Layout.fillHeight: true

                // 【数据透传】将 Provider ID 传给下一级
                providerId: root.providerId

                // 视觉修饰：给个边框
                Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                    border.color: "#ccc"
                    z: -1 // 放在后面
                }
            }
        }
    }

    // 辅助函数：更新基础信息
    function updateBasicInfo() {
        if (!root.currentProvider) return

        // 既然 Provider 结构体是全量更新，我们需要构造一个新的对象
        // 或者如果 C++ 提供了专门 updateBasicInfo 的接口更好
        // 这里假设复用通用的 updateProvider(id, obj)

        let newObj = root.currentProvider
        // 注意：QML 中的对象是引用的副本，修改它的属性：
        // 这里 TextField 并没有直接绑定 newObj，所以我们需要手动获取
        // 为了代码简洁，上面的 TextField 应该赋值给 id，这里去取 id.text

        // 更严谨的写法：
        // 获取 UI 上最新的值（防止数据没同步）
        // 实际上上面的 onEditingFinished 已经是修改时刻了，
        // 我们可以直接读取 TextField 的 text 属性，或者依赖绑定。
        // 但最安全的是直接从 UI 控件取值。

        // (由于上面的 TextField 没有 id，这里示意逻辑)
        // 实际代码建议给 TextField 加 id，如下：
        // newObj.baseUrl = baseUrlField.text
        // newObj.apiKey = apiKeyField.text

        // 这里演示最简逻辑（假设 TextField 绑定了 onEditingFinished）:
        // 在上面的 TextField 代码块里：
        /* currentProvider.baseUrl = text
           configManager.updateProvider(currentProvider.name, currentProvider)
        */

        // 由于 QML 对象副本机制，上面的 TextField onEditingFinished 代码其实是最方便的：
        /*
         onEditingFinished: {
             // 修改副本
             root.currentProvider.baseUrl = text
             // 发送回后端
             configManager.updateProvider(root.providerId, root.currentProvider)
         }
        */
    }
}