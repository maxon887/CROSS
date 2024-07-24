cmake_minimum_required(VERSION 3.9.1)

project(Demo)

include_directories(../../../Sources)
include_directories(../../../Sources/Core)
include_directories(../../../Sources/Internals)
include_directories(../../../Sources/Experimental)
include_directories(../../../Sources/Libs)

file(GLOB_RECURSE GLOBAL_SRC ../../../Sources/*.*)
list(FILTER GLOBAL_SRC EXCLUDE REGEX "../../../Sources/Platform/.*" )

#windows specific
include_directories(../../../Sources/Platform/Windows)
file(GLOB WINDOWS_SRC ../../../Sources/Platform/Windows/*.cpp)
add_definitions(-DWIN)
add_definitions(-DOPENGL)

add_executable(${PROJECT_NAME} ${GLOBAL_SRC} ${WINDOWS_SRC})

find_package(GLEW REQUIRED)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../../Sources" FILES ${GLOBAL_SRC})