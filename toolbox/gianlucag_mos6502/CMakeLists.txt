add_library(gianlucag_mos6502_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/gianlucag_mos6502/mos6502.cpp
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
)

target_include_directories(gianlucag_mos6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/gianlucag_mos6502
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(gianlucag_mos6502_toolbox
    PUBLIC benchmark6502_common
)

target_compile_features(gianlucag_mos6502_toolbox PUBLIC cxx_std_17)

# Enable the core's optional NMOS undocumented opcode table for SingleStepTests.
target_compile_definitions(gianlucag_mos6502_toolbox PRIVATE ILLEGAL_OPCODES)
