cmake_minimum_required(VERSION 3.20)

project(ChessBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR}/ChessBot)

file(GLOB_RECURSE SOURCES
    ${PROJECT_ROOT}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_ROOT}/include
        ${CMAKE_SOURCE_DIR}/3party
)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg-master/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
    PRIVATE glad::glad
    PRIVATE glm::glm
    PRIVATE opengl32        
)