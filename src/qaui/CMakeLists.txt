cmake_minimum_required(VERSION 3.23)
qt_add_library(quick_ai_qml STATIC)
qt_add_qml_module(quick_ai_qml
        URI "qaui"
        QML_FILES
            "sessionwindow/ChatMainView.qml"
            "sessionwindow/InputView.qml"
            "sessionwindow/SessionView.qml"
            "sessionwindow/StatusBar.qml"
            "sessionwindow/ParamsConfig.qml"
            "sessionwindow/SelectionConfig.qml"
            "settingswindow/SettingsMainView.qml"
            "settingswindow/Divider.qml"
            "settingswindow/GeneralSettingsPage.qml"
            "settingswindow/LLMConfigPage.qml"
            "settingswindow/UserConfigPage.qml"
            "settingswindow/SectionHeader.qml"
            "settingswindow/SettingsItem.qml"
            "settingswindow/ModelParamsEditor.qml"
            "settingswindow/ProviderEditor.qml"
            "TrayIcon.qml"
        RESOURCES
            "icons/user-config.svg"
            "icons/llm-runtime.svg"
            "icons/appearance.svg"
            "icons/icon64.svg"
)