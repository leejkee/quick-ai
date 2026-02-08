add_library(QuickAILog STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Log.cpp
)

target_include_directories(QuickAILog PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(QuickAILog PUBLIC
        Qt6::Core
)