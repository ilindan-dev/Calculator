add_library(cos_plugin MODULE src/cos.cpp)

target_compile_definitions(cos_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(cos_plugin PRIVATE calculator)

set_target_properties(cos_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)