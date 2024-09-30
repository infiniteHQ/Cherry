cmake_minimum_required(VERSION 3.16)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(example_shared SHARED 
${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(example_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

file(GLOB_RECURSE example EXAMPLESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ uikit_build)
target_link_libraries(example_shared PUBLIC editor)

add_executable(example ${EXAMPLESOURCE} main.cpp)

file(
    GLOB_RECURSE
    UIKIT_SRC
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/../../platform/*.h" "${PROJECT_SOURCE_DIR}/../../platform/*.hpp" "${PROJECT_SOURCE_DIR}/../../platform/*.cpp"
    "${PROJECT_SOURCE_DIR}/../../src/*.h" "${PROJECT_SOURCE_DIR}/../../src/*.hpp" "${PROJECT_SOURCE_DIR}/../../src/*.cpp"
    "${PROJECT_SOURCE_DIR}/../../components/*.h" "${PROJECT_SOURCE_DIR}/../../components/*.hpp" "${PROJECT_SOURCE_DIR}/../../components/*.cpp"
    "${PROJECT_SOURCE_DIR}/../../kit/*.h" "${PROJECT_SOURCE_DIR}/../../kit/*.hpp" "${PROJECT_SOURCE_DIR}/../../kit/*.cpp"
)

add_library(editor STATIC ${UIKIT_SRC})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

find_package(Vulkan REQUIRED)
target_link_libraries(editor PUBLIC Vulkan::Vulkan)
target_link_libraries(editor PUBLIC imgui)
target_link_libraries(editor PUBLIC glm)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(editor PUBLIC ${SDL2_LIBRARIES})
target_link_libraries(editor PUBLIC SDL2_image)
target_link_libraries(editor PUBLIC stbimage)
target_link_libraries(editor PUBLIC spdlog)

target_link_libraries(example PRIVATE example_shared)

add_custom_command(TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/ressources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ressources
)
