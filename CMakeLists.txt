cmake_minimum_required(VERSION 4.0)
project(Calculator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(./app)
add_subdirectory(./lib)
add_subdirectory(./tests)
add_subdirectory(./plugins)
