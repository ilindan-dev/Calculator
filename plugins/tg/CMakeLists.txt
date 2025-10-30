add_library(tg_plugin MODULE src/tg.cpp)

target_compile_definitions(tg_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(tg_plugin PRIVATE calculator)

set_target_properties(tg_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)