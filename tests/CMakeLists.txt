include(GoogleTest)

add_executable(CalculatorTests
        src/main_test.cpp
        src/basic_test.cpp
        src/plugin_test.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(
        CalculatorTests
        PRIVATE
        GTest::gtest
        Threads::Threads
        calculator_impl
)

target_compile_definitions(CalculatorTests PRIVATE
        PLUGIN_DIR="${CMAKE_BINARY_DIR}/plugins"
        PLUGIN_EXTENSION="${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

add_dependencies(CalculatorTests
        sin_plugin
        cos_plugin
        tg_plugin
        ctg_plugin
        ln_plugin
        deg_plugin
        rad_plugin
        factorial_plugin
        subfactorial_plugin
)

gtest_discover_tests(CalculatorTests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)