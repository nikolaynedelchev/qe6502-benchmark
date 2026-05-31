add_library(fake6502_toolbox STATIC
    src/fake6502_bridge.c
    src/klaus_benchmark.cpp
    src/smoke.cpp
)

target_include_directories(fake6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/fake6502
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_compile_features(fake6502_toolbox PUBLIC cxx_std_17)
