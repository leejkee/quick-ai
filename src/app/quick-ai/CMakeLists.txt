cmake_minimum_required(VERSION 3.23)

file(GLOB_RECURSE QUICKAIAPP_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AppManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WindowManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/IPCManager.cpp
        
)

qt_add_executable(quickai_app ${QUICKAIAPP_SRC})

target_compile_features(quickai_app PRIVATE cxx_std_17)

set_target_properties(quickai_app PROPERTIES AUTOMOC ON)

set_property(TARGET quickai_app PROPERTY AUTORCC ON)

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
        Qt6::Network
        quick_ai_qmlplugin
        QuickAIService
        QuickAILog
        Qt6::QuickControls2
        $<$<PLATFORM_ID:Windows>:user32>
)