cmake_minimum_required(VERSION 3.23)
qt_add_library(QuickAIUiSession STATIC)
qt_add_qml_module(QuickAIUiSession
        URI "qaui.sessionwindow"
        QML_FILES
            "Main.qml"
            "InputView.qml"
            "SessionView.qml"
            "StatusBar.qml"
            "ParamsConfig.qml"
            "SelectionConfig.qml"
)
