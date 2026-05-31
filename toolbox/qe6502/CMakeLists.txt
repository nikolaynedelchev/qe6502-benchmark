add_library(qe6502_toolbox STATIC
    src/klaus_benchmark.cpp
    src/smoke.cpp
    src/singlestep.cpp
)

target_include_directories(qe6502_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(qe6502_toolbox
    PUBLIC qe6502::cpp
    PUBLIC benchmark6502_common
)

target_compile_features(qe6502_toolbox PUBLIC cxx_std_17)
