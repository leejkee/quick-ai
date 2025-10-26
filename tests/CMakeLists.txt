FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)
add_executable(qalib-test
        fakehttpserver.h
        conversation_test.cpp
        llmclient_test.cpp
        deepseekadapter_test.cpp
)

target_link_libraries(qalib-test PRIVATE
        quick-ai-lib
        Catch2::Catch2WithMain
)

add_test(NAME qalib-test COMMAND qalib-test)