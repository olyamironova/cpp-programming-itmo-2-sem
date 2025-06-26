add_executable(${PROJECT_NAME}_tests
    BinarySearchTreeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
    GTest::GTest
    GTest::Main
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_test(
    NAME ${PROJECT_NAME}_ContainerTests
    COMMAND ${PROJECT_NAME}_tests
)