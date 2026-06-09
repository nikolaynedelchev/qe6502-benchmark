add_library(sflib65c02_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/sflib6502/lib6502.c
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
    src/instruction_cpu.cpp
)

target_include_directories(sflib65c02_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/sflib6502
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_compile_features(sflib65c02_toolbox PUBLIC cxx_std_20)


target_link_libraries(sflib65c02_toolbox
    PUBLIC benchmark6502_common
)
