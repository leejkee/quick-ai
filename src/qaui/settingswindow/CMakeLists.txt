cmake_minimum_required(VERSION 3.23)
qt_add_library(QuickAIUiSettings STATIC)
qt_add_qml_module(QuickAIUiSettings
        URI "qaui.settingswindow"
        QML_FILES
            "MainView.qml"
            "AppDivider.qml"
            "GeneralSettingsPage.qml"
            "LLMConfigPage.qml"
            "SectionHeader.qml"
            "SettingsItem.qml"
)
