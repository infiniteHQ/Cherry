cmake_minimum_required(VERSION 3.16)
project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build options
option(CHERRY_STATIC "Link Cherry statically" ON)
option(CHERRY_DEBUG "Enable Cherry debug tools" OFF)
option(CHERRY_PACKAGE "Enable packaging features with pyinstaller" OFF)
option(CHERRY_ENABLE_NET "Enable net features" OFF)
option(CHERRY_ENABLE_AUDIO "Enable audio features" ON)
option(CHERRY_ENABLE_LINUXDRM "Enable Linux Direct Rendering Manager" OFF)
option(CHERRY_ENABLE_COMPUTE_SHADERS "Enable compute shaders" ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../ cherry_build)
add_executable(hello main.cpp)
target_link_libraries(hello PRIVATE cherry)

add_custom_command(TARGET hello POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
 
# Find shader compiler
find_program(GLSLANG_VALIDATOR glslangValidator HINTS $ENV{VULKAN_SDK}/bin)
if(NOT GLSLANG_VALIDATOR)
    message(WARNING "glslangValidator not found, trying glslc...")
    find_program(GLSLC glslc HINTS $ENV{VULKAN_SDK}/bin)
    if(NOT GLSLC)
        message(FATAL_ERROR "No SPIR-V compiler found! Install Vulkan SDK.")
    endif()
    set(SHADER_COMPILER ${GLSLC})
    set(SHADER_COMPILER_FLAGS "")
else()
    set(SHADER_COMPILER ${GLSLANG_VALIDATOR})
    set(SHADER_COMPILER_FLAGS "-V")
endif()

message(STATUS "Using shader compiler: ${SHADER_COMPILER}")

# Compile shaders
set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders)
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/shaders)

# Compile compute shader
set(SHADER_SRC ${SHADER_SOURCE_DIR}/shader.comp)
set(SHADER_SPV ${SHADER_OUTPUT_DIR}/shader.comp.spv)

add_custom_command(
    OUTPUT ${SHADER_SPV}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIR}
    COMMAND ${SHADER_COMPILER} ${SHADER_COMPILER_FLAGS} -o ${SHADER_SPV} ${SHADER_SRC}
    DEPENDS ${SHADER_SRC}
    COMMENT "Compiling compute shader to SPIR-V"
)

add_custom_target(shaders DEPENDS ${SHADER_SPV})
add_dependencies(hello shaders)
