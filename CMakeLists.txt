# CMake required version
cmake_minimum_required(VERSION 3.14)

# Project name
project(Calculator VERSION 0.1.0 LANGUAGES CXX)

# ---------------------------------------------------- Settings C++ ----------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -------------------------------------------------- Output Settings ---------------------------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ----------------------------------------------- Connecting Dependencies ----------------------------------------------

include(FetchContent)

# 1. cxxopts
message(STATUS "Fetching cxxopts...")
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG        v3.1.1
)
FetchContent_MakeAvailable(cxxopts)

# 2. GoogleTest
message(STATUS "Fetching GoogleTest...")
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)

# Conflict resolution on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ------------------------------------------------- Add subdirectories -------------------------------------------------

add_subdirectory(./lib)
add_subdirectory(./plugins)
add_subdirectory(./app)
add_subdirectory(./tests)