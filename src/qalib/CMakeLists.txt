set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build all libraries as static")
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
set(CURL_STATICLIB ON CACHE BOOL "Force static curl build for cpr")
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG  1.12.0
)
FetchContent_MakeAvailable(json cpr)
file(GLOB_RECURSE QALIB_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
add_library(QuickAILib STATIC ${QALIB_SRC})

target_compile_features(QuickAILib PRIVATE cxx_std_17)

target_include_directories(QuickAILib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(QuickAILib PUBLIC cxx_std_17)

target_link_libraries(QuickAILib PUBLIC
        cpr::cpr
        nlohmann_json
)