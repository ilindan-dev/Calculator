cmake_minimum_required(VERSION 4.0)
project(Calculator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(DEFINED ENV{VCPKG_ROOT} AND IS_DIRECTORY $ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")
endif()

enable_testing()

add_subdirectory(./app)
add_subdirectory(./lib)
add_subdirectory(./tests)
add_subdirectory(./plugins)
