add_library(benchmark6502_common STATIC
    src/singlestep.cpp
)

target_include_directories(benchmark6502_common
    PUBLIC include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/qe6502/harness/third_party
)

target_compile_definitions(benchmark6502_common
    PUBLIC BENCHMARK6502_SINGLESTEP_LIGHT_ROOT="${PROJECT_SOURCE_DIR}/toolbox/common/SingleStepTestsLight"
)

target_compile_features(benchmark6502_common PUBLIC cxx_std_17)
