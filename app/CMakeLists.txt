add_executable(calculator_app src/main.cpp)

target_link_libraries(
        calculator_app PRIVATE
        calculator_impl
        cxxopts::cxxopts
)

add_custom_command(TARGET calculator_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:calculator_app>/plugins
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/plugins
        $<TARGET_FILE_DIR:calculator_app>/plugins
)