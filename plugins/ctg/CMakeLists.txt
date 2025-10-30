add_library(ctg_plugin MODULE src/ctg.cpp)

target_compile_definitions(ctg_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(ctg_plugin PRIVATE calculator)

set_target_properties(ctg_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)