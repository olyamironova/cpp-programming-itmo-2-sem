cmake_minimum_required(VERSION 3.14)

project(
    labwork11
    VERSION 1.0
    DESCRIPTION "C++ Year 2024 labwork 11"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} main.cpp)
enable_testing()
add_subdirectory(tests)