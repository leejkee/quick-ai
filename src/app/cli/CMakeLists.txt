add_executable(qacli-nostreamingclient nostreaming_llmclient_cli.cpp)
target_link_libraries(qacli-nostreamingclient PRIVATE quick-ai-lib)

add_executable(qacli-streamingclient streaming_llmclient_cli.cpp)
target_link_libraries(qacli-streamingclient PRIVATE quick-ai-lib)
