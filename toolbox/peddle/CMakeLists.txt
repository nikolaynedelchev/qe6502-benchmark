add_library(peddle_toolbox STATIC
    ${PROJECT_SOURCE_DIR}/cores/peddle/Peddle/Peddle.cpp
    ${PROJECT_SOURCE_DIR}/cores/peddle/Peddle/PeddleDebugger.cpp
    ${PROJECT_SOURCE_DIR}/cores/peddle/Peddle/PeddleDisassembler.cpp
    ${PROJECT_SOURCE_DIR}/cores/peddle/Peddle/StrWriter.cpp
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/singlestep.cpp
)

target_include_directories(peddle_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/peddle/Peddle
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(peddle_toolbox
    PUBLIC benchmark6502_common
)

target_compile_features(peddle_toolbox PUBLIC cxx_std_20)
