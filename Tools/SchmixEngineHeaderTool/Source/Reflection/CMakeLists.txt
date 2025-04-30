cmake_minimum_required(VERSION 3.20)

project("SchmixEngineHeaderTool")

file(GLOB_RECURSE Src "*.h" "*.inl" "*.cpp")

add_library(${CMAKE_PROJECT_NAME} ${Src})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
	LINKER_LANGUAGE CXX
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	CXX_STANDARD_REQUIRED ON
)