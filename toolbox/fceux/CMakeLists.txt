add_library(fceux_toolbox STATIC
    src/klaus_benchmark.cpp
    src/smoke.cpp
    src/stubs.cpp
    ${PROJECT_SOURCE_DIR}/cores/fceux/src/x6502.cpp
)

target_include_directories(fceux_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/fceux/src
)

target_link_libraries(fceux_toolbox
    PUBLIC benchmark6502_common
)

target_compile_features(fceux_toolbox PUBLIC cxx_std_20)
