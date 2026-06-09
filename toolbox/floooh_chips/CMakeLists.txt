add_library(floooh_chips_toolbox STATIC
    src/chips_impl.cpp
    src/icpu.cpp
    src/instruction_cpu.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
    src/smoke.cpp
)

target_include_directories(floooh_chips_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/flooh_chips
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(floooh_chips_toolbox
    PUBLIC benchmark6502_common
    PRIVATE tools6502::tools6502
)

target_compile_features(floooh_chips_toolbox PUBLIC cxx_std_20)
