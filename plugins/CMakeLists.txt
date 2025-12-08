function(create_calc_plugin plugin_name source_file)
    add_library(${plugin_name} MODULE ${source_file})

    target_compile_definitions(${plugin_name} PRIVATE PLUGIN_EXPORTS)

    target_link_libraries(${plugin_name} PRIVATE calculator)

    set_target_properties(${plugin_name} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            PREFIX ""
    )
endfunction()


# -------------------------------------------------- Basic operations --------------------------------------------------

create_calc_plugin(plugin_add "add/src/add.cpp")
create_calc_plugin(plugin_sub "sub/src/sub.cpp")
create_calc_plugin(plugin_mul "mul/src/mul.cpp")
create_calc_plugin(plugin_div "div/src/div.cpp")
create_calc_plugin(plugin_pow "pow/src/pow.cpp")

# ----------------------------------------------- Additional operations ------------------------------------------------

create_calc_plugin(plugin_sin "sin/src/sin.cpp")
create_calc_plugin(plugin_cos "cos/src/cos.cpp")
create_calc_plugin(plugin_tg "tg/src/tg.cpp")
create_calc_plugin(plugin_ctg "ctg/src/ctg.cpp")
create_calc_plugin(plugin_ln "ln/src/ln.cpp")
create_calc_plugin(plugin_deg "deg/src/deg.cpp")
create_calc_plugin(plugin_rad "rad/src/rad.cpp")
create_calc_plugin(plugin_factorial "factorial/src/factorial.cpp")
create_calc_plugin(plugin_subfactorial "subfactorial/src/subfactorial.cpp")