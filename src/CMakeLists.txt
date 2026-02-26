add_subdirectory(qalib)
add_subdirectory(qalog)

if (QA_BUILD_UI)
    add_subdirectory(app)
    add_subdirectory(qaservice)
    add_subdirectory(qaui)
endif ()