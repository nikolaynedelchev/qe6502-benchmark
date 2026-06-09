add_library(mame_toolbox STATIC
    src/mame6502_cpu.cpp
    src/instruction_cpu.cpp
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
)

target_include_directories(mame_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(mame_toolbox
    PUBLIC benchmark6502_common
    PRIVATE tools6502::tools6502
)

target_compile_features(mame_toolbox PUBLIC cxx_std_20)
target_compile_options(mame_toolbox PRIVATE -Wno-unused-variable -Wno-unused-function -Wno-sign-compare)
