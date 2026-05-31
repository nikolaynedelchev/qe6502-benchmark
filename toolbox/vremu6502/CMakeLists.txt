add_library(vremu6502_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/vremu6502/src/vrEmu6502.c
    src/klaus_benchmark.cpp
    src/smoke.cpp
)

target_include_directories(vremu6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/vremu6502/src
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_compile_definitions(vremu6502_toolbox
    PRIVATE VR_EMU_6502_STATIC
)

target_compile_features(vremu6502_toolbox PUBLIC cxx_std_17)
