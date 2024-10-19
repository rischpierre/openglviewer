cmake_minimum_required(VERSION 3.12)

project(openglviewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLFW_BUILD_WAYLAND OFF)  # if not it will fail try to build wayland

find_package(OpenGL 4.6 REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glad CONFIG REQUIRED)

file(GLOB_RECURSE sources source/*.cpp include/*.hpp)
add_executable(${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
#include_directories(include/glad)
include_directories(include)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw assimp::assimp spdlog::spdlog_header_only glm::glm-header-only glad::glad)
