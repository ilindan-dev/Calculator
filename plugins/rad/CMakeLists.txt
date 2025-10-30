add_library(rad_plugin MODULE src/rad.cpp)

target_compile_definitions(rad_plugin PRIVATE PLUGIN_EXPORTS)
target_link_libraries(rad_plugin PRIVATE calculator)

set_target_properties(rad_plugin PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)