cmake_minimum_required(VERSION 3.20)

project("SchmixEngine")

set(SCHMIX_ENGINE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${SCHMIX_ENGINE_ROOT})

add_subdirectory("SchmixEngine/Core")
add_subdirectory("SchmixEngine/OpenGL")
add_subdirectory("SchmixEngine/SDL2/OpenGL")
add_subdirectory("SchmixEngine/SDL2/Input")

add_executable(${CMAKE_PROJECT_NAME} "Test.cpp")
target_link_libraries(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}-Core" "${CMAKE_PROJECT_NAME}-SDL2_OpenGL" "${CMAKE_PROJECT_NAME}-SDL2_Input")

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
					LINKER_LANGUAGE CXX
					CXX_STANDARD 17
					CXX_EXTENSIONS OFF
					CXX_STANDARD_REQUIRED ON
					)