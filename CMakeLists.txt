cmake_minimum_required(VERSION 3.25)

include(FetchContent)

project(Calculator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Fetching cxxopts...")
FetchContent_Declare(
        cxxopts
        URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.1.1.zip
)
FetchContent_MakeAvailable(cxxopts)

message(STATUS "Fetching GoogleTest...")
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(./lib)
add_subdirectory(./plugins)
add_subdirectory(./app)
add_subdirectory(./tests)