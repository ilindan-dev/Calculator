add_executable(calculator_app src/main.cpp)

target_link_libraries(
        calculator_app PRIVATE
        calculator_impl
        cxxopts::cxxopts
)