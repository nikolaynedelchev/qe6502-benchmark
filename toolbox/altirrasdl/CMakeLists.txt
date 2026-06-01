add_library(altirrasdl_toolbox STATIC
    src/smoke.cpp
    src/klaus_benchmark.cpp
    src/stubs.cpp
    ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCPU/source/co6502.cpp
    ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCPU/source/co6502machine.cpp
    ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCPU/source/decode6502.cpp
    ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCPU/source/memorymap.cpp
    ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCore/source/logging.cpp
)

target_include_directories(altirrasdl_toolbox
    PUBLIC include
    PUBLIC ${PROJECT_SOURCE_DIR}/toolbox/common/include
    PUBLIC ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/h
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCPU/h
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/ATCore/h
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/altirrasdl/src/system/h
)

target_link_libraries(altirrasdl_toolbox PUBLIC benchmark6502_common)
target_compile_features(altirrasdl_toolbox PUBLIC cxx_std_20)
