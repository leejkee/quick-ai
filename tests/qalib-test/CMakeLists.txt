# qalib-test CMakeLists.txt

# Fetch Catch2 v3.8.1 (header-only)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

set(QALIB_TEST_SOURCES
    main.cpp
    test_llm_client_factory.cpp
    test_llm_conversation.cpp
    test_openai_client.cpp
)

add_executable(qalib-test ${QALIB_TEST_SOURCES})

target_compile_features(qalib-test PRIVATE cxx_std_17)

target_link_libraries(qalib-test PRIVATE
    QuickAILib
    LLMHttpServer
    Catch2::Catch2WithMain
    Qt6::Core
    Qt6::Network
)

# Register tests with CTest
include(Catch)
catch_discover_tests(qalib-test)
