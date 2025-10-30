add_executable(calculator_app src/main.cpp)

find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(calculator_app PRIVATE
        calculator_impl
        Boost::program_options
)

add_custom_command(TARGET calculator_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/plugins
        $<TARGET_FILE_DIR:calculator_app>/plugins
)