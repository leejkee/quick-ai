file(GLOB_RECURSE QASERVICE_SRC CONFIGURE_DEPENDS
       ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
       ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

list(REMOVE_ITEM QASERVICE_SRC
       "${CMAKE_CURRENT_SOURCE_DIR}/include/ConfigService/ConfigService.h"
       "${CMAKE_CURRENT_SOURCE_DIR}/ConfigService.cpp"
)


# set(QASERVICE_SRC
#         "${CMAKE_CURRENT_SOURCE_DIR}/include/ChatService/ChatService.h"
#         "${CMAKE_CURRENT_SOURCE_DIR}/ChatService.cpp"
#         "${CMAKE_CURRENT_SOURCE_DIR}/include/ChatService/ParamsConfig.h"
#         "${CMAKE_CURRENT_SOURCE_DIR}/ParamsConfig.cpp"
#         "${CMAKE_CURRENT_SOURCE_DIR}/include/ChatService/SelectionConfig.h"
#         "${CMAKE_CURRENT_SOURCE_DIR}/SelectionConfig.cpp"
#         "${CMAKE_CURRENT_SOURCE_DIR}/include/ChatViewModel/ChatViewModel.h"
#         "${CMAKE_CURRENT_SOURCE_DIR}/ChatViewModel.cpp"
#         "${CMAKE_CURRENT_SOURCE_DIR}/include/MessageListModel/MessageListModel.h"
#         "${CMAKE_CURRENT_SOURCE_DIR}/MessageListModel.cpp"
#         "${CMAKE_CURRENT_SOURCE_DIR}/include/UserSettings/UserSettings.h"
# )


add_library(QuickAIService STATIC ${QASERVICE_SRC})

set_target_properties(QuickAIService PROPERTIES AUTOMOC ON)

target_link_libraries(QuickAIService PRIVATE Qt6::Core)

target_link_libraries(QuickAIService PUBLIC
        QuickAILib
)

target_include_directories(QuickAIService PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)