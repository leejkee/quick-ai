add_subdirectory(qalib)

if (QA_BUILD_UI)
    find_package(Qt6 6.8 REQUIRED COMPONENTS
            Core
            Gui
            Qml
            Quick
            QuickControls2
            QuickLayouts
    )
    qt_standard_project_setup(REQUIRES 6.8)
    add_subdirectory(app)
    add_subdirectory(qaservice)
    add_subdirectory(qaui)
endif ()