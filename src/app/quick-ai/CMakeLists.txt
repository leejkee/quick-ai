cmake_minimum_required(VERSION 3.23)

set(QUICKAIAPP_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AppManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WindowManager.cpp
)

qt_add_executable(quickai_app ${QUICKAIAPP_SRC})

target_compile_features(quickai_app PRIVATE cxx_std_17)

target_include_directories(quickai_app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    target_sources(quickai_app PRIVATE WinGlobalShortcut.cpp)
endif()

target_link_libraries(quickai_app PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        Qt6::Widgets
        Qt6::DBus
        QuickAIUiSessionplugin
        QuickAIUISettingsplugin
        QuickAIService
        QuickAILog
        Qt6::QuickControls2
        $<$<PLATFORM_ID:Windows>:user32>
)