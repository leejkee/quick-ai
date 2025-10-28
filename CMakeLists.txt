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

option(QA_BUILD_TESTS "Build tests" OFF)
option(QA_BUILD_UI "Build the Qt-based UI" OFF)

include(FetchContent)
add_subdirectory(src)
if (QA_BUILD_TESTS)
	include(CTest)
	enable_testing()
	add_subdirectory(tests)
endif ()
