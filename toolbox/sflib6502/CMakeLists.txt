add_library(sflib6502_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/sflib6502/lib6502.c
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
)

target_include_directories(sflib6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/sflib6502
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(sflib6502_toolbox
    PUBLIC benchmark6502_common
)

target_compile_features(sflib6502_toolbox PUBLIC cxx_std_20)
