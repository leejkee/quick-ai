add_executable(qacli-no-streaming-client no_streaming_llm_client_cli.cpp)
target_link_libraries(qacli-no-streaming-client PRIVATE quick-ai-lib)

add_executable(qacli-streaming-client streaming_llm_client_cli.cpp)
target_link_libraries(qacli-streaming-client PRIVATE quick-ai-lib)
