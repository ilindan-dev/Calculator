add_library(subfactorial_plugin MODULE src/subfactorial.cpp)

target_compile_definitions(subfactorial_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(subfactorial_plugin PRIVATE calculator)

set_target_properties(subfactorial_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)