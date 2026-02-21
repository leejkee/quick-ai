import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: root
    width: 1024
    height: 768
    visible: true
    title: "Quick AI Settings"
    color: "#f5f5f7"

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
                    ListElement { name: "LLM Runtime Config"; icon: "🤖"; pageIndex: 1 }
                    ListElement { name: "User Configuration"; icon: "📝"; pageIndex: 2 }
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
            clip: true

            StackLayout {
                id: viewStack
                anchors.fill: parent
                anchors.margins: 20
                currentIndex: 0

                GeneralSettingsPage {
                    viewModel: appConfigViewModel
                }

                LLMConfigPage {
                    runtimeViewModel: llmRuntimeViewModel
                    paramsViewModel: modelParamsViewModel
                }

                UserConfigPage {
                    viewModel: llmInitViewModel
                }
            }
        }
    }
}
