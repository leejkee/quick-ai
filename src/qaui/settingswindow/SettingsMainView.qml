import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls.Fusion

Window {
    id: root
    visible: true
    title: "Quick AI Settings"
    width: 800
    height: 600

    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color windowBackground: palette.window
    readonly property color sidebarBackground: palette.base
    readonly property color sidebarBorder: palette.mid
    readonly property color contentAreaBackground: "transparent"
    readonly property color selectedItemBackground: palette.highlight
    readonly property color selectedItemText: palette.highlightedText
    readonly property color hoverItemBackground: palette.midlight
    readonly property color normalText: palette.text
    readonly property color selectedText: palette.highlightedText
    readonly property color accentText: palette.highlight

    // ============================================
    // Dimensions
    // ============================================
    readonly property int sidebarWidth: 240
    readonly property int sidebarMargin: 20
    readonly property int itemHeight: 48
    readonly property int iconSize: 18
    readonly property int textSize: 14
    readonly property int iconSpacing: 12
    readonly property int leftMargin: 20
    readonly property int borderWidth: 1

    color: windowBackground

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // 1. Sidebar
        Rectangle {
            Layout.preferredWidth: root.sidebarWidth
            Layout.fillHeight: true
            color: root.sidebarBackground
            border.color: root.sidebarBorder
            border.width: root.borderWidth

            ListView {
                id: navList
                anchors.fill: parent
                anchors.topMargin: root.sidebarMargin
                focus: true
                model: ListModel {
                    ListElement { 
                        name: "General Settings"; 
                        icon: "qrc:/qt/qml/qaui/icons/appearance.svg"; 
                        pageIndex: 0 
                    }
                    ListElement { 
                        name: "LLM Runtime Config"; 
                        icon: "qrc:/qt/qml/qaui/icons/llm-runtime.svg"; 
                        pageIndex: 1 
                    }
                    ListElement { 
                        name: "User Configuration"; 
                        icon: "qrc:/qt/qml/qaui/icons/user-config.svg"; 
                        pageIndex: 2 
                    }
                }

                delegate: Rectangle {
                    id: delegateItem
                    width: navList.width
                    height: root.itemHeight
                    color: ListView.isCurrentItem ? root.selectedItemBackground : (mouseArea.containsMouse ? root.hoverItemBackground : "transparent")

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: root.leftMargin
                        spacing: root.iconSpacing

                        Image {
                            source: model.icon
                            sourceSize.width: root.iconSize
                            sourceSize.height: root.iconSize
                            fillMode: Image.PreserveAspectFit
                            Layout.alignment: Qt.AlignVCenter
                        }

                        Text {
                            text: model.name
                            font.pixelSize: root.textSize
                            font.bold: ListView.isCurrentItem
                            color: ListView.isCurrentItem ? root.selectedItemText : root.normalText
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
            color: root.contentAreaBackground
            clip: true

            StackLayout {
                id: viewStack
                anchors.fill: parent
                // anchors.margins: 20
                currentIndex: 0

                GeneralSettingsPage {
                    viewModel: appConfigViewModel
                }

                LLMConfigPage {
                    runtimeViewModel: llmRuntimeViewModel
                    paramsViewModel: modelParamsViewModel
                }

                UserConfigPage {
                    initViewModel: llmInitViewModel
                    providerViewModel: providerEditorViewModel
                }
            }
        }
    }
}
