cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(Quick-Ai 
	VERSION 0.1
	DESCRIPTION "A quick ai desktop application"
	LANGUAGES CXX
)

include(FetchContent)
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


find_package(Qt6 6.8 REQUIRED COMPONENTS
	Core
	Widgets
	Quick
)

add_subdirectory(src)
add_subdirectory(examples)