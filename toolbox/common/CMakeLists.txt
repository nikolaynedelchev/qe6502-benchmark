add_library(benchmark6502_common STATIC
    src/build_info.cpp
    src/singlestep.cpp
)

target_include_directories(benchmark6502_common
    PUBLIC include
    PRIVATE ${PROJECT_SOURCE_DIR}/cores/qe6502/harness/third_party
)

target_link_libraries(benchmark6502_common
    PUBLIC tools6502::tools6502
)

string(TOUPPER "${CMAKE_BUILD_TYPE}" BENCHMARK6502_BUILD_TYPE_UPPER)
set(BENCHMARK6502_CXX_FLAGS_TEXT "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BENCHMARK6502_BUILD_TYPE_UPPER}}")
if(NOT CMAKE_BUILD_TYPE)
    set(BENCHMARK6502_BUILD_TYPE_TEXT "multi-config/unspecified")
else()
    set(BENCHMARK6502_BUILD_TYPE_TEXT "${CMAKE_BUILD_TYPE}")
endif()

target_compile_definitions(benchmark6502_common
    PUBLIC BENCHMARK6502_SINGLESTEP_LIGHT_ROOT="${PROJECT_SOURCE_DIR}/toolbox/common/SingleStepTestsLight"
    PRIVATE BENCHMARK6502_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
    PRIVATE BENCHMARK6502_CXX_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
    PRIVATE BENCHMARK6502_CXX_FLAGS="${BENCHMARK6502_CXX_FLAGS_TEXT}"
    PRIVATE BENCHMARK6502_BUILD_TYPE="${BENCHMARK6502_BUILD_TYPE_TEXT}"
)

target_compile_features(benchmark6502_common PUBLIC cxx_std_20)
