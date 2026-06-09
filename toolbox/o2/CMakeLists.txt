add_library(o2_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/o2/O2.cpp
    src/icpu.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
    src/smoke.cpp
)

target_include_directories(o2_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/o2
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(o2_toolbox
    PUBLIC benchmark6502_common
    PRIVATE tools6502::tools6502
)

target_compile_features(o2_toolbox PUBLIC cxx_std_20)
