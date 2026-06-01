add_library(mame_toolbox STATIC
    src/mame6502_cpu.cpp
)

target_include_directories(mame_toolbox
    PUBLIC include
)

target_compile_features(mame_toolbox PUBLIC cxx_std_20)
target_compile_options(mame_toolbox PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-sign-compare)
