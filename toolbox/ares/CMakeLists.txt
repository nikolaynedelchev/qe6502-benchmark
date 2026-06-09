add_library(ares_toolbox STATIC
    src/klaus_benchmark.cpp
    src/singlestep.cpp
    src/instruction_cpu.cpp
    src/smoke.cpp
    ${PROJECT_SOURCE_DIR}/cores/ares/ares/component/processor/mos6502/mos6502.cpp
)

target_include_directories(ares_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/ares/ares
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/ares/nall
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(ares_toolbox
    PUBLIC benchmark6502_common
)

target_compile_features(ares_toolbox PUBLIC cxx_std_20)
