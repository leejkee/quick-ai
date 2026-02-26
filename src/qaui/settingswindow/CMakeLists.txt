cmake_minimum_required(VERSION 3.23)
qt_add_library(QuickAIUISettings STATIC)
qt_add_qml_module(QuickAIUISettings
        URI "qaui.settingswindow"
        QML_FILES
            "MainView.qml"
            "Divider.qml"
            "GeneralSettingsPage.qml"
            "LLMConfigPage.qml"
            "UserConfigPage.qml"
            "SectionHeader.qml"
            "SettingsItem.qml"
            "ModelParamsEditor.qml"
            "ProviderEditor.qml"
)
