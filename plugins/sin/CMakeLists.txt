add_library(sin_plugin MODULE src/sin.cpp)

target_compile_definitions(sin_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(sin_plugin PRIVATE calculator)

set_target_properties(sin_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)