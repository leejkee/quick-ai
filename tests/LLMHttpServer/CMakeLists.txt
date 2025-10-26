add_library(qatest-httpserver STATIC

)

target_include_directories(qatest-httpserver PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qatest-httpserver PUBLIC
        nlohmann_json
)

