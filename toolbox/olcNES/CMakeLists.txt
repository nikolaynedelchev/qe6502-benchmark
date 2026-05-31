add_library(olcNES_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/olcNES/Bus.cpp
    ${PROJECT_SOURCE_DIR}/cores/olcNES/olc6502.cpp
    src/smoke.cpp
    src/klaus_benchmark.cpp
)

target_include_directories(olcNES_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/olcNES
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_compile_features(olcNES_toolbox PUBLIC cxx_std_17)
