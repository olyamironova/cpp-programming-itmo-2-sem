cmake_minimum_required(VERSION 3.26)
project(cpr_example)

set(CMAKE_CXX_STANDARD 23)

# Set to C++ 11 if you are using cpr <= 1.9.x
# More: https://github.com/libcpr/cpr#supported-releases
# set(CMAKE_CXX_STANDARD 11)

# Set a default build type if none was specified
# Based on: https://github.com/openchemistry/tomviz/blob/master/cmake/BuildType.cmake
#set(DEFAULT_BUILD_TYPE "Release")
#if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
#  set(DEFAULT_BUILD_TYPE "Debug")
#endif()
#if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
#  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
#  # Set the possible values of build type for cmake-gui
#  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
#endif()


if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_executable(cpr_example src/main.cpp
    src/config/ConfigParser.cpp
    src/data/GetCoordinates.cpp
    src/data/GetWeather.cpp
    src/ui/PrintWeather.cpp
    include/config/ConfigParser.h
    include/data/GetCoordinates.h
    include/data/GetWeather.h
    include/ui/PrintWeather.h
)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3020c34ae2b732121f37433e61599c34535e68a8) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
)

FetchContent_MakeAvailable(json)
#---

target_link_libraries(cpr_example PRIVATE cpr::cpr nlohmann_json::nlohmann_json)


FetchContent_Declare(pdcurses URL https://github.com/wmcbrine/PDCurses/archive/refs/tags/3.9.tar.gz)
FetchContent_MakeAvailable(pdcurses)

target_link_libraries(${PROJECT_NAME} PRIVATE pdcurses)