# -------------------------------------------------- Interface library -------------------------------------------------
add_library(calculator INTERFACE)

target_include_directories(
        calculator INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_sources(calculator INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator/calculator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator/plugin.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator/i_calculator_registrar.hpp
)

# ----------------------------------------------- Implementation Library -----------------------------------------------
add_library(calculator_impl STATIC)

target_compile_definitions(
        calculator_impl PRIVATE
        PLUGIN_EXTENSION="${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

target_sources(calculator_impl PRIVATE
        src/calculator.cpp
        src/tokenizer.cpp
        src/dll_loader.cpp
        src/dll_loader.hpp
)

target_link_libraries(calculator_impl PUBLIC calculator)

# ---------------------------------------------- Linking system libraries ----------------------------------------------

if(UNIX)
    target_link_libraries(calculator_impl PRIVATE dl)
endif()