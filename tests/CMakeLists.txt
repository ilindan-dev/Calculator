find_package(Threads REQUIRED)

add_executable(CalculatorTests
        src/main_test.cpp
        src/basic_test.cpp
        src/plugin_test.cpp
)

target_link_libraries(CalculatorTests PRIVATE
        GTest::gtest
        Threads::Threads
        calculator_impl
)

target_compile_definitions(CalculatorTests PRIVATE
        PLUGIN_DIR="${CMAKE_BINARY_DIR}/bin"
)

add_dependencies(CalculatorTests
        plugin_add
        plugin_sub
        plugin_mul
        plugin_div
        plugin_pow
        plugin_sin
        plugin_cos
        plugin_tg
        plugin_ctg
        plugin_ln
        plugin_deg
        plugin_rad
        plugin_factorial
        plugin_subfactorial
)

include(GoogleTest)
gtest_discover_tests(CalculatorTests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)