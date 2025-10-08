add_library(calculator INTERFACE)

target_include_directories(
        calculator INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(calculator INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator/calculator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator/plugin.hpp
)

add_library(calculator_impl STATIC)
target_sources(calculator_impl PRIVATE src/calculator.cpp src/plugin.cpp)

target_link_libraries(calculator_impl PRIVATE calculator)