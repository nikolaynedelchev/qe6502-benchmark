add_library(clk_toolbox STATIC
    src/klaus_benchmark.cpp
    src/smoke.cpp
    ${PROJECT_SOURCE_DIR}/cores/clk/Processors/6502/Implementation/6502Storage.cpp
)

target_include_directories(clk_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/clk
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(clk_toolbox
    PUBLIC benchmark6502_common
)

target_compile_features(clk_toolbox PUBLIC cxx_std_20)
