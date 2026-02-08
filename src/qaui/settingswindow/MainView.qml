import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: root
    width: 1024
    height: 768
    visible: true
    title: "AI Studio Settings Manager"
    color: "#f5f5f7" // 浅灰背景，避免纯白刺眼

    // =================================================================
    // MOCK VIEWMODEL (模拟 C++ 后端逻辑)
    // 实际项目中，这通常是一个注册为上下文属性的 C++ 类
    // =================================================================
    QtObject {
        id: mockViewModel

        // General Settings Properties
        property int themeIndex: 0
        property int fontSize: 14
        property string systemPrompt: "You are a helpful AI assistant."

        // LLM Configuration Properties
        property int providerIndex: 0
        property var providers: ["DeepSeek", "Qwen", "OpenAI", "Anthropic"]
        property var models: [] // 动态列表
        property int modelIndex: 0
        property real temperature: 0.7
        property int maxTokens: 2048

        // Logic: 模拟 C++ 信号槽，当 Provider 改变时更新 Model 列表
        onProviderIndexChanged: updateModels()

        function updateModels() {
            // 模拟后端根据 Provider 获取不同模型列表的逻辑
            let p = providers[providerIndex];
            if (p === "DeepSeek") {
                models = ["deepseek-chat", "deepseek-coder"];
            } else if (p === "Qwen") {
                models = ["qwen-max", "qwen-plus", "qwen-turbo"];
            } else if (p === "OpenAI") {
                models = ["gpt-4-turbo", "gpt-3.5-turbo"];
            } else {
                models = ["claude-3-opus", "claude-3-sonnet"];
            }
            modelIndex = 0; // 重置模型选择
        }

        // 初始化
        Component.onCompleted: updateModels()
    }

    // =================================================================
    // MAIN LAYOUT
    // =================================================================
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 1. Sidebar
        Rectangle {
            Layout.preferredWidth: 240
            Layout.fillHeight: true
            color: "#ffffff"
            border.color: "#e5e5e5"
            border.width: 1

            ListView {
                id: navList
                anchors.fill: parent
                anchors.topMargin: 20
                focus: true
                model: ListModel {
                    ListElement { name: "General Settings"; icon: "⚙️"; pageIndex: 0 }
                    ListElement { name: "LLM Configuration"; icon: "🤖"; pageIndex: 1 }
                }

                delegate: Rectangle {
                    id: delegateItem
                    width: navList.width
                    height: 48
                    color: ListView.isCurrentItem ? "#e6f2ff" : (mouseArea.containsMouse ? "#f5f5f5" : "transparent")

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 20
                        spacing: 12

                        Text {
                            text: model.icon
                            font.pixelSize: 18
                        }

                        Text {
                            text: model.name
                            font.pixelSize: 14
                            font.bold: ListView.isCurrentItem
                            color: ListView.isCurrentItem ? "#0066cc" : "#1e1e1e"
                            Layout.fillWidth: true
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            navList.currentIndex = index
                            viewStack.currentIndex = model.pageIndex
                        }
                    }
                }
            }
        }

        // 2. Content Area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
            clip: true // 防止内容溢出到 Sidebar

            StackLayout {
                id: viewStack
                anchors.fill: parent
                anchors.margins: 20
                currentIndex: 0

                // 页面 0: General
                GeneralSettingsPage {
                    viewModel: mockViewModel
                }

                // 页面 1: LLM Config
                LLMConfigPage {
                    viewModel: mockViewModel
                }
            }
        }
    }
}
