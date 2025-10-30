add_library(ln_plugin MODULE src/ln.cpp)

target_compile_definitions(ln_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(ln_plugin PRIVATE calculator)

set_target_properties(ln_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)