import QtQuick
import Qt.labs.platform

SystemTrayIcon {
    id: mainTray
    visible: true
    icon.source: "qrc:/qt/qml/qaui/icons/icon64.svg"
    tooltip: "Quick AI"

    menu: Menu {
        MenuItem {
            text: qsTr("Show Chat Window")
            onTriggered: systemTray.showChat()
        }
        MenuItem {
            text: qsTr("Quick AI Settings")
            onTriggered: systemTray.showSettings()
        }
        MenuSeparator {}
        MenuItem {
            text: qsTr("Exit")
            onTriggered: systemTray.quitApp()
        }
    }

    onActivated: (reason) => {
        systemTray.handleActivated(reason)
    }
}