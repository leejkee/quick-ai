cmake_minimum_required(VERSION 3.23)
qt_add_executable(QuickAIApp main.cpp)

target_link_libraries(QuickAIApp PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        QuickAIUiplugin
        QuickAIService
)