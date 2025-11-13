FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_library(qatest-httpserver STATIC
        llm_http_server.cpp
)

target_include_directories(qatest-httpserver PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qatest-httpserver PRIVATE
        nlohmann_json
)

add_executable(qalib-test
        conversation_test.cpp
        llm_client_test.cpp
        deepseek_adapter_test.cpp
)

target_link_libraries(qalib-test PRIVATE
        qatest-httpserver
        QuickAILib
        Catch2::Catch2WithMain
)

add_test(NAME qalib-test COMMAND qalib-test)