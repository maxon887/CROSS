cmake_minimum_required(VERSION 3.9.1)

project(Demo)

set (CMAKE_CXX_STANDARD 11)

if (!ANDROID)
	include_directories(../../../Sources)
	include_directories(../../../Sources/Core)
	include_directories(../../../Sources/Internals)
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

	add_definitions(-DOPENGL)

	if (APPLE)
	    include_directories(../../../Sources/Platform/MacOS)
	    file(GLOB MACOS_SRC ../../../Sources/Platform/MacOS/*.*)
	    target_sources(${PROJECT_NAME} PRIVATE ${MACOS_SRC})
	    
	    add_definitions(-DMACOS)
	    add_definitions(-DCROSS_DEBUG)
	    
	    find_package(GLEW REQUIRED)
	    find_package(GLFW3 REQUIRED)
	    find_library(COCOA_LIBRARY Cocoa)
	    
	    target_link_libraries (${PROJECT_NAME} PRIVATE
	        ${OPENGL_gl_LIBRARY}
	        glfw
	        ${COCOA_LIBRARY}
	        GLEW::GLEW
	        "${CMAKE_SOURCE_DIR}/../../../Binaries/MacOS/libfmod.dylib"
			"${CMAKE_SOURCE_DIR}/../../../Binaries/MacOS/libassimpMAC.dylib")


	endif (APPLE)

	if (WIN32)
	    include_directories(../../../Sources/Platform/Windows)
	    file(GLOB WINDOWS_SRC ../../../Sources/Platform/Windows/*.cpp)
		target_sources(${PROJECT_NAME} PRIVATE ${WINDOWS_SRC})

	    add_definitions(-DWIN)

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

else()
	file(GLOB CROSS_SRC
	    "../../Sources/*.cpp"
	    "../../Sources/Scenes/*.cpp"
	    "../../Sources/Scenes/Simple/*.cpp"
	    "../../Sources/Scenes/Light/*.cpp"
	    "../../Sources/Scenes/Maps/*.cpp"
	    "../../Sources/Scenes/Misc/*.cpp"
	    "../../Sources/ThirdParty/ImGui/*.cpp"
	    "../../Sources/UI/*.cpp"
	    "../../../Sources/Core/*.cpp"
	    "../../../Sources/Math/*.cpp"
	    "../../../Sources/Shaders/*.cpp"
	    "../../../Sources/Internals/*.cpp"
	    "../../../Sources/Experimental/*.cpp"
	    "../../../Sources/Utils/*.cpp"
	    "../../../Sources/Libs/SOIL/*.c"
	    "../../../Sources/Libs/TinyXML2/*.cpp"
	    "../../../Sources/Platform/*.cpp"
	    "../../../Sources/Platform/Android/*.cpp"

	)

	add_library( cross
	             SHARED
	             ${CROSS_SRC} )

	find_library( # Sets the name of the path variable.
	              log-lib

	              # Specifies the name of the NDK library that
	              # you want CMake to locate.
	              log )

	add_library( fmod SHARED IMPORTED )
	add_library( assimp SHARED IMPORTED )
	add_library( freetype2 SHARED IMPORTED )

	set_target_properties( fmod PROPERTIES IMPORTED_LOCATION
						  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfmod.so )

	set_target_properties( assimp PROPERTIES IMPORTED_LOCATION
					  ../../../../src/main/jniLibs/${ANDROID_ABI}/libassimp.so )

	set_target_properties( freetype2 PROPERTIES IMPORTED_LOCATION
				  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfreetype2.so )

	target_link_libraries( # Specifies the target library.
							cross
							${log-lib}
							android
							GLESv2
							EGL
							fmod
							assimp )

	include_directories(../../../Sources)
	include_directories(../../../Sources/Core)
	include_directories(../../Sources)
	include_directories(../../../Sources/Libs)
	include_directories(../../../Sources/Libs/FreeType)
	include_directories(../../../Sources/Libs/FreeType/freetype2)
	include_directories(../../../Sources/Libs/TinyXML)
	include_directories(../../../Sources/Libs/Assimp)
	include_directories(../../../Sources/Libs/SOIL)

endif()