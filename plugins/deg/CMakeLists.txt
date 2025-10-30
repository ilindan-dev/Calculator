add_library(deg_plugin MODULE src/deg.cpp)

target_compile_definitions(deg_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(deg_plugin PRIVATE calculator)

set_target_properties(deg_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)