add_library(factorial_plugin MODULE src/factorial.cpp)

target_compile_definitions(factorial_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(factorial_plugin PRIVATE calculator)

set_target_properties(factorial_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)