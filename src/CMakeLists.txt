add_subdirectory(qalib)


if (QA_BUILD_UI)
    add_subdirectory(app)
    add_subdirectory(qaservice)
    add_subdirectory(qaui)
    add_subdirectory(qalog)
endif ()