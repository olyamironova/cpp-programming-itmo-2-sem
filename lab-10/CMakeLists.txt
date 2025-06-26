cmake_minimum_required(VERSION 3.14)

project(
    labwork10
    VERSION 1.0
    DESCRIPTION "C++ Year 2024 labwork 10"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)