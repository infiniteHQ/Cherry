
cmake_minimum_required(VERSION 3.16)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")


# Création de la bibliothèque partagée
add_library(example_shared SHARED 
${CMAKE_CURRENT_SOURCE_DIR}/example.cpp
)

# Définir les chemins d'inclusion pour la bibliothèque partagée
target_include_directories(example_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)
# Liste des fichiers sources pour l'exécutable principal
file(GLOB_RECURSE example EXAMPLESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add editor
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ uikit_build)
target_link_libraries(example_shared PUBLIC editor)


# Création de l'exécutable principal
add_executable(example ${EXAMPLESOURCE} example.cpp)


add_library(editor
    # editor requirements
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/GUI/editor/ImGui/ImGuiTheme.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/GUI/editor/UI/UI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/GUI/editor/UI/Spinner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/GUI/editor/ApplicationGUI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../platform/GUI/editor/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/core/Log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/icons.cpp
)

find_package(Vulkan REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(editor PUBLIC Vulkan::Vulkan)
target_link_libraries(editor PUBLIC glfw)
target_link_libraries(editor PUBLIC imgui)
target_link_libraries(editor PUBLIC glm)
target_link_libraries(editor PUBLIC stbimage)
target_link_libraries(editor PUBLIC spdlog::spdlog)


# Liaison avec la bibliothèque partagée
target_link_libraries(example PRIVATE example_shared)

