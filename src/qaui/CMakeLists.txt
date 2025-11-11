cmake_minimum_required(VERSION 3.23)
qt_add_library(QuickAIUi STATIC)
qt_add_qml_module(QuickAIUi
        URI qaui
        QML_FILES
            "Main.qml"
            "InputView.qml"
            "SessionView.qml"
)
