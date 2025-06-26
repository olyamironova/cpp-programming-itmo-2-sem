cmake_minimum_required(VERSION 3.26)
project(LW8 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(LW8
        include/core/s24_inorder_tree.h
	include/core/s24_postorder_tree.h
	include/core/s24_preorder_tree.h
	include/interfaces/s24_inorder_set.h
	include/interfaces/s24_postorder_set.h
	include/interfaces/s24_preorder_set.h
        src/main.cpp)
