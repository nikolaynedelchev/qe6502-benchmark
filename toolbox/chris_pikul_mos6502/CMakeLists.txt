add_library(chris_pikul_mos6502_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/bus.cpp
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/cpu.cpp
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/cpu_address_modes.cpp
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/cpu_instructions.cpp
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/instructions.cpp
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/memory.cpp
    ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/src/utils.cpp
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
)

target_include_directories(chris_pikul_mos6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/chris_pikul_mos6502/include
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(chris_pikul_mos6502_toolbox
    PUBLIC benchmark6502_common
)


target_compile_features(chris_pikul_mos6502_toolbox PUBLIC cxx_std_20)
