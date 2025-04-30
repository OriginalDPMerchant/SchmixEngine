cmake_minimum_required(VERSION 3.20)

file(GLOB Src "*.h" "*.inl" "*.cpp")
file(GLOB Src_Reflection "Reflection/*.h" "Reflection/*.inl" "Reflection/*.cpp")
file(GLOB Src_Application "Application/*.h" "Application/*.inl" "Application/*.cpp")
file(GLOB Src_Debugging "Debugging/*.h" "Debugging/*.inl" "Debugging/*.cpp")
file(GLOB Src_Graphics "Graphics/*.h" "Graphics/*.inl" "Graphics/*.cpp")
file(GLOB Src_Graphics-Core "Graphics/Core/*.h" "Graphics/Core/*.inl" "Graphics/Core/*.cpp")
file(GLOB Src_Graphics-Util "Graphics/Util/*.h" "Graphics/Util/*.inl" "Graphics/Util/*.cpp")
file(GLOB Src_Graphics-Renderer "Graphics/Renderer/*.h" "Graphics/Renderer/*.inl" "Graphics/Renderer/*.cpp")
file(GLOB Src_Util "Util/*.h" "Util/*.inl" "Util/*.cpp")
file(GLOB Src_Input "Input/*.h" "Input/*.inl" "Input/*.cpp")
file(GLOB Src_GameFramework "GameFramework/*.h" "GameFramework/*.inl" "GameFramework/*.cpp")
file(GLOB Src_FileSystem "FileSystem/*.h" "FileSystem/*.inl" "FileSystem/*.cpp")

add_library("${CMAKE_PROJECT_NAME}-Core" ${Src} ${Src_Reflection} ${Src_Application} ${Src_Debugging} ${Src_Graphics} ${Src_Graphics-Core} ${Src_Graphics-Util} ${Src_Graphics-Renderer} ${Src_Util} ${Src_Input} ${Src_GameFramework} ${Src_FileSystem} "${SCHMIX_ENGINE_ROOT}/SchmixEngine/ThirdParty/stb_image.cpp")

set_target_properties("${CMAKE_PROJECT_NAME}-Core" PROPERTIES
	LINKER_LANGUAGE CXX
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	CXX_STANDARD_REQUIRED ON
)

install(TARGETS "${CMAKE_PROJECT_NAME}-Core" EXPORT ${CMAKE_PROJECT_NAME}
    ARCHIVE DESTINATION ${SCHMIX_ENGINE_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${SCHMIX_ENGINE_INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${SCHMIX_ENGINE_INSTALL_BIN_DIR}
)

source_group("" FILES ${Src})
source_group("Reflection" FILES ${Src_Reflection})
source_group("Application" FILES ${Src_Application})
source_group("Debugging" FILES ${Src_Debugging})
source_group("Graphics" FILES ${Src_Graphics})
source_group("Graphics/Core" FILES ${Src_Graphics-Core})
source_group("Graphics/Util" FILES ${Src_Graphics-Util})
source_group("Graphics/Renderer" FILES ${Src_Graphics-Renderer})
source_group("Util" FILES ${Src_Util})
source_group("Input" FILES ${Src_Input})
source_group("GameFramework" FILES ${Src_GameFramework})
source_group("FileSystem" FILES ${Src_FileSystem})
