file(GLOB_RECURSE QALIB_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(QuickAILib STATIC ${QALIB_SRC})

target_compile_features(QuickAILib PRIVATE cxx_std_17)

target_include_directories(QuickAILib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(QuickAILib PUBLIC cxx_std_17)

target_link_libraries(QuickAILib PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Network
        QuickAILog
)