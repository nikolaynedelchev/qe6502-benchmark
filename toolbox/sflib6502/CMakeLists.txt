add_library(sflib6502_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/sflib6502/lib6502.c
    src/smoke.cpp
)

target_include_directories(sflib6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/sflib6502
)

target_compile_features(sflib6502_toolbox PUBLIC cxx_std_17)
