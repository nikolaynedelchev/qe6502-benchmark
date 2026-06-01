add_library(fake65c02_toolbox STATIC
    src/fake65c02_bridge.c
    src/klaus_benchmark.cpp
    src/smoke.cpp
    src/singlestep.cpp
)

target_include_directories(fake65c02_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/fake6502
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_compile_features(fake65c02_toolbox PUBLIC cxx_std_17)


target_link_libraries(fake65c02_toolbox
    PUBLIC benchmark6502_common
)
