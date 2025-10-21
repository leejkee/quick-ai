file(GLOB_RECURSE QALIB_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
add_library(quick-ai-lib STATIC ${QALIB_SRC})

target_compile_features(quick-ai-lib PRIVATE cxx_std_17)

target_include_directories(quick-ai-lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(quick-ai-lib PUBLIC cxx_std_17)

target_link_libraries(quick-ai-lib PUBLIC
        cpr::cpr
        nlohmann_json
)