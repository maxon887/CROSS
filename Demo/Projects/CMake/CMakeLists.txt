cmake_minimum_required(VERSION 3.9.1)

project(Demo)
set (CMAKE_CXX_STANDARD 11)

include_directories(../../../Sources)
include_directories(../../../Sources/Core)
include_directories(../../../Sources/Internals)
include_directories(../../../Sources/Experimental)
include_directories(../../../Sources/Libs)
include_directories(../../../Demo/Sources)

file(GLOB_RECURSE CROSS_SRC ../../../Sources/*.*)
list(FILTER CROSS_SRC EXCLUDE REGEX "../../../Sources/Platform/.*" )

file(GLOB_RECURSE DEMO_SRC ../../../Demo/Sources/*.*)
list(FILTER DEMO_SRC EXCLUDE REGEX "../../../Demo/Sources/ThirdParty/ImGui/.*" )

file(GLOB IMGUI_SRC ../../../Demo/Sources/ThirdParty/ImGui/*.cpp)

add_executable(${PROJECT_NAME} WIN32 ${CROSS_SRC} ${DEMO_SRC} ${IMGUI_SRC})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../../Sources" FILES ${CROSS_SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../../Demo/Sources" FILES ${DEMO_SRC})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

if (APPLE)
    include_directories(../../../Sources/Platform/MacOS)
    file(GLOB MACOS_SRC ../../../Sources/Platform/MacOS/*.*)
    target_sources(${PROJECT_NAME} PRIVATE ${MACOS_SRC})
    
    add_definitions(-DMACOS)
    
    find_package(ASSIMP REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLFW3 REQUIRED)
    find_library(COCOA_LIBRARY Cocoa)
    
    target_link_libraries (${PROJECT_NAME} PRIVATE
        ${OPENGL_gl_LIBRARY}
        glfw
        ${ASSIMP_LIBRARIES}
        ${COCOA_LIBRARY}
        GLEW::GLEW
        "${CMAKE_SOURCE_DIR}/../../../Binaries/MacOS/libfmod.dylib")

endif (APPLE)

if (WIN32)
    include_directories(../../../Sources/Platform/Windows)
    file(GLOB WINDOWS_SRC ../../../Sources/Platform/Windows/*.cpp)
	target_sources(${PROJECT_NAME} PRIVATE ${WINDOWS_SRC})

    add_definitions(-DWIN)
    add_definitions(-DOPENGL)

	find_library(GLEW_LIB NAMES glew32
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../Demo/Projects/Windows)
	find_library(FMOD_LIB NAMES fmod fmod_vc
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../Demo/Projects/Windows)
	find_library(ASSIMP_LIB NAMES assimp
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../../../Demo/Projects/Windows)

	target_link_libraries (${PROJECT_NAME} PRIVATE
		${GLEW_LIB}
		${OPENGL_gl_LIBRARY}
		${FMOD_LIB}
		${ASSIMP_LIB})

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../Binaries/Windows/assimp.dll"
		"${CMAKE_CURRENT_BINARY_DIR}/Debug/assimp.dll")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../Binaries/Windows/fmod.dll"
		"${CMAKE_CURRENT_BINARY_DIR}/Debug/fmod.dll")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../Binaries/Windows/glew32.dll"
		"${CMAKE_CURRENT_BINARY_DIR}/Debug/glew32.dll")

endif (WIN32)
