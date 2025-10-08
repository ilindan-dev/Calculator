include(GoogleTest)

add_executable(CalculatorTests

)

find_package(GTest REQUIRED)

target_link_libraries(
        CalculatorTests
        PRIVATE
        GTest::gtest
        GTest::gtest_main
)

gtest_discover_tests(CalculatorTests)

