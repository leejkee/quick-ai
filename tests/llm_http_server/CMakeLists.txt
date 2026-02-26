# LLM HTTP Server library for testing

# Fetch nlohmann/json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(LLMHttpServer STATIC
    llm_http_server.cpp
)

target_include_directories(LLMHttpServer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LLMHttpServer PUBLIC
    nlohmann_json::nlohmann_json
)
