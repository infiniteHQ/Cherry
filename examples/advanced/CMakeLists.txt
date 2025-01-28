cmake_minimum_required(VERSION 3.16)
project(advanced)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CHERRY_CEF ON)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ cherry_build)

add_executable(advanced ${CHERRYSOURCES} main.cpp)
target_link_libraries(advanced PRIVATE cherry)

set_target_properties(advanced PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    BUILD_RPATH "$ORIGIN/../lib"
)

add_custom_command(TARGET advanced POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:cherry>
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)

add_custom_command(TARGET advanced POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
