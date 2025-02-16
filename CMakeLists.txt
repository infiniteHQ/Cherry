cmake_minimum_required(VERSION 3.16)
project(libcherry)

set(CMAKE_CXX_STANDARD 20)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

find_package(Vulkan REQUIRED)

add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/stb-image)
add_subdirectory(lib/sdl2)

if(CHERRY_CEF)
    add_subdirectory(lib/cef-project)
endif()

if(CHERRY_NET)
    if (NOT TARGET restclient-cpp)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/restcpp restcpp_build)
    endif()
endif()

file(GLOB_RECURSE CHERRY_SRC CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/platform/*.h" "${PROJECT_SOURCE_DIR}/platform/*.hpp" "${PROJECT_SOURCE_DIR}/platform/*.cpp" "${PROJECT_SOURCE_DIR}/platform/*.cc"
    "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.hpp" "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/components/*.h" "${PROJECT_SOURCE_DIR}/components/*.hpp" "${PROJECT_SOURCE_DIR}/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/kit/*.h" "${PROJECT_SOURCE_DIR}/kit/*.hpp" "${PROJECT_SOURCE_DIR}/kit/*.cpp"
)

add_library(cherry SHARED ${CHERRY_SRC})

set_target_properties(cherry PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)



target_link_libraries(cherry PRIVATE Vulkan::Vulkan)
target_link_libraries(cherry PUBLIC SDL2)
target_link_libraries(cherry PUBLIC imgui)
target_link_libraries(cherry PRIVATE glm)
target_link_libraries(cherry PRIVATE stbimage)
target_link_libraries(cherry PRIVATE spdlog)

target_include_directories(cherry PUBLIC "${Vulkan_INCLUDE_DIRS}")
target_include_directories(cherry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb_image")
target_include_directories(cherry PUBLIC "${PROJECT_SOURCE_DIR}/lib/spdlog/include")

if(CHERRY_NET)
    target_compile_definitions(cherry PUBLIC CHERRY_NET)
    target_link_libraries(cherry PUBLIC restclient-cpp)
endif()

if(CHERRY_CEF)
    target_include_directories(cherry PUBLIC ${CEF_ROOT})
    target_compile_definitions(cherry PUBLIC CHERRY_CEF)

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(CEF_LIB_EXTENSION ".dll")
        set(CEF_RELEASE_DIR "${CMAKE_BINARY_DIR}/Release")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CEF_LIB_EXTENSION ".so")
        set(CEF_RELEASE_DIR "${CMAKE_BINARY_DIR}/libcef")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CEF_LIB_EXTENSION ".dylib")
        set(CEF_RELEASE_DIR "${CMAKE_BINARY_DIR}/Frameworks")
    endif()

    set(CEF_LIBRARIES "${CEF_RELEASE_DIR}/libcef${CEF_LIB_EXTENSION}")
    target_link_libraries(cherry PUBLIC ${CEF_LIBRARIES})

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(CEF_EXTRA_LIBRARIES
            "${CEF_RELEASE_DIR}/chrome_elf${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/libEGL${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/libGLESv2${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/d3dcompiler_47${CEF_LIB_EXTENSION}"
        )
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CEF_EXTRA_LIBRARIES
            "${CEF_RELEASE_DIR}/libEGL${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/libGLESv2${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/libvk_swiftshader${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/libvulkan${CEF_LIB_EXTENSION}"
        )
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(CEF_EXTRA_LIBRARIES
            "${CEF_RELEASE_DIR}/libEGL${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/libGLESv2${CEF_LIB_EXTENSION}"
            "${CEF_RELEASE_DIR}/Chromium Embedded Framework.framework"
        )
    endif()

    target_link_libraries(cherry PUBLIC ${CEF_EXTRA_LIBRARIES})
    target_link_libraries(cherry PUBLIC libcef_dll_wrapper)

    file(GLOB_RECURSE CEF_RESOURCES
        "${CEF_RELEASE_DIR}/*.pak"
        "${CEF_RELEASE_DIR}/*.bin"
        "${CEF_RELEASE_DIR}/*.json"
        "${CEF_RELEASE_DIR}/icudtl.dat"
    )
    file(COPY ${CEF_RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/Resources")
endif()


add_custom_command(TARGET cherry POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:cherry>
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
