file(GLOB_RECURSE QASERVICE_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(QuickAIService STATIC ${QASERVICE_SRC})

set_target_properties(QuickAIService PROPERTIES AUTOMOC ON)

target_link_libraries(QuickAIService PRIVATE Qt6::Core)

target_link_libraries(QuickAIService PUBLIC QuickAILib)

target_include_directories(QuickAIService PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)