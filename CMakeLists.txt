cmake_minimum_required(VERSION 3.10)

project(LearnOpenGL VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_ADD_DEFINITIONS ON)

set(GLFW_BUILD_PATH ${CMAKE_SOURCE_DIR}/libs/glfw/build)

add_subdirectory(libs/glfw)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall -Wextra -Wpedantic -Wshadow -Wformat=2
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_BUILD_PATH}/src/libglfw3.a)

if (UNIX AND NOT APPLE) 
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)
endif()
